#include "road-recg.hpp"

roadRecg::roadRecg(int row, int col) {
	width = col;
	height = row;
	map = cv::Mat::zeros(row,col,CV_8UC3);
	gray = cv::Mat::zeros(row,col,CV_8UC1);
	edge = cv::Mat::zeros(row,col,CV_8UC1);
	result = cv::Mat::zeros(row,col,CV_8UC3);
}

int roadRecg::setColMap() {
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			map.data[3*(j+i*width)] = int(input.data[3*(j+i*width)]/32);
			map.data[3*(j+i*width)+1] = int(input.data[3*(j+i*width)+1]/32);
			map.data[3*(j+i*width)+2] = int(input.data[3*(j+i*width)+2]/32);
		}
	}
	return 0;
}

int roadRecg::setHist(int w, int h, int top3[3]) {
	int result_count=0;
	int b,g,r;

	for (int i=0; i<512; i++)	histogram[i] = 0;

	if (w<0 || h<0 || w>=width || h>=height)	return -1;

	for (int i=height-h; i<height; i++) {
		for (int j=width/2-w/2; j<width/2+w/2; j++) {
			b = map.data[3*(j+i*width)];
			g = map.data[3*(j+i*width)+1];
			r = map.data[3*(j+i*width)+2];
			histogram[b + 8*g + 64*r] += 1;
		}
	}
	for (int i=0; i<512; i++) {
		if (histogram[i] >= result_count) {
			top3[2] = top3[1];
			top3[1] = top3[0];
			top3[0] = i;
			result_count = histogram[i];
		}
	}
	return result_count;
}

int roadRecg::mainloop(cv::Mat in, cv::Mat out) {
	int edge_size = 0;
	in.copyTo(input);
	setColMap();
	cv::cvtColor(in,gray,CV_RGB2GRAY);
	result = cv::Mat::zeros(height,width,CV_8UC3);

	detectRoadEdge(10);
//	edge_size = detectRateOfChange(100);

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

int roadRecg::detectRoadEdge(int num) {
	int top3[3] = {-1,-1,-1};
	int y;
	setHist(40, 120, top3);

	std::cout<<top3[0]<<","<<top3[1]<<","<<top3[2]<<"\n";


	for (int i=0; i<height; i++) {
		y = i*width;
		for (int j=0; j<width; j++) {
			result.data[3*(j + y)] = map.data[3*(j + y)]*32;
			result.data[3*(j + y)+1] = map.data[3*(j + y)+1]*32;
			result.data[3*(j + y)+2] = map.data[3*(j + y)+2]*32;
		}
	}
	
	return 0;
}
