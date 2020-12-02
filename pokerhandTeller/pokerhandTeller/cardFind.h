#pragma once

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

#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include<cstdio>
#include<math.h>
#include<algorithm>

using namespace cv;
using namespace std;

bool compare_approx_x(Point2f a, Point2f b);
bool compare_approx_y(Point2f a, Point2f b);
vector<Mat> find_cards(Mat& img);