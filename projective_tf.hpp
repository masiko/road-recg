#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class projective_tf {
private:
	int width;
	int height;
	int centerx;
	int centery;
	int startx;
	int starty;
	int endx;
	int endy;

	double a, b;	//x axis scaling pra: f(y) = a*y + b
	double c, d;
	double rm[9] = {1.0, .0, .0,
					.0, 1.0, .0,
					.0, .0, 1.0};
					//rotation matrix
	double sm[9] = {1.0, .0, .0,
					.0, 1.0, .0,
					.0, .0, 1.0};
					//scaling matrix

	double tilt;	//[rage]
	double FOV;		//Filed Of View [rage]

public:
	cv::Mat in;
	cv::Mat map;
	cv::Mat out;

	projective_tf();
	~projective_tf(){};

	int setMatPra(int w, int h);
	int setRM(double);
	int setFOV(double);
	int culTf();
	int culMt(double* m, double* vi, double* vo);
	int culFOV(double);
	int setOutput(double* vi, double* vo);
	int interNearest();
	int interNearestX();
	int interNearestY();
	int interNearestAverage();
};

