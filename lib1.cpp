#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;
using namespace std;

//提取alpha通道并显示
void test1(){
	Mat img = imread("a.png", CV_LOAD_IMAGE_UNCHANGED);
	int imageHeight = img.rows;//图像的高
	int imageWidth = img.cols;//图像的宽
	int step = img.step;//每行的字节数
	uchar *inputImageData = img.data;//图像信息
	int nc = img.channels();;//通道数
	Mat outImg(imageHeight, imageWidth, CV_8UC4);

	//遍历像素
	for (int i = 0; i < imageHeight; ++i, inputImageData += step)
	{
		uchar *px = inputImageData;
		for (int j = 0; j < imageWidth; ++j,px+=nc)
		{
			Vec4b pixelo;
			pixelo[0] = px[3];
			pixelo[1] = px[3];
			pixelo[2] = px[3];
			pixelo[3] = px[3];
			outImg.at<Vec4b>(i, j) = pixelo;
		}
	}
	imshow("showImage", outImg);
	waitKey();
}

//得到某像素
uchar*  get_pixel(const Mat &img, int x, int y) { 
	return (uchar*)img.data + y*img.step + x * 4;
}

//使用alpha混合替换背景图void test2(){
	Mat bgImg = imread("bg.png", CV_LOAD_IMAGE_UNCHANGED);//背景图
	Mat frImg = imread("a.png", CV_LOAD_IMAGE_UNCHANGED);//前景图
	int imageHeight = frImg.rows;//ROI区域的高
	int imageWidth = frImg.cols;//ROI区域的宽
	int step = bgImg.step;//每行的字节数
	int nc = bgImg.channels();//背景图通道数

	uchar *ROI = get_pixel(bgImg, 0, 0);//得到ROI区域的起始像素位置

	for (int i = 0; i < imageHeight; ++i, ROI += step)
	{
		uchar *px = ROI;//遍历ROI得到的像素
		for (int j = 0; j < imageWidth; ++j, px += nc)
		{
			uchar *pixel = get_pixel(frImg, j, i);//前景图在(j,i)的像素!!		
			//alpha混合算法
			double opcity = (double)(pixel[3]) / 255;
			px[0] = pixel[0] * opcity + px[0] * (1 - opcity);
			px[1] = pixel[1] * opcity + px[1] * (1 - opcity);
			px[2] = pixel[2] * opcity + px[2] * (1 - opcity);
			px[3] = pixel[3] * opcity + px[3] * (1 - opcity);	
		}
	}	
	imshow("showImage", bgImg);	
	waitKey();
}

void main() {
	test1();
	//test2();
}






