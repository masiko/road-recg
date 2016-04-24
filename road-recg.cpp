#include "road-recg.hpp"

roadRecg::roadRecg(int row, int col) {
	width = col;
	height = row;
	gray = cv::Mat::zeros(row,col,CV_8UC1);
	edge = cv::Mat::zeros(row,col,CV_8UC1);
	result = cv::Mat::zeros(row,col,CV_8UC3);
}

int roadRecg::setTbl(int w, int h) {
	int result_bin=0;
	int result_count=0;

	for (int i=0; i<16; i++)	histogram[i] = 0;

	if (w>=width || h>=height)	return -1;

	for (int i=height-h; i<height; i++) {
		for (int j=width/2-w/2; j<width/2+w/2; j++) {
			histogram[int(gray.data[j+i*width]/16)] += 1;
		}
	}
	for (int i=0; i<16; i++) {
		if (histogram[i] >= result_count) {
			result_bin = i;
			result_count = histogram[i];
		}
	}
	return result_bin;
}


int roadRecg::mainloop(cv::Mat in, cv::Mat out) {
	int edge_size = 0;
	cv::cvtColor(in,gray,CV_RGB2GRAY);
	result = cv::Mat::zeros(height,width,CV_8UC3);

	detectRoadEdge(10);
	edge_size = detectRateOfChange(100);

	for (int r=0; r<height; r++) {
		for (int c=0; c<width; c++) {
			out.data[c + width*r] = edge.data[c + width*r];
		}
	}

/*	if (edge_size>20)	cv::HoughLinesP( edge, lines, 3, CV_PI/45, 5, 10, 10);
	for (int c=0; c<lines.size(); c++) {
		cv::line(result, cv::Point(lines[c][0], lines[c][1]),
					cv::Point(lines[c][2], lines[c][3]), cv::Scalar(0,255,0), 3, 8);
	}
*/
	cv::imshow("result", result);

	return 0;
}

int roadRecg::detectRateOfChange(int thre) {
	const int MASK = 10;
	const int MASK_HALF = 5;
	int first=0, end=0;
	int diff = 0;
	int edge_size = 0;
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
					edge_size++;
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
//	if (edge_size>20)	cv::HoughLinesP( edge, lines, 3, CV_PI/45, 5, 10, 10);
	return edge_size;
}

int roadRecg::detectRoadEdge(int num) {
	const int MASK = 5;
	int interval = height/(2*num);
	int x=width/2;
	int y;
	int col_hist = setTbl(20, 40);
	
	std::cout<<col_hist<<"\n";

	for (int i=0; i<num; i++) {
		y = width*(i*interval+height/2);
		for (int j=MASK; j<width/2; j+=MASK) {
			 std::cout<<(gray.data[x+j + y]/16)<<",";
			 if ((gray.data[x+j + y]/16)==col_hist){
				 result.data[3*(x+j-MASK + y)] = 255;
				 result.data[3*(x+j-MASK + y)+1] = 255;
				 result.data[3*(x+j-MASK + y)+2] = 255;
			 }
		}
		std::cout<<"\n";
	}
	return 0;
}
