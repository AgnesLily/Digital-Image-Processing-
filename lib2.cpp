#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "math.h"

using namespace cv;
using namespace std;

//得到某像素
uchar*  get_pixel(const Mat &img, int x, int y) {
	return (uchar*)img.data + x*img.step + y * 3;
}

uchar bilinear(uchar a, uchar b, uchar c, uchar d, float dx, float dy){
	float  h1 = a + dx*(b - a);// = (1-dx)*a + dx*b
	float  h2 = c + dx*(d - c);
	return h1 + dy*(h2 - h1);
}

void Scale(const Mat &input, Mat &output, double sx, double sy){
	Mat outputScale(input.rows*sy, input.cols*sx, CV_8UC3);
	int outHeight = input.rows*sy;
	
	int outWidth = input.cols*sx;
	int step = outputScale.step;
	int nc = outputScale.channels();
	uchar *outputImageData = get_pixel(outputScale, 0, 0);
	//cout << outputImageData;
	cout << outHeight << endl;
	for (int i = 0; i < outHeight; ++i, outputImageData += step)
	{
		cout << i<<endl;
		uchar *px = outputImageData;
		for (int j = 0; j < outWidth; ++j, px += nc)
		{
			int x = floor(i / sy);
			int y = floor(j / sx);

			float dx = i / sy - x;
			float dy = j / sx - y;

			//在原图中取到要进行插值的四个像素点
			uchar *pixela = get_pixel(input, x, y);
			uchar *pixelb = get_pixel(input, x, y + 1);
			uchar *pixelc = get_pixel(input, x + 1, y);
			uchar *pixeld = get_pixel(input, x + 1, y + 1);
			
			px[0] = bilinear(pixela[0], pixelb[0], pixelc[0], pixeld[0], dx, dy);
			px[1] = bilinear(pixela[1], pixelb[1], pixelc[1], pixeld[1], dx, dy);
			px[2] = bilinear(pixela[2], pixelb[2], pixelc[2], pixeld[2], dx, dy);		
		}
	}
	cout << outputScale.cols;
	cout << outputScale.rows;
	output = outputScale;
	imshow("showImage", output);
	waitKey();
}

void transform( Mat &originalImg) {
	
	int originWidth = originalImg.cols;
	int originHeight = originalImg.rows;
	int step = originalImg.step;
	int nc = originalImg.channels();
	Mat output(originHeight, originWidth, CV_8UC3);

	//遍历应该输出图像的像素
	uchar *row = get_pixel(output, 0, 0);
	for (int i = 0; i < originHeight; ++i, row += step)//行
	{
		uchar *px = row;
		for (int j = 0; j < originWidth; ++j, px += nc)//列
		{    
			//将输出图像的坐标归一化处理
			double xoc = (j - 0.5*output.cols) / (0.5*output.cols);
			double yoc = (i - 0.5*output.rows) / (0.5*output.rows);			
			double r = sqrt(xoc*xoc + yoc*yoc);
			double orix;
			double oriy;
			//cout << r << endl;
			if (r >= 1){
				orix = xoc;
				oriy = yoc;							
			}
			else {
				double radian = (1 - r)*(1 - r);
				orix = cos(radian)*xoc - sin(radian)*yoc;
				oriy = sin(radian)*xoc + cos(radian)*yoc;				
			}
			double realorix = 0.5*output.cols*orix + 0.5*output.cols;
			double realoriy = 0.5*output.rows*oriy + 0.5*output.rows;
			
			//得到变形后的元素在原图中的像素值
			int x = floor(realoriy);
			int y = floor(realorix);

			double dx = realoriy - x;
			double dy = realorix - y;

			//在原图中取到要进行插值的四个像素点
			uchar *pixela = get_pixel(originalImg, x, y);
			uchar *pixelb = get_pixel(originalImg, x, y + 1);
			uchar *pixelc = get_pixel(originalImg, x + 1, y);
			uchar *pixeld = get_pixel(originalImg, x + 1, y + 1);

			px[0] = bilinear(pixela[0], pixelb[0], pixelc[0], pixeld[0], dx, dy);
			px[1] = bilinear(pixela[1], pixelb[1], pixelc[1], pixeld[1], dx, dy);
			px[2] = bilinear(pixela[2], pixelb[2], pixelc[2], pixeld[2], dx, dy);
		}
	}
	imshow("showImage", output);
	waitKey();
}

void test3(){
	Mat originalImg = imread("b.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat output;
	Scale(originalImg,output,0.5, 0.5);
}

void test4() {
	Mat originalImg = imread("c.png", CV_LOAD_IMAGE_UNCHANGED);
	transform(originalImg);
}

void main() {
	//test3();
	test4();
}




