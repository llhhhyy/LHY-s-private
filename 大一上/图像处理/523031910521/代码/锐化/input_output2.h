#include<iostream>
#include "Windows.h"
using namespace std;
//����������һ����ͼ��Ķ�ȡ��һ����ͼ��ı���
/***********************************************************************
* �������ƣ�
* readBmp()
*
*����������
*  char *bmpName -�ļ����ּ�·��
*  uint8_t *** &image -Ҫ������ֵ�洢�����ڴ�ռ��ָ��
*  int& bmpWidth ͼ����
*  int& bmpHeight ͼ��߶�
*����ֵ��
*   bool���� 0Ϊʧ��,1Ϊ�ɹ�
*   uint8_t *** &image����ά����,��СΪbmpHeight*bmpWidth*3
*   int& bmpWidth ͼ����
*   int& bmpHeight ͼ��߶�
*˵��������һ��ͼ���ļ�������·������ͼ����������ݡ����ߵ����ݽ��ڴ�,�������Ӧ�ı�����
***********************************************************************/
bool readBmp(char* bmpName, uint8_t***& image, int& bmpWidth, int& bmpHeight)
{
	FILE* fp = fopen(bmpName, "rb");
	if (fp == 0) return 0;
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	bmpWidth = head.biWidth;
	bmpHeight = head.biHeight;
	int biBitCount = head.biBitCount;
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	if (biBitCount != 24) {
		cout << "����ͼ�����Ϊ24λ��ɫͼ" << endl;
		return 0;
	}
	unsigned char* pBmpBuf = new unsigned char[lineByte * bmpHeight];
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
	int bmpWidthBytes = bmpWidth * 3;
	if (bmpWidth % 4 > 0) {
		bmpWidthBytes = (bmpWidthBytes / 4 + 1) * 4;
	}
	image = new uint8_t * *[bmpHeight];
	int i, j, k;
	for (i = 0; i < bmpHeight; ++i) {
		image[i] = new uint8_t * [bmpWidth];
		for (j = 0; j < bmpWidth; ++j) {
			image[i][j] = new uint8_t[3];
			for (k = 0; k < 3; ++k) {
				image[i][j][k] = (uint8_t)pBmpBuf[(bmpHeight - 1 - i) * bmpWidthBytes + j * 3 + k];
			}
		}
	}
	delete[]pBmpBuf;
	fclose(fp);
	return 1;
}

/***********************************************************************
* �������ƣ�
* saveBmp()
*
*����������
*  char *bmpName -�ļ����ּ�·��
*  uint8_t *** image  -�����̵��������� ��СΪheight*width*3����ά����
*  int width   -����Ϊ��λ������λͼ�Ŀ�
*  int  height  -����Ϊ��λ������λͼ��
*����ֵ��
*   0Ϊʧ��,1Ϊ�ɹ�
*
*˵��������һ��ͼ���������ݡ�������Ϣ������д��ָ���ļ���
***********************************************************************/
bool saveBmp(char* bmpName, uint8_t*** image, int width, int height)
{
	if (!image)
		return 0;
	int biBitCount = 24;
	int colorTablesize = 0;
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
	FILE* fp = fopen(bmpName, "wb");
	if (fp == 0) return 0;
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ colorTablesize + lineByte * height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	fileHead.bfOffBits = 54 + colorTablesize;
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
	BITMAPINFOHEADER head;
	head.biBitCount = biBitCount;
	head.biClrImportant = 0;
	head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = lineByte * height;
	head.biWidth = width;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	unsigned char* imgBuf = new unsigned char[height * lineByte]();
	int i, j, k;
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			for (k = 0; k < 3; ++k) {
				imgBuf[(height - 1 - i) * lineByte + j * 3 + k] = image[i][j][k];
			}
		}
	}
	fwrite(imgBuf, height * lineByte, 1, fp);
	delete[]imgBuf;
	fclose(fp);
	return 1;
}
