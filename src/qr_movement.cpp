#include <iostream>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include "nav_msgs/Odometry.h"
#include <stdio.h>
#include <string.h>


using namespace std;

// global element :
std::string _cmd_frame_id;
ros::Publisher _cmd_publisher;
double _dmax_l_speed, _max_a_speed;
int looping_order = 0;
const char* str = ""; // Faire un string local avec un strcpy.
float angle_z = 0.0; //angle suivant z
float angle_z_av = 0.0; //angle suivant z d'avant pour pouvoir calculer un angle de pi/2


//Recuperer l'information du qr code
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str() );
  str = msg->data.c_str();
}

//Recuperer l'angle selon l'axe z du robot
void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
{
  ROS_INFO("I received odom: [%f]", odom->pose.pose.orientation.w); //store z position values
  angle_z = 2 * asin(odom->pose.pose.orientation.z);
}


//On veut créer une commande de mouvement
void cmd_def(int n){
  cout << "scan_subscriber" << endl;

  // generate appropriate commande message :
  geometry_msgs::Twist cmd;

  cmd.linear.x = 0.1;
  cmd.linear.y = 0.0;
  cmd.linear.z = 0.0;

  cmd.angular.x = 0.0;
  cmd.angular.y = 0.0;
  cmd.angular.z = 0.0; //0.5 => 13s pour un tour

  //Boucle où on applique nos fonctions
  ros::Rate r(2); // 2 hz, frequence minimal pour que le robot ne s'interrompt pas dans son mouvement
  int loop = 0;
  int lim_loop = 1; //nombre de commande qu'il va falloir pour tourner, ici = 1 pour que le cas avancer soit correct.

  switch (n) {
    case 1: cmd.angular.z = 0.42; //Cas où on tourne à gauche
      cmd.linear.x = 0.0;
      angle_z_av = angle_z; //doit augmenter pour faire un angle de pi/2
      lim_loop = 10;
      break;
    case 2: cmd.angular.z = -0.42; //Cas où on tourne à droite
      cmd.linear.x = 0.0;
      angle_z_av = angle_z; //doit diminuer pour faire un angle de pi/2
      lim_loop = 10;
      break;
      /*
    case 3: cmd.angular.z = 0.18; //Cas où on tourne à gauche pour se remettre en place
      looping_order += 1;
      lim_loop = 2;
      break;
    case 4: cmd.angular.z = -0.18; //Cas où on tourne à droite pour se remettre en place
      looping_order += 1;
      lim_loop = 2;
      break;*/
    default: cmd.linear.x = 0.1; //avancer tout droit
    break;
  }

  //On regarde si on a déjà analyse le QRcode, pour ne pas avoir a l'analyser deux fois d'affile
  if(looping_order == 2){
    //On réinitialise la commande
    looping_order = 0;
    lim_loop = 1;
    cmd.angular.z = 0;
    cmd.linear.x = 0.0;
  }
/*
  while (ros::ok() && loop != lim_loop)
  {
    cout << "\tcommande linear: " << cmd.linear.x << ", angular: " << cmd.angular.z << endl;
    _cmd_publisher.publish( cmd );
    r.sleep();
    loop += 1;
  }*/


  if(n==1 || n==2){ //Si on doit tourner de pi/2
    while (ros::ok() && ((fabs(angle_z - angle_z_av) < 1.57) || (fabs(angle_z - angle_z_av) > 4.71))) //Changer la valeur de 1.57 et de 4.71 permettera de faire un meilleur angle de pi/2
    {
      cout << "\tcommande linear: " << cmd.linear.x << ", angular: " << cmd.angular.z << endl;
      printf("z = %f\t", angle_z);
      printf("z av = %f\t", angle_z_av);
      _cmd_publisher.publish( cmd );
      ros::spinOnce(); //Pour reactualiser la valeur de angle_z (et de tous les noeuds)

    }
  }
  else{
    while (ros::ok() && loop != lim_loop) //Sinon
    {
      cout << "\tcommande linear: " << cmd.linear.x << ", angular: " << cmd.angular.z << endl;
      _cmd_publisher.publish( cmd );
      r.sleep();
      loop += 1;
    }
  }


  str = ""; //On réinitialise le message pour laisser la commande pour avancer
}

int main(int argc, char **argv)
{
    // ROS:
    cout << "Initialize Torobo::Move: " << endl;
    ros::init(argc, argv, "qr_movement");
    ros::NodeHandle node;
    ros::Subscriber sub = node.subscribe("chatter", 1000, chatterCallback); //Pour se connecter au noeud chatter
    ros::Subscriber sub_angular = node.subscribe("odom", 1000, odomCallback); //pour se connecter au noeud odom
    ros::NodeHandle node_private("~");

    // Configuration Movement:
    std::string cmd_topic;

    if( !node_private.getParam("cmd_topic", cmd_topic) ) cmd_topic= "/cmd_vel_mux/input/navi";
    if( !node_private.getParam("cmd_frame_id", _cmd_frame_id) ) _cmd_frame_id= "base_link"; //frame=repere

    if( !node_private.getParam("linear_speed", _dmax_l_speed) ) _dmax_l_speed= 0.2;
    if( !node_private.getParam("angular_speed", _max_a_speed) ) _max_a_speed= 1.2;

    // publisher function:
    _cmd_publisher= node.advertise<geometry_msgs::Twist>( cmd_topic, 1 );

    // get the hand to ros:
    cout << "run torob move" << endl;

    ros::Rate r(2); // 2 hz, frequence minimal pour que le robot ne s'interrompt pas dans son mouvement
    while (ros::ok())
    {
      //Commande que l'on cherche à fournir
      if (std::strcmp(str, "gauche") == 0) {
        cmd_def(1);
      }
      else if (std::strcmp(str, "droite") == 0) {
        cmd_def(2);
      }
      else if (std::strcmp(str, "fin") == 0) {
        return 0;
      }
      else if (std::strcmp(str, "remettreG") == 0) { //A coder proprement
        cmd_def(3);
      }
      else if (std::strcmp(str, "remettreD") == 0) { //A coder proprement
        cmd_def(4);
      }
      else {
        cmd_def(0);
      }
      r.sleep();
      ros::spinOnce(); //Pour reactualiser les valeurs de tous les noeuds)
    }



    ros::spin();//ros tourne en boucle jusqu'a ce quon tue ros

    // Properly stop the program:
    cout << "close move" << endl;

    return 0;
}
