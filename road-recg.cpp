#include "road-recg.hpp"

roadRecg::roadRecg(int row, int col) {
	width = col;
	height = row;
	gray = cv::Mat::zeros(row,col,CV_8UC1);
}

int roadRecg::mainloop(cv::Mat in, cv::Mat out) {

//	int row, col;
	cv::cvtColor(in,gray,CV_RGB2GRAY);

	rowRateOfChange(100);

	for (int r=0; r<height; r++) {
		for (int c=0; c<width; c++) {
			out.data[c + width*r] = gray.data[c + width*r];
		}
	}

	return 0;
}

int roadRecg::rowRateOfChange(int thre) {
	const int MASK = 10;
	const int MASK_HALF = 5;
	int first=0, end=0;
	int diff = 0;

	for (int e=0; e<MASK_HALF-1; e++) {
		first += abs(gray.data[e]-gray.data[e+1]);
	}

	for (int c=0; c<height; c++) {
		for (int e=0; e<MASK_HALF-1; e++) {
			first += abs(gray.data[e + c*width] - gray.data[e+1 + c*width]);
		}

		for (int d=MASK_HALF; d<width-MASK_HALF; d+=MASK_HALF) {
			
			for (int f=0; f<MASK_HALF-1; f++) {
				 end += abs(gray.data[d+f + c*width] - gray.data[d+f+1 + c*width]);
			}

			diff = abs(first - end);
			if (diff>thre) 	gray.data[d + c*width] = 255;

			first = end;
			end = 0;
		}
		first = 0;
		end = 0;
	}		
	return 0;
}

