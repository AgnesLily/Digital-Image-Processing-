#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "math.h"

using namespace cv;
using namespace std;
#define M_PI 3.14159265358979323846
//得到某像素
uchar*  get_pixel(const Mat &img, int x, int y) {
	return (uchar*)img.data + x*img.step + y * 3;
}
//边界处理,镜像处理
Mat& boundaryTreat(Mat &input, int windowSize){
	Mat result;
	result.cols = input.cols + windowSize - 1;
	result.rows = input.rows + windowSize - 1;
	for (int i = windowSize / 2-1; i < input.rows; i++){
		for (int j = windowSize / 2-1; j < input.cols; j++){
			Vec3b pixel;
			uchar *original = get_pixel(input, i - windowSize / 2, j-windowSize/2);
			pixel[0] = original[0];
			pixel[1] = original[1];
			pixel[2] = original[2];
			result.at<Vec3b>(i, j) = pixel;
		}
	}
	//先处理高边界
	for (int i = windowSize / 2; i < input.rows; i++){
		for (int j = 0; j < windowSize / 2; j++){
			Vec3b pixel;
			uchar *original = get_pixel(input, i - windowSize / 2, j);
			pixel[0] = original[0];
			pixel[1] = original[1];
			pixel[2] = original[2];
			result.at<Vec3b>(i, j) = pixel;
		}
	}
	for (int i = windowSize / 2; i < input.rows; i++){
		for (int j = windowSize / 2 + input.cols - 1; j < result.cols; j++){
			Vec3b pixel;
			uchar *original = get_pixel(input, i - windowSize / 2, j);
			pixel[0] = original[0];
			pixel[1] = original[1];
			pixel[2] = original[2];
			result.at<Vec3b>(i, j) = pixel;
		}
	}

	//再处理宽边界
	
	for (int i = 0; i < windowSize / 2; i++){
		for (int j = 0; j < result.cols; j++){
			Vec3b pixel;
			uchar *original = get_pixel(result, windowSize-i-1, j);
			//cout << original[0];
			pixel[0] = original[0];
			pixel[1] = original[1];
			pixel[2] = original[2];
			result.at<Vec3b>(i, j) = pixel;
		}
	}
	

	for (int i = windowSize / 2 + input.rows - 1; i < result.rows; i++){
		for (int j = 0; j < result.cols; j++){
			Vec3b pixel;
			uchar *original = get_pixel(result, i-windowSize, j);
			pixel[0] = original[0];
			pixel[1] = original[1];
			pixel[2] = original[2];
			result.at<Vec3b>(i, j) = pixel;
		}
	}
	return result;
}

//得到一维高斯滤波的滤波核
double* getGaussianTemplate(int sigma,int windowSize,int flag){
	int size = windowSize*windowSize;
	double model[1000];
	int index;
	if (flag == 1){
		int centerx = windowSize / 2 + 1;
		for (int i = 0; i < windowSize; i++){
			for (int j = 0; j < windowSize; j++){
				int dis = i - centerx;
				double value = exp(-(1 / 2)*dis*dis / (sigma*sigma)) / (sqrt(2 * M_PI) * sigma);
				model[index] = value;
				index++;
			}
		}
	}
	if (flag == 2){
		int centery = windowSize / 2 + 1;
		for (int i = 0; i < windowSize; i++){
			for (int j = 0; j < windowSize; j++){
				int dis = j - centery;
				double value = exp(-(1 / 2)*dis*dis / (sigma*sigma)) / (sqrt(2 * M_PI) * sigma);
				model[index] = value;
				index++;
			}
		}
	}
	return model;
}

void Gaussian(const Mat &input, Mat &output, double sigma){
	int windowSize = (floor(sigma * 6 - 1)) / 2 * 2 + 1;
	//先进行x轴的滤波
	double *xmodel = getGaussianTemplate(sigma, windowSize,1);
	for (int i = 0; i < input.cols; i++){
		for (int j = 0; j < input.rows; j++){
			double sum;
			for (int m = 0; m < windowSize; m++){
				for (int n = 0; n < windowSize; n++){
					Vec3b pixel;
					int locationx;
					int locationy;
					if (m <= windowSize / 2){
						locationx = i - (windowSize/2-m);
						//出现在图像上面
						if (locationx < 0){
							locationx = i - (i - locationx);
						}
					}
					else{
						locationx = i + m - windowSize / 2;
						//出现在图像下面
						if (locationx >= input.rows){
							locationx = i - (locationx - i);
						}
					}
					if (n <= windowSize / 2){
						locationy = j - (windowSize/2-n);
						//出现在图像左侧
						if (locationy < 0){
							locationy = j;
						}
						
					}
					else{
						locationy = j + n - windowSize / 2;
					}
					
					


					uchar *pixel = get_pixel(input, locationx, locationy);
					
				}
			}
		}
	}
	//再进行y轴的滤波
	double *ymodel = getGaussianTemplate(sigma, windowSize, 2);
}


//void MeanFilter(const Mat &input, Mat &output, int window_size);


void testGaussian(){

}

void testMean(){

}
void testBoundray(){
	Mat bgImg = imread("bg.png", CV_LOAD_IMAGE_UNCHANGED);//背景图
	Mat result = boundaryTreat(bgImg, 5);                          
	imshow("showImage", result);
}
void main(){
	//testGaussian();
	//testMean();
	//testBoundray();
}

