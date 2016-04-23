#include "road-recg.hpp"

roadRecg::roadRecg(int row, int col) {
	width = col;
	height = row;
	gray = cv::Mat::zeros(row,col,CV_8UC1);
	edge = cv::Mat::zeros(row,col,CV_8UC1);
	result = cv::Mat::zeros(row,col,CV_8UC3);
}

int roadRecg::mainloop(cv::Mat in, cv::Mat out) {

//	int row, col;
	cv::cvtColor(in,gray,CV_RGB2GRAY);
	result = cv::Mat::zeros(height,width,CV_8UC3);

	detectRateOfChange(100);

	for (int r=0; r<height; r++) {
		for (int c=0; c<width; c++) {
			out.data[c + width*r] = edge.data[c + width*r];
		}
	}

	for (int c=0; c<lines.size(); c++) {
		cv::line(result, cv::Point(lines[c][0], lines[c][1]),
					cv::Point(lines[c][2], lines[c][3]), cv::Scalar(0,255,0), 3, 8);
	}

	cv::imshow("result", result);

	return 0;
}

int roadRecg::detectRateOfChange(int thre) {
	const int MASK = 10;
	const int MASK_HALF = 5;
	int first=0, end=0;
	int diff = 0;
	int edge_count = 0;
	char edge_flag = 1;
/*
	for (int e=0; e<MASK_HALF-1; e++) {
		first += abs(gray.data[e]-gray.data[e+1]);
	}
*/
	for (int c=0; c<height; c++) {
		edge_flag = 1;
		for (int e=0; e<MASK_HALF-1; e++) {
			first += abs(gray.data[e + c*width] - gray.data[e+1 + c*width]);
		}

		for (int d=MASK_HALF; d<width-MASK_HALF; d+=MASK_HALF) {
			
			for (int f=0; f<MASK_HALF-1; f++) {
				 end += abs(gray.data[d+f + c*width] - gray.data[d+f+1 + c*width]);
			}

			diff = abs(first - end);
			if (diff>thre) {
				if (edge_flag) {
					edge.data[d + c*width] = 255; 
					result.data[3*d + 3*c*width+2] = 255;
					edge_count++;
					edge_flag = 0;
				} else {
					edge.data[d + c*width] = 0; 
					result.data[3*d + 3*c*width+2] = 0;
					edge_flag = 1;
				}
			} else {
				edge_flag = 0;
				edge.data[d + c*width] = 0; 
				result.data[3*d + 3*c*width+2] = 0;
			}

			first = end;
			end = 0;
		}
		first = 0;
		end = 0;
	}
	if (edge_count>20)	cv::HoughLinesP( edge, lines, 3, CV_PI/45, 5, 30, 10);
	return 0;
}

