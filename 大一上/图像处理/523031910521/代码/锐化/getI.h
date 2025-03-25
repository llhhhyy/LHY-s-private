#include<iostream>
using namespace std;
void getI(float** I, uint8_t** Image, int width, int height)
{
	for (int i = 0; i < width + 2; i++)
	{
		I[0][i] = 0;
		I[height+1][i] = 0;
	}
	for (int i = 0; i < height + 2; i++)
	{
		I[i][0] = 0;
		I[i][width + 1] = 0;
	}
	for (int i = 1; i < height + 1; i++)
	{
		for (int j = 1; j < width + 1; j++)
		{
			I[i][j] = Image[i - 1][j - 1];
		}
	}
}
void getIb(uint8_t** Image, float** T, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Image[i][j] = T[i][j];
		}
	}
}
