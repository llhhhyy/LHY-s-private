#include <opencv2\opencv.hpp> 
#include <iostream>
#include<cstring>
using namespace std;
using namespace cv;

void getr(float* r, uint8_t** Image, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			r[Image[i][j]]++;	
		}
	}
	
}

void show_lzb( float arr[], int size, string title)
{
	Mat drawImage = Mat::zeros(Size(256, 256), CV_8UC3);

	int _max = 0;
	for (int i = 0; i < size; i++)
	{
		if (arr[i] > _max)
		{
			_max = arr[i];//�ҵ������е����ֵ��������Ҫ��һ��
		}
	}

	for (int i = 0; i < 256; i++)
	{
		int current_value = (int)(double(arr[i]) / double(_max) * 256);//ÿ�ζ�Ҫ�������ֵ�������ֵ��һ����0-255֮��
		line(drawImage, Point(i, drawImage.rows - 1), Point(i, drawImage.rows - 1 - current_value), Scalar(255, 0, 255));
	}
	imshow(title, drawImage);
}
