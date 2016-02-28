#include<math.h>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

class roadRecg {
	private:
		int width;
		int height;
		cv::Mat gray;
		char color_tbl[765];	//color = b + 255*g + 2*255*r
	public:

	roadRecg(int r, int c);
	int setTbl(char map[765]);
	int mainloop(cv::Mat in, cv::Mat out);
	int rowRateOfChange(int thre);
};

