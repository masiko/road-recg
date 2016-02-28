#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "road-recg.hpp"
// tmp of image processing program

int main() {
	cv::Mat in, out;

	cv::VideoCapture cap(1);
	cv::namedWindow("cap");
	cv::namedWindow("out");

	if (!cap.isOpened())	return -1;
	std::cout << "start cap channel 1\n";

	cap >> in;

	//cv::waitKey(1000);
	out.create(in.rows, in.cols, CV_8UC1);
	roadRecg RoadRecg(in.rows, in.cols);
	while(1) {
		cap >> in;
		//write main loop

		RoadRecg.mainloop(in, out);

		cv::imshow("cap",in);
		cv::imshow("out",out);
		if (cv::waitKey(100)!=-1)	break;
	}

	return 0;
}

