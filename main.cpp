#include <iostream>
#include "projective_tf.hpp"
#include "DetectObject.hpp"

#define CAP_MODE 1

int main() {
	projective_tf pt;
	int x, y, t;
	int x0, y0, t0;
#ifdef CAP_MODE
	cv::VideoCapture cap(1);
#else
	cv::VideoCapture cap("video2.mp4");
#endif	
	cv::namedWindow("cap");
	cv::namedWindow("dst");

	if (!cap.isOpened())	return -1;
	std::cout << "start cap channel 1\n";
	
	pt.setMatPra(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	pt.setFOV(2.0);
	pt.setRM(9*M_PI/20);

	while(1) {
		cap >> pt.in;

		//write main loop
		pt.culTf();

		cv::imshow("cap",pt.in);
		cv::imshow("dst",pt.out);
		if (cv::waitKey(100)!=-1)	break;
	}

	return 0;
}

