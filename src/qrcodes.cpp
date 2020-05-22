/*
 * Code used by Yann DE COSTER and Julien BOUDET
 */

//Includes pour l'analyse des QR Codes
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <zbar.h>

//Includes specifiques a ROS
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <sstream>

using namespace std;
using namespace cv;
using namespace zbar;


int main(int argc, char **argv) {

    ros::init(argc, argv, "QR");
    ros::NodeHandle n;
    ros::Publisher QR_pub = n.advertise<std_msgs::String>("chatter", 1000);

    // Touche clavier
    char key;
    // Image
    IplImage *image1;
    IplImage *image2;

    // Capture vidéo
    CvCapture *capture1;
    CvCapture *capture2;

    // Ouvrir le flux vidéo
    capture1 = cvCreateCameraCapture(1);
    capture2 = cvCreateCameraCapture(2);

    // Set 640 x 480 resolution 5 fps on cam1
    cvSetCaptureProperty(capture2, CV_CAP_PROP_FRAME_HEIGHT, 640);
    cvSetCaptureProperty(capture2,CV_CAP_PROP_FRAME_WIDTH, 480);
    cvSetCaptureProperty(capture2, CV_CAP_PROP_FPS, 5);

    // Set 640 x 480 resolution 5 fps on cam2
    cvSetCaptureProperty(capture1, CV_CAP_PROP_FRAME_HEIGHT, 640);
    cvSetCaptureProperty(capture1,CV_CAP_PROP_FRAME_WIDTH, 480);
    cvSetCaptureProperty(capture1, CV_CAP_PROP_FPS, 5);

    // Vérifier si l'ouverture du flux est ok
    if ((!capture1) || (!capture2)) {
       printf("Ouverture du flux vidéo impossible !\n");
       return 1;
    }

    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {

       // On récupère une image
       image1 = cvQueryFrame(capture1);
       image2 = cvQueryFrame(capture2);

       // On affiche l'image dans une fenêtre
       cvShowImage( "IMT PDR window 1", image1);
       cvShowImage( "IMT PDR window 2", image2);

       //Scanner les deux flux videos
       //Mat(const IplImage* img, bool copyData=false);
        //IplImage *src=cvLoadImage("image.png",CV_LOAD_IMAGE_GRAYSCALE);
        ImageScanner scanner;
        scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
        cv::Mat frame(cv::cvarrToMat(image1));
        Mat grey;
        cvtColor(frame,grey,CV_BGR2GRAY);
        int width = frame.cols;
        int height = frame.rows;
        uchar* raw = (uchar *)(grey.data);
        Image image(width, height, "Y800", raw, width * height);
        int n = scanner.scan(image);
        char* str = "";
        int left;
        int right;
        int top;
        int bottom;
        std_msgs::String msg;
        std::stringstream ss;

        //Decodage du Qr code
        for(Image::SymbolIterator symbol = image.symbol_begin();
        symbol != image.symbol_end();
        ++symbol) {
                vector<Point> vp;
        // Montrer ce qu'on a obtenu
        //cout << "decoded " << symbol->get_type_name()  << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;
        // This is a message object. You stuff it with data, and then publish it.
        ss << "" + symbol->get_data();
        msg.data = ss.str();

        //Obtenir les quatre points extremmaux du qr code
        int pointsCount = symbol->get_location_size();
        int x[pointsCount];
        int y[pointsCount];
        for (int ii = 0; ii < pointsCount; ++ii) {
            x[ii] = symbol->get_location_x(ii);
            y[ii] = symbol->get_location_y(ii);
            //Pour initialiser left, right, top et bottom
            if (!ii) {
                left = x[ii];
                right = x[ii];
                top = y[ii];
                bottom = y[ii];
            }
            left = std::min(left, x[ii]);
            right = std::max(right, x[ii]);
            top = std::min(top, y[ii]);
            bottom = std::max(bottom, y[ii]);
        }
        //printf("rect(%d,%d,%d,%d), \n", left, top, right, bottom);

        strcpy(str = (char *)malloc(sizeof(char) * 100), msg.data.c_str());
        }

        cv::Mat frame2(cv::cvarrToMat(image2));
        Mat grey2;
        cvtColor(frame2,grey2,CV_BGR2GRAY);
        int width2 = frame2.cols;
        int height2 = frame2.rows;
        uchar* raw2 = (uchar *)(grey2.data);
        Image image2b(width2, height2, "Y800", raw2, width2 * height2);
        int n2 = scanner.scan(image2b);
        char* str2 = "";
        int left2;
        int right2;
        int top2;
        int bottom2;

        for(Image::SymbolIterator symbol2 = image2b.symbol_begin();
        symbol2 != image2b.symbol_end();
        ++symbol2) {
                vector<Point> vp;
        // Montrer ce qu'on a obtenu
        //cout << "decoded " << symbol2->get_type_name()  << " symbol2 \"" << symbol2->get_data() << '"' <<" "<< endl;
        // This is a message object. You stuff it with data, and then publish it.
        std_msgs::String msg2;
        std::stringstream ss2;
        ss2 << "" + symbol2->get_data();
        msg2.data = ss2.str();

        int pointsCount2 = symbol2->get_location_size();
        int x2[pointsCount2];
        int y2[pointsCount2];
        for (int ii = 0; ii < pointsCount2; ++ii) {
            x2[ii] = symbol2->get_location_x(ii);
            y2[ii] = symbol2->get_location_y(ii);
            //Pour initialiser left, right, top et bottom
            if (!ii) {
                left2 = x2[ii];
                right2 = x2[ii];
                top2 = y2[ii];
                bottom2 = y2[ii];
            }
            left2 = std::min(left2, x2[ii]);
            right2 = std::max(right2, x2[ii]);
            top2 = std::min(top2, y2[ii]);
            bottom2 = std::max(bottom2, y2[ii]);
        }
        //printf("rect(%d,%d,%d,%d), \n", left2, top2, right2, bottom2);
        strcpy(str2 = (char *)malloc(sizeof(char) * 100), msg2.data.c_str());
        }

        if (strcmp(str, str2) == 0 && strcmp(str, "") != 0){
          //Calcul de la distance pour savoir si on a la distance requise pour pouvoir lire le QR code d'arrête 9,6 cm dans notre labyrinthe.
          double entraxe = 0.06; // m
          double taux_pixel = 0.00025; // m/px => Il faut changer ce taux la
          double dist_focale = 0.37; // m (on a mesure 0.385) => Eventuellement celui la aussi doit etre change
          double dist_qr_gauche = dist_focale * entraxe / (taux_pixel * fabs(left - left2)); // m
          double dist_qr_droit = dist_focale * entraxe / (taux_pixel * fabs(right - right2)); // m
          double dist_qr = (dist_qr_droit + dist_qr_gauche) / 2; // m

          printf("dist : %lf\n", dist_qr);

          //Si la distance a notre qr code est inferieur a 50 cm, alors on transmet le message du qr code
          if(dist_qr<=0.85){
            ROS_INFO("%s", msg.data.c_str());
            printf("rect(%d,%d,%d,%d), \n", left, top, right, bottom);
            printf("rect(%d,%d,%d,%d), \n", left2, top2, right2, bottom2);
            QR_pub.publish(msg);
            /*
            if(std::strcmp(msg.data.c_str(),"fin") == 0){
              ROS_INFO("%s", msg.data.c_str());
              QR_pub.publish(msg);
              key = cvWaitKey(3000);
              cvReleaseCapture(&capture1);
              cvReleaseCapture(&capture2);
              cvDestroyWindow("IMT PDR window 1");
              cvDestroyWindow("IMT PDR window 2");
              return 0;
            }*/
          }


/*
          //On rajoute une condition pour remettre le robot droit
          if(((dist_qr_droit - dist_qr_gauche) > 0.01) && (dist_qr > 1.1)){
            std::stringstream ss2;
            ss2 << "remettreG";
            msg.data = ss2.str();
            ROS_INFO("%s", msg.data.c_str());
            QR_pub.publish(msg);
          }
          //On rajoute une condition pour remettre le robot droit
          if(((dist_qr_droit - dist_qr_gauche) < -0.01) && (dist_qr > 1.1)){
            std::stringstream ss2;
            ss2 << "remettreD";
            msg.data = ss2.str();
            ROS_INFO("%s", msg.data.c_str());
            QR_pub.publish(msg);
          }
*/
        }

        // On attend 10ms
        key = cvWaitKey(10);

    }


    cvReleaseCapture(&capture1);
    cvReleaseCapture(&capture2);
    cvDestroyWindow("IMT PDR window 1");
    cvDestroyWindow("IMT PDR window 2");

    return 0;

}
