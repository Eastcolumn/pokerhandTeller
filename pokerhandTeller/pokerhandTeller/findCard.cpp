#include "cardFind.h"

bool compare_approx_x(Point2f a, Point2f b) {
	return a.x < b.x;
}
bool compare_approx_y(Point2f a, Point2f b) {
	return a.y < b.y;
}

vector<Mat> find_cards(Mat &img) {
	vector<vector<Point> > contours;
	vector<Point2f> approx;

	Mat img_gray;
	Mat img_wb;
	Mat detected_edges;
	Mat cards[7];

	int lowThreshold = 50;
	int highThreshold = 150;


	/*gray scale image*/
	cvtColor(*img, img_gray, COLOR_BGR2GRAY);
	blur(img_gray, img_gray, Size(3, 3));
	/*binary image*/
	threshold(img_gray, img_wb, 125, 255, THRESH_BINARY_INV | THRESH_OTSU);
	imshow("img_wb", img_wb);
	waitKey(0);
	/*detect edges*/
	Canny(img_wb, detected_edges, lowThreshold, highThreshold, 3);
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

		/* 세로변이 가장 긴 형태일 경우*/
		if ((max_index[0] == 0 && max_index[1] == 1) ||
			(max_index[0] == 2 && max_index[1] == 3)) {
			max_width = max / 88 * 62;
		}
		/* 가로변이 가장 긴 형태일 경우*/
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

	///* 테스트 출력*/
	//for (int i = 0; i < contours.size(); i++) {
	//	cout << i << " image" << endl;
	//	imshow("Result", cards[i]);
	//	waitKey(0);
	//}

	return cards;
}