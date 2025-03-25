#include<iostream>
using namespace std;
void trangray(uint8_t** Image, uint8_t*** image, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Image[i][j] = image[i][j][0] * 0.299 + image[i][j][1] * 0.587 + image[i][j][2] * 0.114;
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
			image[i][j][1] = IB[i][j];
			image[i][j][2] = IB[i][j];
		}
	}
}

