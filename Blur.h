#include "bitmap\Bitmap.h"
#include "linalg\linalg.h"
#include <iostream>

#define PI 3.1415926

Color Medium(Bitmap bmp, int i, int j, int R)
{
	Color result;
	unsigned int Red = 0, G = 0, B = 0;
	int count = 0;

	for (int u = i - R; u <= i + R; u++)
	{
		for (int v = j - R; v <= j + R; v++)
		{
			if ((u >= 0) && (u < bmp.height) && (v >= 0) && (v < bmp.width))
			{
				Color temp;
				GetPixel(bmp, u, v, temp);
				Red += temp.R; G += temp.G; B += temp.B;
				count++;
			}
		}
	}result.B = B / count;
	result.G = G / count;
	result.R = Red / count;
	return result;
}

Bitmap MediumBlur(Bitmap bmp, int R)
{
	Bitmap result;
	result.height = bmp.height;
	result.width = bmp.width;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.height * result.rowSize];

	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			SetPixel(result, i, j, Medium(bmp, i, j, R));
		}
	}
	return result;
}

double* FilterCreator(int R, double sigma)
{
	double* result = new double[2 * R + 1];
	for (int x = -R; x <= R; x++)
	{
		result[x + R] = (exp(-(x*x) / (2 * sigma*sigma)) / (sqrt(2 * PI)*sigma));
	}return result;
}

void ConvertKernel(double* input, int R, linalg::Matrix &horizontalKernel, linalg::Matrix &verticalKernel)
{
	horizontalKernel.resize(1); verticalKernel.resize(2 * R + 1); horizontalKernel[0].resize(2 * R + 1);
	for (int i = 0; i <= 2 * R; i++)
	{
		horizontalKernel[0][i] = input[i] / input[0];
		verticalKernel[i].resize(1);
		verticalKernel[i][0] = input[i] / input[0];
	}
}

int SumKernel(linalg::Matrix kernel, int R)
{
	int result = 0;
	for (int i = 0; i <= 2 * R; i++)
	{
		for (int j = 0; j <= 2 * R; j++)
		{
			result += kernel[i][0] * kernel[j][0];
		}
	}return result;
}

Bitmap GaussianBlur(Bitmap bmp, int R, double sigma)
{

	Bitmap result;
	result.height = bmp.height;
	result.width = bmp.width;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.height * result.rowSize];

	linalg::Matrix Blue, Red, Green, tempBlue, tempRed, tempGreen;
	Blue.resize(bmp.height); Red.resize(bmp.height); Green.resize(bmp.height);
	tempBlue.resize(bmp.height); tempRed.resize(bmp.height); tempGreen.resize(bmp.height);
	for (int i = 0; i < bmp.height; i++)
	{
		Blue[i].resize(bmp.width);
		Red[i].resize(bmp.width);
		Green[i].resize(bmp.width);
		tempBlue[i].resize(bmp.width);
		tempRed[i].resize(bmp.width);
		tempGreen[i].resize(bmp.width);
	}

	linalg::Matrix horizontalKernel, verticalKernel;
	ConvertKernel(FilterCreator(R, sigma), R, horizontalKernel, verticalKernel);

	int factor = SumKernel(verticalKernel, R);

	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			Color temp;
			GetPixel(bmp, i, j, temp);
			Blue[i][j] = temp.B; Green[i][j] = temp.G; Red[i][j] = temp.R;
		}
	}

	linalg::convolution1D(Red, tempRed, horizontalKernel); linalg::convolution1D(tempRed, Red, verticalKernel);
	linalg::convolution1D(Green, tempGreen, horizontalKernel); linalg::convolution1D(tempGreen, Green, verticalKernel);
	linalg::convolution1D(Blue, tempBlue, horizontalKernel); linalg::convolution1D(tempBlue, Blue, verticalKernel);

	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			Color temp;
			temp.B = Blue[i][j] / factor; temp.G = Green[i][j] / factor; temp.R = Red[i][j] / factor;
			SetPixel(result, i, j, temp);
		}
	}
	return result;
}