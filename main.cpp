#include<core.hpp>
#include<imgproc.hpp>
#include<highgui.hpp>
#include<iostream>
#include<cstdio>
#include<math.h>

using namespace cv;
using namespace std;

enum point_pos {
	UP_LEFT,
	UP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};
//opencv ��� �Լ�
//ī�� ũ�� 62*88
/*
arcLength()�Լ��� contours �ѷ��� ���̸� ����ϴ� �Լ��̴�. 
parameter1: �ѷ��� ����� contour�̴�.
parameter2: contour�� ������ ������ ���θ� Ÿ������. True�� ���� �ǹ��Ѵ�. 
0.1: �ѷ��� ���̿� 10%�� ��Ÿ����. 

approxPolyDP()�Լ��� �ٰ����� ������� �������� ���� �ٿ������� �Լ��̴�. 
epsilon(����)��ŭ�� �ִ������� �ؼ� �������� �ٿ�������. 
epsilon���� �������� ������ ����� ����� ����ǰ� 
epsilon(����)���� ũ�� Ŭ���� �������� ������ ���� �� �پ���. 
epsilon�� �ʹ� ũ�� �������� ����ؼ� �پ���� �Ǵϱ� �ᱹ �������� ������ 0�� ������ ����� ���� �� �ִ�. 
parameter1���� �������� ������ ���� contour�� �ְ� 
parameter2�� �ٿ����� ���� epsilon�� �ְ� 
parameter3�� ������ ������ ���θ� �ִ´�.
*/

void find_edge(Mat* img) {
	vector<vector<Point> > contours;
	vector<Point2f> approx;

	Mat img_gray;
	Mat detected_edges;
	Mat img_result = (*img).clone();

	int lowThreshold = 50;
	int highThreshold = 150;

	cvtColor(*img, img_gray, COLOR_BGR2GRAY);
	blur(img_gray, detected_edges, Size(3, 3));
	Canny(detected_edges, detected_edges, lowThreshold, highThreshold, 3);
	findContours(detected_edges, contours, RETR_LIST, CHAIN_APPROX_SIMPLE, Point());

	for (size_t i = 0; i < contours.size(); i++){
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);
	}

	float length, max_width, max =0;
	for (int i = 0; i < 4; i++) {
		length = sqrt(pow(approx[i%4].x - approx[(i+1)%4].x, 2) + pow(approx[i % 4].y - approx[(i + 1) % 4].y, 2));
		if (length > max)
			max = length;
	}

	max_width = max / 88 * 62;
	vector<Point2f> vertex{ 
		Point2f(0, 0), Point2f(0, max), 
		Point2f(max_width, max), Point2f(max_width , 0) 
	};
	Mat affine = getPerspectiveTransform(approx, vertex);
	warpPerspective(*img, *img, affine, Size(max_width,max));
	resize(*img, *img, Size(310, 440), 310 / max_width, 440 / max);
}


int main() {
	Mat img;

	int width = img.cols;
	int height = img.rows;
	
	img = imread("C:/input/card_5.jpg", IMREAD_COLOR);
	if (img.empty()) {
		cout << "�̹����� ã�� �� ����" << endl;
		return 1;
	}

	find_edge(&img);
	imshow("Result", img);

	waitKey(0);
	return 0;
}
