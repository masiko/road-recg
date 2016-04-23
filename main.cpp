#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "road-recg.hpp"

//#define VIDEO_MODE
// tmp of image processing program

int main() {
	cv::Mat in, out;

	#ifdef VIDEO_MODE
	//Video Test Mode
	cv::VideoCapture cap(1);
	cv::namedWindow("cap");
	cv::namedWindow("out");
	if (!cap.isOpened())	return -1;
	std::cout << "start cap channel 1\n";
	cap >> in;

	#else
	//Image Test Mode
	in = cv::imread("test_img.jpg");
	if (in.empty())	return -1;
	
	#endif

	out.create(in.rows, in.cols, CV_8UC1);
	roadRecg RoadRecg(in.rows, in.cols);
	
	#ifdef VIDEO_MODE
	//Video Test Mode	
	while(1) {
		cap >> in;
		//write main loop
		RoadRecg.mainloop(in, out);
		cv::imshow("cap",in);
		cv::imshow("out",out);
		if (cv::waitKey(100)!=-1)	break;
	}
	
	#else
	//Image Test Mode
	RoadRecg.mainloop(in, out);
	cv::imshow("cap",in);
	cv::imshow("out",out);
	while(1)	if (cv::waitKey(100)!=-1)	break;

	#endif

	return 0;
}

