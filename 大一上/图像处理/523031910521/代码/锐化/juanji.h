#include<iostream>
using namespace std;
//锐化卷积核
// 拉普拉斯核
extern float ker0[3][3];
float ker0[3][3] = { 0,-1,0,-1,5,-1,0,-1,0 };
//平滑卷积
extern float ker1[3][3];
float ker1[3][3] = {0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111,0.1111};
//检测竖线
extern float ker2[3][3];
float ker2[3][3] = { {1,0,-1},{1,0,-1}, {1,0,-1} };
void juanji0(float** T, float** I, int width, int height)
{
	for (int i = 1; i < height; i++)
	{
		for (int j = 1; j < width; j++)
		{
			T[i - 1][j - 1] = I[i - 1][j - 1] * ker0[0][0] + I[i][j - 1] * ker0[0][1] + I[i + 1][j - 1] * ker0[0][2] + I[i - 1][j] * ker0[1][0] + I[i][j] * ker0[1][1] + I[i + 1][j] * ker0[1][2] + I[i - 1][j + 1] * ker0[2][0] + I[i][j + 1] * ker0[2][1] + I[i + 1][j + 1] * ker0[2][2];
		}
	}
}
void juanji1(float** T,float** I, int width, int height)
{
	for (int i = 1; i < height; i++)
	{
		for (int j = 1; j < width; j++)
		{
			T[i - 1][j - 1] = I[i - 1][j - 1] * ker1[0][0] + I[i][j - 1] * ker1[0][1] + I[i + 1][j - 1] * ker1[0][2] + I[i - 1][j] * ker1[1][0] + I[i][j] * ker1[1][1] + I[i + 1][j] * ker1[1][2] + I[i - 1][j + 1] * ker1[2][0] + I[i][j + 1] * ker1[2][1] + I[i + 1][j + 1] * ker1[2][2];			
		}
	}
}
void juanji2(float** T, float** I, int width, int height)
{
	for (int i = 1; i < height; i++)
	{
		for (int j = 1; j < width; j++)
		{
			T[i - 1][j - 1] = I[i - 1][j - 1] * ker2[0][0] + I[i][j - 1] * ker2[0][1] + I[i + 1][j - 1] * ker2[0][2] + I[i - 1][j] * ker2[1][0] + I[i][j] * ker2[1][1] + I[i + 1][j] * ker2[1][2] + I[i - 1][j + 1] * ker2[2][0] + I[i][j + 1] * ker2[2][1] + I[i + 1][j + 1] * ker2[2][2];
		}
	}
}
