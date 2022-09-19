#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;


int main()
{
	Mat input = imread("gaussian1.jpg");
	int height = input.rows;
	int width = input.cols;
	Mat output = Mat::zeros(height, width, CV_8U);
	Mat output2 = Mat::zeros(height, width, CV_8U);

	GaussianBlur(input, output, Size(5, 5), 2.0);

	imshow("original", input);
	imshow("Gaussian", output);

	waitKey();
	destroyAllWindows();
	return 0;
}