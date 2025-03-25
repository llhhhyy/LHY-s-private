#include "Windows.h"
#include<iostream>
#include<cstring>
#include"getr.h"
#include"input_output.h"
#include"trangray.h"
#include <opencv2\opencv.hpp> 

using namespace cv;
using namespace std;
int main()
{
	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	uint8_t*** image;//RGB图像的像素值
	uint8_t** Image;//灰度图像的像素值
	int i, j;

	//读取图像, 将像素值存储到image中
	char readPath[] = "C:/Users/96317/Desktop/刘浩宇常用/作业/图像处理/pics/12.bmp";
	readBmp(readPath, image, bmpWidth, bmpHeight);
	
	cout << "width=" << bmpWidth << " height=" << bmpHeight << endl;
	//将RGB图像转化为灰度图像
	Image = new uint8_t*[bmpHeight + 10];
	for (int i = 0; i < bmpHeight; i++)
	{
		Image[i] = new uint8_t[bmpWidth + 10];
	}
	trangray(Image, image, bmpWidth, bmpHeight);
	//统计每一个灰度值对应的像素点个数
    float r[256] = {0};
	getr(r, Image, bmpWidth, bmpHeight);
	show_lzb(r, 256, "zhifangtu1");
	waitKey(0);
	//直方图均衡化
	float** B;
	B = new float*[bmpHeight + 2];
	for (int i = 0; i < bmpHeight; i++)
	{
		B[i] = new float[bmpWidth + 2];
	}
	for (int i = 0; i < bmpHeight; i++)
	{
		for (int j = 0; j < bmpWidth; j++)
		{
			B[i][j] = 0;
		}
	}
	tran(B, r, Image, bmpWidth, bmpHeight);
	
	uint8_t** IB;
	IB = new uint8_t * [bmpHeight + 2];
	for (int i = 0; i < bmpHeight; i++)
	{
		IB[i] = new uint8_t[bmpWidth + 2];
	}
	junheng(IB, B, bmpWidth, bmpHeight);
	//均衡后直方图
	getr(r, IB, bmpWidth, bmpHeight);
	show_lzb(r, 256, "zhifangtu2");
	waitKey(0);
	//灰度图转RGB
	tranRGB(IB, image, bmpWidth, bmpHeight);
	//存储image
	char writePath[] = "C:/Users/96317/Desktop/刘浩宇常用/作业/图像处理/pics/my_image12.bmp";
	saveBmp(writePath, image, bmpWidth, bmpHeight);

	//清除image以及grey image空间
	for (i = 0; i < bmpHeight; ++i) {
		for (j = 0; j < bmpWidth; ++j) {
			delete image[i][j];
		}
		delete image[i];
	}
	delete image;
	for (i = 0; i < bmpHeight; ++i) {
		delete Image[i];
	}
	delete []Image;
	for (i = 0; i < bmpHeight; ++i) {
		
		delete IB[i];
	}
	delete []IB;
	for (i = 0; i < bmpHeight; ++i) {

		delete B[i];
	}
	delete []B;
}
