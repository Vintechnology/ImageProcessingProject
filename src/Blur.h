#pragma once
#include "bitmap/Bitmap.h"
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

double** FilterCreator(int R, double sigma)
{
	double** result = new double*[2 * R + 1];
	for (int i = 0; i <= 2 * R; i++)
		result[i] = new double[2 * R + 1];
	for (int x = -R; x <= R; x++)
	{
		for (int y = -R; y <= R; y++)
			result[x + R][y + R] = (exp(-(x*x + y*y) / (2 * sigma*sigma)) / (2 * PI*sigma*sigma));
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

void convolution2D(int** input, int** output, double** kernel, int height, int width, int R, double sum)
{
	double** temp = new double*[height];
	for (int i = 0; i < height; i++)
		temp[i] = new double[width];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			temp[i][j] = 0;
			for (int u = i - R; u <= i + R; u++)
			for (int v = j - R; v <= j + R; v++)
			{
				if ((u >= 0) && (u < height) && (v >= 0) && (v < width))
				{
					temp[i][j] += input[u][v] * kernel[u - i + R][v - j + R];
				}
			}
			output[i][j] = temp[i][j] / sum;
		}
	}
	delete[] temp;
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

	double** kernel2D = new double*[2 * R + 1];
	for (int i = 0; i <= 2 * R; i++)
	{
		kernel2D[i] = new double[2 * R + 1];
	}
	kernel2D = FilterCreator(R, sigma);
	double sum = 0;
	for (int i = 0; i <= 2 * R; i++)
	for (int j = 0; j <= 2 * R; j++)
		sum += kernel2D[i][j];
	
	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			Color temp;
			GetPixel(bmp, i, j, temp);
			Blue[i][j] = temp.B; Green[i][j] = temp.G; Red[i][j] = temp.R;
		}
	}

	convolution2D(Blue, tempBlue, kernel2D, bmp.height, bmp.width, R, sum);
	convolution2D(Red, tempRed, kernel2D, bmp.height, bmp.width, R, sum);
	convolution2D(Green, tempGreen, kernel2D, bmp.height, bmp.width, R, sum);

	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			Color temp;
			temp.B = tempBlue[i][j]; temp.G = tempGreen[i][j]; temp.R = tempRed[i][j];
			SetPixel(result, i, j, temp);
		}
	}
	return result;
}