#include "cardDetect.h"

const int CORNER_WIDTH = 35;
const int CORNER_HEIGHT = 85;

const string SUIT[] = { "H", "D", "C", "S" };
const string NUMBER[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K" };


Card::Card(Mat img) {
	img_src = img;
	width = img.cols;
	height = img.rows;
}

bool cmp_contour(const vector<Point> cnt1, const vector<Point> cnt2)
{
	return contourArea(cnt1) > contourArea(cnt2);
}

bool Card::preprocess()
{
	Mat img_tf;

	/* resize card */
	resize(this->img_src, img_tf, Size(200, 300));
	cvtColor(img_tf, img_tf, COLOR_BGR2GRAY);
	threshold(img_tf, img_tf, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	/* calculate corner  */
	img_tf = img_tf(Range(0, CORNER_HEIGHT), Range(0, CORNER_WIDTH));
	
	/* print corner of card  */
	//imshow("Corner", img_tf);
	//waitKey(0);

	vector<vector<Point> > contours;
	Rect rect;

	/* detect number */
	Mat img_num = img_tf(Range(0, int(CORNER_HEIGHT / 2) + 10), Range(0, CORNER_WIDTH));

	findContours(img_num, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	
	if (!contours.size()) {
		cout << contours.size() << endl;
		return false;
	}
	
	sort(contours.begin(), contours.end(), cmp_contour);
	rect = boundingRect(contours[0]);
	//resize(img_num(rect), this->img_num, Size(70, 125));
	this->img_num = img_num(rect);

	/* detect suit */
	Mat img_suit = img_tf(Range(int(CORNER_HEIGHT / 2), CORNER_HEIGHT), Range(0, CORNER_WIDTH));

	findContours(img_suit, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	if (!contours.size()) {
		cout << contours.size() << endl;
		return false;
	}
	
	sort(contours.begin(), contours.end(), cmp_contour);
	rect = boundingRect(contours[0]);
	resize(img_suit(rect), this->img_suit, Size(70, 100));

	return true;
}


int composite(Mat train, Mat query)
{
	int cnt = 0;

	resize(query, query, Size(train.cols, train.rows), train.cols / query.cols, train.rows / query.rows);

	for (int i = 0; i < train.rows; i++) {
		for (int j = 0; j < train.cols; j++) {
			if (train.at<uchar>(i, j) != query.at<uchar>(i, j))
				cnt++;

			if (train.at<uchar>(i, j) == (uchar)255 && query.at<uchar>(i, j) == (uchar)255)
				train.at<uchar>(i, j) = (uchar)0;
		}
	}

	return cnt;
}


string Card::match_number()
{
	int min = INT_MAX;

	for (int i = 0; i < sizeof(::NUMBER) / sizeof(::NUMBER[0]); i++) {

		Mat img_train_num = imread("./number_image/" + NUMBER[i] + ".jpg");
		if (img_train_num.empty()) {
			printf("(number) train image empty");
			exit(-1);
		}
		cvtColor(img_train_num, img_train_num, COLOR_BGR2GRAY);
		threshold(img_train_num, img_train_num, 125, 255, THRESH_BINARY | THRESH_OTSU);

		int unmatched = composite(img_train_num, this->img_num);
		if (min > unmatched) {
			min = unmatched;
			this->num = NUMBER[i];
		}
	}
	
	return this->num;
}


string Card::match_suit()
{
	int min = INT_MAX;

	for (int i = 0; i < sizeof(::SUIT) / sizeof(::SUIT[0]); i++) {
		
		Mat img_train_suit = imread("./suit_image/" + SUIT[i] + ".jpg");
		if (img_train_suit.empty()) {
			printf("(suit) train image empty");
			exit(-1);
		}
		cvtColor(img_train_suit, img_train_suit, COLOR_BGR2GRAY);
		threshold(img_train_suit, img_train_suit, 125, 255, THRESH_BINARY | THRESH_OTSU);

		int unmatched = composite(img_train_suit, this->img_suit);
		if (min > unmatched) {
			min = unmatched;
			this->suit = SUIT[i];
		}
	}

	return this->suit;
}
