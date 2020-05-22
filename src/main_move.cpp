#include <iostream>

#include <ros/ros.h>
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>

using namespace std;

// global element :
std::string _cmd_frame_id;

ros::Publisher _cmd_publisher;

double _dmax_l_speed, _max_a_speed;

// Callback :
//void goal_subscriber(const geometry_msgs::PoseStamped & g);


//On veut créer une commande de mouvement
void cmd_def(){

  cout << "scan_subscriber" << endl;

  // generate appropriate commande message :
  geometry_msgs::Twist cmd;

  cmd.linear.x = 0.0;
  cmd.linear.y = 0.0;
  cmd.linear.z = 0.0;

  cmd.angular.x = 0.0;
  cmd.angular.y = 0.0;
  cmd.angular.z = 0.5; //0.5 => 13s pour un tour

  cout << "\tcommande linear: " << cmd.linear.x << ", angular: " << cmd.angular.z << endl;

  _cmd_publisher.publish( cmd );
}


int main(int argc, char **argv)
{
    // ROS:
    cout << "Initialize Torobo::Move: " << endl;
    ros::init( argc, argv, "move" );
    ros::NodeHandle node;
    ros::NodeHandle node_private("~");

    // Configuration Movement:
    std::string cmd_topic;

    if( !node_private.getParam("cmd_topic", cmd_topic) ) cmd_topic= "/cmd_vel_mux/input/navi";
    if( !node_private.getParam("cmd_frame_id", _cmd_frame_id) ) _cmd_frame_id= "base_link"; //frame=repere

    if( !node_private.getParam("linear_speed", _dmax_l_speed) ) _dmax_l_speed= 0.2;
    if( !node_private.getParam("angular_speed", _max_a_speed) ) _max_a_speed= 1.2;

    // subscribers function:
    //ros::Subscriber sub2 = node.subscribe( goal_topic, 1, goal_subscriber );

    // publisher function:
    _cmd_publisher= node.advertise<geometry_msgs::Twist>( cmd_topic, 1 );

    //Boucle où on applique nos fonctions
    ros::Rate r(2); // 2 hz, frequence minimal pour que le robot ne s'interrompt pas dans son mouvement
    while (ros::ok())
    {
      cmd_def();
      r.sleep();
    }


    // get the hand to ros:
    cout << "run torob move" << endl;
    ros::spin();//ros tourne en boucle jusqu'a ce quon tue ros

    // Properly stop the program:
    cout << "close move" << endl;

    return 0;
}
