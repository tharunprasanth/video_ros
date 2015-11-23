#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/videoio.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer
#include <sensor_msgs/image_encodings.h> 
cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
cv::String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;
cv::String window_name = "Capture - Face detection";


    int main(int argc, char** argv)
    {  
     
      // Check if video source has been passed as a parameter
     if(argv[1] == NULL) return 1;
   
     ros::init(argc, argv, "image_publisher");
     ros::NodeHandle nh;
     image_transport::ImageTransport it(nh);
     image_transport::Publisher pub = it.advertise("camera/image", 1);

     // Convert the passed as command line parameter index for the video device to an integer
     std::istringstream video_sourceCmd(argv[1]);
     int video_source;
     // Check if it is indeed a number
     if(!(video_sourceCmd >> video_source)){printf("\n failed in >>"); return 1;}
   
     cv::VideoCapture cap(video_source);
     // Check if video device can be opened with the given index
     if(!cap.isOpened()) return 1;
     cv::Mat frame;
     sensor_msgs::ImagePtr msg;
   
     ros::Rate loop_rate(5);
     while (nh.ok()) {
       cap >> frame;
       // Check if grabbed frame is actually full with some content
       if(!frame.empty()) {
	// face detection
	

	/*std::vector<cv::Rect> faces;
         cv::Mat frame_gray;
  	cv::cvtColor( frame, frame_gray, CV_BGR2GRAY );
	cv::equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );

for ( size_t i = 0; i < faces.size(); i++ )
    {
        cv::Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        cv::ellipse( frame, center, cv::Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4, 8, 0 );

       /* cv::Mat faceROI = frame_gray( faces[i] );
        std::vector<cv::Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            cv::Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            cv::circle( frame, eye_center, radius, cv::Scalar( 255, 0, 0 ), 4, 8, 0 );
        }*/
  //  }//  END OF DETECTION

         msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
         pub.publish(msg);
         cv::waitKey(1);
       }

       ros::spinOnce();
       loop_rate.sleep();
     }
   }

