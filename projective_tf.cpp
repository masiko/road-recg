#include "projective_tf.hpp"

projective_tf::projective_tf() {
	width =
	height =
	centerx = 
	centery = -1;

	tilt = .0;//4*M_PI/10;	//0 < tilt < M_PI/2
	FOV = 1.0;
}

int projective_tf::setMatPra(int w, int h) {
	width = w;
	height = h;
	centerx = w/2;
	centery = h/2;
	startx = w/6;
	starty = h/6;
	endx = 5*w/6;
	endy = 5*h/6;

	in.create(height, width, CV_8UC3);
	map = cv::Mat::zeros(height, width, CV_8UC1);
	out.create(height, width, CV_8UC3);
	
	a = 
	b = 
	c = 
	d = .0;

	rm[4] = 1/cos(tilt);
	sm[5] = (1-rm[4])*centery;
	return 0;
}

int projective_tf::setRM(double ang) {
	tilt = ang;
	rm[4] = 1/cos(tilt);
	sm[5] = (1-rm[4])*centery;
	return 0;
}

int projective_tf::setFOV(double l) {
	a = (1.0 - l) / (endy - starty);
	b = -a*starty + l;
	return 0;
}

int projective_tf::culTf() { 
	double v0[3] = {.0, .0, 1.0};
	double v1[3] = {.0 ,.0, 1.0};
	double v2[3] = {.0, .0, 1.0};
	map = cv::Mat::zeros(height, width, CV_8UC1);
	for (v0[1]=starty; v0[1]<endy; v0[1]++) {
		culFOV(v0[1]);
		for (v0[0]=startx; v0[0]<endx; v0[0]++) {
			culMt(rm, v0, v1);
			culMt(sm, v1, v2);
			setOutput(v0, v2);
		}
	}
	interNearestAverage();
	return 0;
}

int projective_tf::culMt(double* m, double* vi, double* vo) {
	vo[0] = m[0]*vi[0] + m[1]*vi[1] + m[2]*vi[2];
	vo[1] = m[3]*vi[0] + m[4]*vi[1] + m[5]*vi[2];
	vo[2] = m[6]*vi[0] + m[7]*vi[1] + m[8]*vi[2];
	return 0;
}

int projective_tf::culFOV(double h) {
	sm[0] = a*h + b;
	sm[2] = -sm[0]*centerx + centerx;
	return 0;
}

int projective_tf::setOutput(double* vi, double* vo) {
	if ( vo[0] < 0 || width <= vo[0] || vo[1] < 0 || height <= vo[1] )	return 1;
	out.data[3*((int)vo[0] + width*(int)vo[1])] = in.data[3*((int)vi[0] + width*(int)vi[1])];
	out.data[3*((int)vo[0] + width*(int)vo[1]) + 1] = in.data[3*((int)vi[0] + width*(int)vi[1]) + 1];
	out.data[3*((int)vo[0] + width*(int)vo[1]) + 2] = in.data[3*((int)vi[0] + width*(int)vi[1]) + 2];
	map.data[(int)vo[0] + width*(int)vo[1]] = 1;
	return 0;
}

int projective_tf::interNearest() {
	interNearestX();
	interNearestY();
	return 0;
}
int projective_tf::interNearestX() {
	int p = 0;
	int y = 0;
	for (int i=0; i<height; i++) {
		y = width*i;
		for (int j=0; j<width; j++) {
			p = j + y;
			if (map.data[p] == 1)  continue;
			for (int k=1; k<8; k++) {		//-8 ~ +8
				if (j-k>=0) {
					if (map.data[p-k]==1) {
						out.data[3*p] = out.data[3*(p-k)];
						out.data[3*p+1] = out.data[3*(p-k)+1];
						out.data[3*p+2] = out.data[3*(p-k)+2];
						map.data[p] = 2;
						continue;
					}
				}
				if (j+k<width) {
					if (map.data[p+k]==1) {
						out.data[3*p] = out.data[3*(p+k)];
						out.data[3*p+1] = out.data[3*(p+k)+1];
						out.data[3*p+2] = out.data[3*(p+k)+2];
						map.data[p] = 2;
						continue;
					}
				}
			}
		}
	}
	return 0;
}

int projective_tf::interNearestY() {
	int p = 0;
	int y = 0;
	for (int i=0; i<height; i++) {
		y = width*i;
		for (int j=0; j<width; j++) {
			p = j + y;
			if (map.data[p]==1 || map.data[p]==2) continue;
			for (int k=1; k<8; k++) {		//-8 ~ +8
				if (i-k>=0) {
					if (map.data[p-width*k]==1 || map.data[p-width*k]==2) {
						out.data[3*p] = out.data[3*(p-width*k)];
						out.data[3*p+1] = out.data[3*(p-width*k)+1];
						out.data[3*p+2] = out.data[3*(p-width*k)+2];
						map.data[p] = 3;
						continue;
					}
				}
				if (i+k<height) {
					if (map.data[p+width*k]==1 || map.data[p+width*k]==2) {
						out.data[3*p] = out.data[3*(p+width*k)];
						out.data[3*p+1] = out.data[3*(p+width*k)+1];
						out.data[3*p+2] = out.data[3*(p+width*k)+2];
						map.data[p] = 3;
						continue;
					}
				}
			}
		}
	}
	return 0;
}

int projective_tf::interNearestAverage() {
	static int bsize = 3;
	int p = 0;
	int y = 0;
	int bp = 0;
	int count = 0;
	int ave[3] = {0, 0, 0};
	int WM[49] = {1, 1, 1, 1, 1, 1, 1,
				  1, 1, 1, 2, 1, 1, 1,
				  1, 1, 2, 3, 2, 1, 1,
				  1, 2, 3, 0, 3, 2, 1,
				  1, 1, 2, 3, 2, 1, 1,
				  1, 1, 1, 2, 1, 1, 1,
				  1, 1, 1, 1, 1, 1, 1};
	
	for (int i=0; i<height; i++) {
		y = width*i;
		for (int j=0; j<width; j++) {
			p = j + y;
			if (map.data[p]==1) continue;
			ave[0] = 0;
			ave[1] = 0;
			ave[2] = 0;
			count=0;
			for (int k=-bsize; k<=bsize; k++) {		//-3 ~ +3
				if (i+k<0 || height<=i+k)	continue;
				for (int l=-bsize; l<=bsize; l++) {
					if (l+j<0 || width<=l+j)	continue;
					bp = p+width*k+l;
					if (map.data[bp]==1) {
						ave[0] += WM[bsize+k + (2*bsize+1)*(bsize+l)]*out.data[3*bp];
						ave[1] += WM[bsize+k + (2*bsize+1)*(bsize+l)]*out.data[3*bp+1];
						ave[2] += WM[bsize+k + (2*bsize+1)*(bsize+l)]*out.data[3*bp+2];
						count += WM[bsize+k + (2*bsize+1)*(bsize+l)];
					}
				}
			}
			if (!count) continue;
			out.data[3*p] = ave[0]/count;
			out.data[3*p+1] = ave[1]/count;
			out.data[3*p+2] = ave[2]/count;
			map.data[p] = 2;
		}
	}
	return 0;
}

