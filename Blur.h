#pragma once
#include "bitmap/Bitmap.h"
#include "linalg/linalg.h"
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

Bitmap MediumBlur(const Bitmap &bmp, int R)
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

int** ConvertKernel(double* input, int R, int* kernel1D)
{
	int** kernel2D = new int*[2 * R + 1];
	for (int i = 0; i <= 2 * R; i++)
	{
		kernel2D[i] = new int[2 * R + 1];
		kernel1D[i] = input[i] / input[0];
	}
	for (int i = 0; i <= 2 * R; i++)
	{
		for (int j = 0; j <= 2 * R; j++)
		{
			kernel2D[i][j] = kernel1D[i] * kernel1D[j];
		}
	}return kernel2D;
}

int SumKernel(int* kernel, int R)
{
	int result = 0;
	for (int i = 0; i <= 2 * R; i++)
	{
		for (int j = 0; j <= 2 * R; j++)
		{
			result += kernel[i] * kernel[j];
		}
	}return result;
}

void convolution2D(int** input, int** output, int** kernel, int height, int width, int R)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			output[i][j] = 0;
			for (int u = i - R; u <= i + R; u++)
			for (int v = j - R; v <= j + R; v++)
			{
				if ((u >= 0) && (u < height) && (v >= 0) && (v < width))
				{
					output[i][j] += input[u][v] * kernel[u - i + R][v - j + R];
				}
			}
		}
	}
}

Bitmap GaussianBlur(const Bitmap &bmp, int R, double sigma)
{

	Bitmap result;
	result.height = bmp.height;
	result.width = bmp.width;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.height * result.rowSize];

	int **Blue, **Green, **Red, **tempBlue, **tempGreen, **tempRed;
	Blue = new int*[bmp.height]; Green = new int*[bmp.height]; Red = new int*[bmp.height];
	tempBlue = new int*[bmp.height]; tempGreen = new int*[bmp.height]; tempRed = new int*[bmp.height];

	for (int i = 0; i < bmp.height; i++)
	{
		Blue[i] = new int[bmp.width]; Green[i] = new int[bmp.width]; Red[i] = new int[bmp.width];
		tempBlue[i] = new int[bmp.width]; tempGreen[i] = new int[bmp.width]; tempRed[i] = new int[bmp.width];
	}

	int* kernel1D;
	kernel1D = new int[2 * R + 1];
	int** kernel2D = new int*[2 * R + 1];
	for (int i = 0; i <= 2 * R; i++)
	{
		kernel2D[i] = new int[2 * R + 1];
	}
	kernel2D = ConvertKernel(FilterCreator(R, sigma), R, kernel1D);

	int factor = SumKernel(kernel1D, R);

	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			Color temp;
			GetPixel(bmp, i, j, temp);
			Blue[i][j] = temp.B; 
			Green[i][j] = temp.G; 
			Red[i][j] = temp.R;
		}
	}

	convolution2D(Blue, tempBlue, kernel2D, bmp.height, bmp.width, R);
	convolution2D(Red, tempRed, kernel2D, bmp.height, bmp.width, R);
	convolution2D(Green, tempGreen, kernel2D, bmp.height, bmp.width, R);

	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			Color temp;

			temp.B = tempBlue[i][j] / factor; temp.G = tempGreen[i][j] / factor; temp.R = tempRed[i][j] / factor;

			SetPixel(result, i, j, temp);
		}
	}
	return result;
}