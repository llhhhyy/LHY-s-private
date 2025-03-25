#include<iostream>
using namespace std;
void trangray(uint8_t** Image, uint8_t*** image, int width, int hight)
{
	for (int i = 0; i < hight; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Image[i][j] = image[i][j][0] * 0.299 + image[i][j][1] * 0.587 + image[i][j][2] * 0.114;
		}
	}
}
void junheng(uint8_t** IB, float** B, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			IB[i][j] = int(B[i][j]);
		}
	}
}
void tran(float** B, float r[], uint8_t** Image, int bmpWidth, int bmpHeight)
{
	int N = bmpWidth * bmpHeight;
	for (int i = 0; i < bmpHeight; i++)
	{
		for (int j = 0; j < bmpWidth; j++)
		{
			for (int k = 0; k < Image[i][j]; k++)
			{
				B[i][j] += r[k]/N;
			}
			B[i][j] = 256 * B[i][j];
		}
	}
}
void tranRGB(uint8_t** IB, uint8_t*** image, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j][0] = IB[i][j];
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j][1] = IB[i][j];
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j][2] = IB[i][j];
		}
	}
}

