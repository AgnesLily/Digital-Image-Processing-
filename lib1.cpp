#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

using namespace cv;
using namespace std;

//��ȡalphaͨ������ʾ
void test1(){
	Mat img = imread("a.png", CV_LOAD_IMAGE_UNCHANGED);
	int imageHeight = img.rows;//ͼ��ĸ�
	int imageWidth = img.cols;//ͼ��Ŀ�
	int step = img.step;//ÿ�е��ֽ���
	uchar *inputImageData = img.data;//ͼ����Ϣ
	int nc = img.channels();;//ͨ����
	Mat outImg(imageHeight, imageWidth, CV_8UC4);

	//��������
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

//�õ�ĳ����
uchar*  get_pixel(const Mat &img, int x, int y) { 
	return (uchar*)img.data + y*img.step + x * 4;
}

//ʹ��alpha����滻����ͼ
void test2(){
	Mat bgImg = imread("bg.png", CV_LOAD_IMAGE_UNCHANGED);//����ͼ
	Mat frImg = imread("a.png", CV_LOAD_IMAGE_UNCHANGED);//ǰ��ͼ
	int imageHeight = frImg.rows;//ROI����ĸ�
	int imageWidth = frImg.cols;//ROI����Ŀ�
	int step = bgImg.step;//ÿ�е��ֽ���
	int nc = bgImg.channels();//����ͼͨ����

	uchar *ROI = get_pixel(bgImg, 0, 0);//�õ�ROI�������ʼ����λ��

	for (int i = 0; i < imageHeight; ++i, ROI += step)
	{
		uchar *px = ROI;//����ROI�õ�������
		for (int j = 0; j < imageWidth; ++j, px += nc)
		{
			uchar *pixel = get_pixel(frImg, j, i);//ǰ��ͼ��(j,i)������!!		
			//alpha����㷨
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


//��˹�˲�
/*
void Gaussian(const Mat  &input, Mat  &output, double sigma){	int window = floor(6 * sigma - 1) / 2 * 2 + 1;//����	int cols = input.cols;	int rows = input.rows;		//ʹ�����з������������˲�	//�ȶ��н��и�˹�˲�	for (int i = 0; i < rows; i++){		for (int j = 0; j < cols; j++){		}	}	//�ٶ��н��и�˹�˲�}*/


