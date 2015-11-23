#include "opencv2/objdetect.hpp"


#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <image_transport/image_transport.h>
//#include <image_transfer/image.h>
#include <iostream>
#include <stdio.h>
 #include <cv_bridge/cv_bridge.h>

 #include <ros/ros.h>

    #include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>

    #include <cv_bridge/cv_bridge.h>
    #include <sstream> // for converting the command line parameter to integer
using namespace std;
using namespace cv;


//-------------- ROS HANDLE for PUBLISHER ----------------//





/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";

/** @function main */
int main(int argc, char *argv[])
{
    VideoCapture capture;
    Mat frame;
double total_time = 0.0;
    int c,cpt = 30;

//--------------------------ROS NODE DECLARATION -------------------------------------------------------------//

	ros::init(argc, argv, "controller_DG808");
	ros::NodeHandle n1;
	image_transport::ImageTransport it(n1);
	image_transport::Publisher pub = it.advertise("camera/image", 1);
	sensor_msgs::ImagePtr msg;
	sensor_msgs::CameraInfo cam_info_msg;

    double t = (double) cv::getTickCount();
    //-- 1. Load the cascades
   // if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
   // if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

    //-- 2. Read the video stream
    capture.open( -1 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }

    while (1) 
    {   capture.read(frame) ;
	t = ((double) cv::getTickCount() - t) / cv::getTickFrequency();
        total_time += t;
        cpt++;
        t = (double) cv::getTickCount();
        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
     	       break;
        }
	cv::putText(frame, cv::format("Average FPS=%d",cvRound(cpt/total_time)), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, 		cv::Scalar(0,0,255));
        //-- 3. Apply the classifier to the frame
       // detectAndDisplay( frame );
        std::vector<Rect> faces;
        Mat frame_gray;

  	cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    //***equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
    }
	// msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
        // pub.publish(msg);
	imshow( window_name, frame );
        //int c = waitKey(10);
        //if( (char)c == 27 ) { break; } // escape
 
    }
    return 0;
}

/** @function detectAndDisplay */
/*void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
    }
    //-- Show what you got
    imshow( window_name, frame );
}*/
