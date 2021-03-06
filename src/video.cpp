#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/video/video.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <cv_bridge/cv_bridge.h>
#include <sstream>		// for converting the command line parameter to integer
#include <sensor_msgs/image_encodings.h>
cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
cv::String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;
cv::String window_name = "Capture - Face detection";

int main(int argc, char **argv)
{

	// Check if video source has been passed as a parameter
	if (argv[1] == NULL)
		return 1;

	ros::init(argc, argv, "image_publisher");
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	image_transport::Publisher pub = it.advertise("camera/image", 1);

	// Convert the passed as command line parameter index for the video device to an integer
	std::istringstream video_sourceCmd(argv[1]);
	int video_source;
	// Check if it is indeed a number
	if (!(video_sourceCmd >> video_source)) {
		printf("\n failed in >>");
		return 1;
	}

	cv::VideoCapture cap(video_source);
	// Check if video device can be opened with the given index
	if (!cap.isOpened())
		return 1;
	cv::Mat frame;
	sensor_msgs::ImagePtr msg;

	//for frame rate
	double total_time = 0.0;
	int c, cpt = 30;
	double t = (double)cv::getTickCount();

	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cv::VideoWriter video("out.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10,
			      cv::Size(frame_width, frame_height), true);
	//ros::Rate loop_rate(5);
	while (nh.ok()) {
		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		total_time += t;
		cpt++;
		t = (double)cv::getTickCount();
		cap >> frame;
		cv::putText(frame,
			    cv::format("Average FPS=%d",
				       cvRound(cpt / total_time)), cv::Point(30,
									     30),
			    cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0,
								      255));
		video.write(frame);
		// Check if grabbed frame is actually full with some content
		if (!frame.empty()) {

			msg =
			    cv_bridge::CvImage(std_msgs::Header(), "bgr8",
					       frame).toImageMsg();
			pub.publish(msg);
			cv::waitKey(1);
		}

		ros::spinOnce();
		//loop_rate.sleep();
	}
}
