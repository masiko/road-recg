#include<math.h>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

class roadRecg {
	private:
		int width;
		int height;
		cv::Mat input;		//BGR input image
		cv::Mat gray;		//gray scale input image
		cv::Mat map;
		cv::Mat edge;		//edge detection image
		cv::Mat result;		//output image, CV_8U3C
		int histogram[512];	//(2^3)^3
//		char color_tbl[765];	//color = b + 255*g + 2*255*r
		cv::vector<cv::Vec4i> lines;
	public:

	roadRecg(int r, int c);
	int setHist(int w, int h);
	int setColMap();
	int mainloop(cv::Mat in, cv::Mat out);
	int detectRateOfChange(int thre);
	int detectRoadEdge(int num);
};

