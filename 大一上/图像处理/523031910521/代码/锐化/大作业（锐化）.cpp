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
	int bmpWidth;//ͼ��Ŀ�
	int bmpHeight;//ͼ��ĸ�
	uint8_t*** image;//RGBͼ�������ֵ
	uint8_t** Image;//�Ҷ�ͼ�������ֵ
	int i, j;

	//��ȡͼ��, ������ֵ�洢��image��
	char readPath[] = "C:/Users/96317/Desktop/�������/��ҵ/ͼ����/pics/6.bmp";
	readBmp(readPath, image, bmpWidth, bmpHeight);

	cout << "width=" << bmpWidth << " height=" << bmpHeight << endl;
	//��RGBͼ��ת��Ϊ�Ҷ�ͼ��
	Image = new uint8_t * [bmpHeight];
	for (int i = 0; i < bmpHeight; i++)
	{
		Image[i] = new uint8_t[bmpWidth];
	}
	trangray(Image, image, bmpWidth, bmpHeight);
	//���ؾ����������ʽ
	float** I;
	I = new float* [bmpHeight + 2];
	for (int i = 0; i < bmpHeight + 2; i++)
	{
		I[i] = new float[bmpWidth + 2];
	}
	getI(I, Image, bmpWidth, bmpHeight);
	//���
	float** T;
	T = new float* [bmpHeight+2];
	for (int i = 0; i < bmpHeight+2; i++)
	{
		T[i] = new float[bmpWidth + 2];
	}
	juanji1(T, I, bmpWidth, bmpHeight);
	
	//����������ص���������
	getIb(Image, T, bmpWidth, bmpHeight);
	//�Ҷ�ͼתRGB
	tranRGB(Image, image, bmpWidth, bmpHeight);
	//�洢image
	char writePath[] = "C:/Users/96317/Desktop/�������/��ҵ/ͼ����/pics/6my_image1.bmp";
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
