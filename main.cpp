#include<core.hpp>
#include<imgproc.hpp>
#include<highgui.hpp>
#include<iostream>
#include<cstdio>
#include<math.h>
#include<algorithm>

using namespace cv;
using namespace std;

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
bool compare_approx_x(Point2f a, Point2f b) {
	return a.x < b.x;
}
bool compare_approx_y(Point2f a, Point2f b) {
	return a.y < b.y;
}

void find_cards(Mat* img) {
	vector<vector<Point> > contours;
	vector<Point2f> approx;

	Mat img_gray;
	Mat detected_edges;
	Mat cards[9];

	int lowThreshold = 50;
	int highThreshold = 150;


	/*gray scale image*/
	cvtColor(*img, img_gray, COLOR_BGR2GRAY);
	/*detect edges*/
	blur(img_gray, detected_edges, Size(3, 3));
	Canny(detected_edges, detected_edges, lowThreshold, highThreshold, 3);
	/*transform edges into coordinates*/
	findContours(detected_edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());

	for (int i = 0; i < contours.size(); i++) {
		/*find vertex using contours*/
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

		///*sort order of vertex*/
		int max_index[2];
		float length, max_width, max = 0;
		Point2f temp(0, 0);
		sort(approx.begin(), approx.end(), compare_approx_y);
		sort(approx.begin(), approx.end(), compare_approx_x);
		temp = approx[2]; approx[2] = approx[3]; approx[3] = temp;

		/*find longest edge*/
		for (int i = 0; i < 4; i++) {
			length = sqrt(pow(approx[i % 4].x - approx[(i + 1) % 4].x, 2) + pow(approx[i % 4].y - approx[(i + 1) % 4].y, 2));
			if (length > max) {
				max = length;
				max_index[0] = i % 4;
				max_index[1] = (i + 1) % 4;
			}
		}

		/* ���κ��� ���� �� ������ ���*/
		if ((max_index[0] == 0 && max_index[1] == 1) ||
			(max_index[0] == 2 && max_index[1] == 3)) {
			max_width = max / 88 * 62;
		}
		/* ���κ��� ���� �� ������ ���*/
		else {
			max_width = max / 62 * 88;
		}
		
		vector<Point2f> vertex{
			Point2f(0, 0), Point2f(0, max),
			Point2f(max_width, max), Point2f(max_width , 0)
		};
		Mat affine = getPerspectiveTransform(approx, vertex);
		warpPerspective(*img, cards[i], affine, Size(max_width, max));
		resize(cards[i], cards[i], Size(62, 88), 62 / max_width, 88 / max);
	}

	/* �׽�Ʈ ���*/
	for (int i = 0; i < contours.size(); i++) {
		cout << i << " image" << endl;
  		imshow("Result", cards[i]);
		waitKey(0);
	}
}


int main() {
	Mat img;

	int width = img.cols;
	int height = img.rows;
	
	img = imread("C:/input/cards_soft.jpg", IMREAD_COLOR);
	if (img.empty()) {
		cout << "�̹����� ã�� �� ����" << endl;
		return 1;
	}

	find_cards(&img);

	return 0;
}
