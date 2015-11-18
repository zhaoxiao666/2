#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat input_image;
Mat output_mask;
Mat output_image;

void main()
{
	VideoCapture cam(0);
	if (!cam.isOpened())
		return;

	namedWindow("input image");
	namedWindow("output mask");
	namedWindow("output image");
	/*椭圆皮肤模型*/
	Mat skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
	ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2), 43.0, 0.0, 360.0, Scalar(255, 255, 255), -1);

	while (true) {
		cam >> input_image;
		if (input_image.empty())
			return;

		Mat ycrcb_image;
		output_mask = Mat::zeros(input_image.size(), CV_8UC1);
		cvtColor(input_image, ycrcb_image, CV_BGR2YCrCb); //首先转换成到YCrCb空间
		for (int i = 0; i < input_image.cols; i++)   //利用椭圆皮肤模型进行皮肤检测
		for (int j = 0; j < input_image.rows; j++){
			Vec3b ycrcb = ycrcb_image.at<Vec3b>(j, i);
			if (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0)
				output_mask.at<uchar>(j, i) = 255;
		}
		input_image.copyTo(output_image, output_mask);

		imshow("input image", input_image);
		imshow("output mask", output_mask);
		imshow("output image", output_image);
		output_image.setTo(0);
		if (27 == waitKey(30))
			return;
	}
	return;
}