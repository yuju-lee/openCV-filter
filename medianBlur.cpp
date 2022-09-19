#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;


int main()
{
	Mat input = imread("salt&pepper1.jpg");
	int height = input.rows, width = input.cols;

	Mat output = Mat::zeros(height, width, CV_8U);
	Mat output2 = Mat::zeros(height, width, CV_8U);

	medianBlur(input, output, 3);

	imshow("original", input);
	imshow("median", output);

	waitKey();
	destroyAllWindows();
	return 0;
}
