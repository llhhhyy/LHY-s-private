#include "Windows.h"
#include<iostream>
#include<cstring>
#include"input_output2.h"
#include"tran.h"
#include"getI.h"
#include"juanji.h"
using namespace std;
int main()
{
	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	uint8_t*** image;//RGB图像的像素值
	uint8_t** Image;//灰度图像的像素值
	int i, j;

	//读取图像, 将像素值存储到image中
	char readPath[] = "C:/Users/96317/Desktop/刘浩宇常用/作业/图像处理/pics/6.bmp";
	readBmp(readPath, image, bmpWidth, bmpHeight);

	cout << "width=" << bmpWidth << " height=" << bmpHeight << endl;
	//将RGB图像转化为灰度图像
	Image = new uint8_t * [bmpHeight];
	for (int i = 0; i < bmpHeight; i++)
	{
		Image[i] = new uint8_t[bmpWidth];
	}
	trangray(Image, image, bmpWidth, bmpHeight);
	//像素矩阵的增广形式
	float** I;
	I = new float* [bmpHeight + 2];
	for (int i = 0; i < bmpHeight + 2; i++)
	{
		I[i] = new float[bmpWidth + 2];
	}
	getI(I, Image, bmpWidth, bmpHeight);
	//卷积
	float** T;
	T = new float* [bmpHeight+2];
	for (int i = 0; i < bmpHeight+2; i++)
	{
		T[i] = new float[bmpWidth + 2];
	}
	juanji1(T, I, bmpWidth, bmpHeight);
	
	//卷积后结果返回到像素数组
	getIb(Image, T, bmpWidth, bmpHeight);
	//灰度图转RGB
	tranRGB(Image, image, bmpWidth, bmpHeight);
	//存储image
	char writePath[] = "C:/Users/96317/Desktop/刘浩宇常用/作业/图像处理/pics/6my_image1.bmp";
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
	delete[]Image;
	for (i = 0; i < bmpHeight; ++i) {

		delete I[i];
	}
	delete[]I;
	for (i = 0; i < bmpHeight; ++i) {

		delete T[i];
	}
	delete[]T;
}
