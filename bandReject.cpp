#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

void ShiftDFT(Mat& fImage)
{
	Mat tmp, q0, q1, q2, q3;
	// first crop the image, if it has an odd number of rows or columns

	fImage = fImage(Rect(0, 0, fImage.cols & -2, fImage.rows & -2));

	int cx = fImage.cols / 2;
	int cy = fImage.rows / 2;

	// rearrange the quadrants of Fourier image
	// so that the origin is at the image center
	q0 = fImage(Rect(0, 0, cx, cy));
	q1 = fImage(Rect(cx, 0, cx, cy));
	q2 = fImage(Rect(0, cy, cx, cy));
	q3 = fImage(Rect(cx, cy, cx, cy));

	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

void MinMaxNormalization(Mat input, Mat output)
{
	double minVal, maxVal;
	minMaxLoc(input, &minVal, &maxVal); //find minimum and maximum intensities
	input.convertTo(output, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
}

void DisplayDftResult(Mat Real, Mat Imaginary, String WindowName)
{
	Mat Magnitude = Mat(Real.rows, Real.cols, CV_32F);

	magnitude(Real, Imaginary, Magnitude);

	// log scaling.
	Magnitude += Scalar::all(1.);
	log(Magnitude, Magnitude);

	// make the origin become the center.
	ShiftDFT(Magnitude);

	// normalize the magnitude from CV_32F to CV_8U.
	Mat NormalizedResult = Mat(Real.rows, Real.cols, CV_8U);
	MinMaxNormalization(Magnitude, NormalizedResult);

	// display the normalized image.
	imshow(WindowName, NormalizedResult);
}

void Filtering(Mat Real, Mat Imaginary, int r)
{
	int height = Real.rows, width = Real.cols;

	// define the filter.
	Mat filter = Mat(height, width, CV_32F);
	int cy = height / 2;
	int cx = width / 2;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			if ((y - cy) * (y - cy) + (x - cx) * (x - cx) < r + 200 * r + 200 && (y - cy) * (y - cy) + (x - cx) * (x - cx) > r + 120 * r + 120)
				filter.at<float>(y, x) = 0.; //Èò»ö
			else
				filter.at<float>(y, x) = 1.; //°ËÀº»ö
		}

	// display the defined filter.
	imshow("filter", filter);
	ShiftDFT(filter);

	// filtering in frequency domain, that is multiplication.
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			Real.at<float>(y, x) *= filter.at<float>(y, x);
			Imaginary.at<float>(y, x) *= filter.at<float>(y, x);
		}
}

int main()
{
	Mat input = imread("pattern1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	int height = input.rows, width = input.cols;

	// convert the input image to float type image
	Mat fimg;
	input.convertTo(fimg, CV_32F);

	// DFT
	Mat frequency;
	dft(fimg, frequency, DFT_COMPLEX_OUTPUT);

	// split the frequency images into real and imaginary parts.
	Mat channels[2];
	split(frequency, channels);	// channels[0] = real, channels[1] = imaginary

								// display the DFT magnitude.
	DisplayDftResult(channels[0], channels[1], "Frequency");

	// filtering.
	Filtering(channels[0], channels[1], 50);

	// merge the real and imaginary parts again.
	merge(channels, 2, frequency);

	// inverse DFT (IDFT)
	Mat spatial;
	dft(frequency, spatial, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);

	// display the IDFT image.
	Mat reconstruction;
	spatial.convertTo(reconstruction, CV_8U);

	// display the DFT magnitude of filtering result.
	DisplayDftResult(channels[0], channels[1], "Frequency (filtered)");

	imshow("original", input);
	imshow("result", reconstruction);
	waitKey();
	destroyAllWindows();
	return 0;
}

pattern1 - band reject






