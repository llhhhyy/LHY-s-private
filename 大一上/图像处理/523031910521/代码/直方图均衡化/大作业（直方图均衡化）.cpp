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
	int bmpWidth;//ͼ��Ŀ�
	int bmpHeight;//ͼ��ĸ�
	uint8_t*** image;//RGBͼ�������ֵ
	uint8_t** Image;//�Ҷ�ͼ�������ֵ
	int i, j;

	//��ȡͼ��, ������ֵ�洢��image��
	char readPath[] = "C:/Users/96317/Desktop/�������/��ҵ/ͼ����/pics/12.bmp";
	readBmp(readPath, image, bmpWidth, bmpHeight);
	
	cout << "width=" << bmpWidth << " height=" << bmpHeight << endl;
	//��RGBͼ��ת��Ϊ�Ҷ�ͼ��
	Image = new uint8_t*[bmpHeight + 10];
	for (int i = 0; i < bmpHeight; i++)
	{
		Image[i] = new uint8_t[bmpWidth + 10];
	}
	trangray(Image, image, bmpWidth, bmpHeight);
	//ͳ��ÿһ���Ҷ�ֵ��Ӧ�����ص����
    float r[256] = {0};
	getr(r, Image, bmpWidth, bmpHeight);
	show_lzb(r, 256, "zhifangtu1");
	waitKey(0);
	//ֱ��ͼ���⻯
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
	//�����ֱ��ͼ
	getr(r, IB, bmpWidth, bmpHeight);
	show_lzb(r, 256, "zhifangtu2");
	waitKey(0);
	//�Ҷ�ͼתRGB
	tranRGB(IB, image, bmpWidth, bmpHeight);
	//�洢image
	char writePath[] = "C:/Users/96317/Desktop/�������/��ҵ/ͼ����/pics/my_image12.bmp";
	saveBmp(writePath, image, bmpWidth, bmpHeight);

	//���image�Լ�grey image�ռ�
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
