#pragma once
#include "Blur.h"
#include "bitmap/Bitmap.h"
#include <math.h>

enum Direction
{
	north,
	north_west,
	west,
	west_south,
	south,
	south_east,
	east,
	east_north
};

Bitmap CopySize(const Bitmap &in)
{
	// TODO: Copy height and width to out bimap
	Bitmap out;
	out.height = in.height;
	out.width = in.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];
	return out;
}

Bitmap MakeSquareBitmap(const Bitmap &in)
{
	// TODO: Copy bitmap to a Square bitmap
	Bitmap temp;
	if (in.height > in.width)
	{
		temp.height = in.height;
		temp.width = temp.height;
		temp.rowSize = ((temp.width * 3 + 3) / 4) * 4;
		temp.pixels = new unsigned char[temp.rowSize*temp.height];
		for (int row = 0; row < in.height; row++)
		for (int col = 0; col < in.width; col++)
		{
			Color color;
			GetPixel(in, row, col, color);
			SetPixel(temp, row, col, color);
		}
	}
	else if (in.height < in.width)
	{
		temp.height = in.width;
		temp.width = in.width;
		temp.rowSize = ((temp.width * 3 + 3) / 4) * 4;
		temp.pixels = new unsigned char[temp.rowSize*temp.height];
		for (int row = 0; row < in.height; row++)
		for (int col = 0; col < in.width; col++)
		{
			Color color;
			GetPixel(in, row, col, color);
			SetPixel(temp, row, col, color);
		}
	}
	else
	{
		temp.height = in.height;
		temp.width = in.width;
		temp.rowSize = ((temp.width * 3 + 3) / 4) * 4;
		temp.pixels = new unsigned char[temp.rowSize*temp.height];
		for (int row = 0; row < in.height; row++)
		for (int col = 0; col < in.width; col++)
		{
			Color color;
			GetPixel(in, row, col, color);
			SetPixel(temp, row, col, color);
		}
	}
	return temp;
}

Bitmap Sobel(const Bitmap &in)
{
	Bitmap out;
	Bitmap temp = MakeSquareBitmap(in);
	out = CopySize(in);

	const int sobel_X[] = { -1, 0, +1, -2, 0, +2, -1, 0, +1 };
	const int sobel_Y[] = { -1, -2, -1, 0, 0, 0, +1, +2, +1 };

	double Gx;
	double Gy;

	for (int y = 1; y<temp.height - 1;y++)
	{
		for (int x = 1; x < temp.width - 1;x++)
		{
			Gx = 0;
			Gy = 0;

			for (int row = 0; row < 3; row++)
			{
				for (int col = 0; col < 3; col++)
				{
					int pixel_X = x + (row - 1);
					int pixel_Y = y + (col - 1);
					Color color;
					GetPixel(temp, pixel_X, pixel_Y, color);
					int value = static_cast<int>(color.G);

					int index = row * 3 + col;
					Gx += sobel_X[index] * value;
					Gy += sobel_Y[index] * value;
				}
			}

			double G = sqrt(Gx*Gx + Gy*Gy);
			int valueOut = static_cast<int>(G);

			if (valueOut > 255)
				valueOut = 255;
			if (valueOut < 0)
				valueOut = 0;

			Color colorOut;
			colorOut.B = static_cast<unsigned char>(valueOut);
			colorOut.G = static_cast<unsigned char>(valueOut);
			colorOut.R = static_cast<unsigned char>(valueOut);

			SetPixel(out, x, y, colorOut);
		}
	}
	return out;
}

Bitmap Robert(const Bitmap &in)
{
	Bitmap out;
	Bitmap temp = MakeSquareBitmap(in);
	out = CopySize(in);

	const int robertX[] = { +1, 0, 0, -1 };
	const int robertY[] = { 0, +1, -1, 0 };

	for (int row = 1; row < temp.height - 1; row++)
	{
		for (int col = 1; col < temp.width - 1; col++)
		{
			double Gx = 0;
			double Gy = 0;
			for (int r = 0; r < 2; r++)
			for (int c = 0; c < 2; c++)
			{
				int pixel_X = col + (r - 1);
				int pixel_Y = row + (c - 1);

				Color color;
				GetPixel(temp, pixel_X, pixel_Y, color);
				int value = static_cast<int>(color.R);

				int index = r * 2 + c;
				Gx += robertX[index] * value;
				Gy += robertY[index] * value;
			}

			double G = sqrt(Gx*Gx + Gy*Gy);
			int color_out = static_cast<int>(G);

			if (color_out>255)
				color_out = 255;
			if (color_out < 0)
				color_out = 0;

			Color colorout;
			colorout.R = static_cast<unsigned char>(color_out);
			colorout.G = static_cast<unsigned char>(color_out);
			colorout.B = static_cast<unsigned char>(color_out);

			SetPixel(out, col, row, colorout);
		}
	}
	return out;
}

Bitmap Prewitt(const Bitmap &in)
{
	Bitmap out;
	Bitmap temp = MakeSquareBitmap(in);
	out = CopySize(in);

	const int prewittY[] = { -1, -1, -1, 0, 0, 0, +1, +1, +1 };
	const int prewittX[] = { -1, 0, +1, -1, 0, +1, -1, 0, +1 };

	for (int row = 1; row < temp.height - 1; row++)
	{
		for (int col = 1; col < temp.width - 1; col++)
		{
			double Gx = 0;
			double Gy = 0;
			for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
			{
				int pixel_X = col + (r - 1);
				int pixel_Y = row + (c - 1);

				Color color;
				GetPixel(temp, pixel_X, pixel_Y, color);
				int value = static_cast<int>(color.R);

				int index = r * 3 + c;
				Gx += prewittX[index] * value;
				Gy += prewittY[index] * value;
			}

			double G = sqrt(Gx*Gx + Gy*Gy);
			int color_out = static_cast<int>(G);

			if (color_out>255)
				color_out = 255;
			if (color_out < 0)
				color_out = 0;

			Color colorout;
			colorout.R = static_cast<unsigned char>(color_out);
			colorout.G = static_cast<unsigned char>(color_out);
			colorout.B = static_cast<unsigned char>(color_out);

			SetPixel(out, col, row, colorout);
		}
	}
	return out;
}

Bitmap Kirsch(const Bitmap &in, Direction dir)
{
	Bitmap out;
	Bitmap temp = MakeSquareBitmap(in);
	out = CopySize(in);

	const int North[] = { 5, 5, 5, -3, 0, -3, -3, -3, -3};
	const int North_West[]= { 5, 5, -3, 5, 0, -3, -3, -3, -3};
	const int West[] = { 5, -3, -3, 5, 0, -3, 5, -3, -3 };
	const int West_South[] = { -3, -3, -3, 5, 0, -3, 5, 5, -3 };
	const int South[] = { -3, -3, -3, -3, 0, -3, 5, 5, 5 };
	const int South_East[] = { -3, -3, 5, -3, 0, 5, -3, 5, 5 };
	const int East[] = { -3, -3, 5, -3, 0, 5, -3, -3, 5 };
	const int East_North[] = { -3, 5, 5, -3, 0, 5, -3, -3, -3 };

	for (int row = 1; row < temp.height - 1; row++)
	{
		for (int col = 1; col < temp.width - 1; col++)
		{
			double G[8];
			for (int i = 0; i < 8; i++)
			{
				G[i] = 0;
			}

			for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
			{
				int pixel_X = col + (r - 1);
				int pixel_Y = row + (c - 1);

				Color color;
				GetPixel(temp, pixel_X, pixel_Y, color);
				int value = static_cast<int>(color.R);

				int index = r * 3 + c;

				if (dir == north)
					G[0] += North[index] * value;
				if (dir == north_west)
					G[1] += North_West[index] * value;
				if (dir == west)
					G[2] += West[index] * value;
				if (dir == west_south)
					G[3] += West_South[index] * value;
				if (dir == south)
					G[4] += South[index] * value;
				if (dir == south_east)
					G[5] += South_East[index] * value;
				if (dir == east)
					G[6] += East[index] * value;
				if (dir == east_north)
					G[7] += East_North[index] * value;
			}
			double g = sqrt(G[0] * G[0] + G[1] * G[1] + G[2] * G[2] + G[3] * G[3] + G[4] * G[4] + G[5] * G[5] + G[6] * G[6] + G[7] * G[7]);
			int color_out = static_cast<int>(g);

			if (color_out>255)
				color_out = 255;
			if (color_out < 0)
				color_out = 0;

			Color colorout;
			colorout.R = static_cast<unsigned char>(color_out);
			colorout.G = static_cast<unsigned char>(color_out);
			colorout.B = static_cast<unsigned char>(color_out);

			SetPixel(out, col, row, colorout);
		}
	}
	return out;
}

Bitmap LaplacianOfGaussian(Bitmap bmp)
{
	Bitmap result;
	result.height = bmp.height;
	result.width = bmp.width;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.height * result.rowSize];
	//blur input image
	bmp = GaussianBlur(bmp, 1, 2);

	int **Gray, **tempGray;
	Gray = new int*[bmp.height]; tempGray = new int*[bmp.height];
	for (int i = 0; i < bmp.height; i++)
	{
		Gray[i] = new int[bmp.width];
		tempGray[i] = new int[bmp.width];
	}

	int** kernel;
	kernel = new int*[3];
	for (int i = 0; i < 3; i++)
	{
		kernel[i] = new int[3];
	}for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
		kernel[i][j] = -1;
	kernel[1][1] = 8;

	//turn input image into grayscale
	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			Color temp;
			GetPixel(bmp, i, j, temp);
			Gray[i][j] = 0.299 * temp.R + 0.587 * temp.G + 0.114 * temp.B;
		}
	}
	//apply LoG kernel to input image
	convolution2D(Gray, tempGray, kernel, bmp.height, bmp.width, 1);

	//zero crossing
	int threshold = 20;
	for (int i = 1; i < result.height - 1; i++)
	{
		for (int j = 1; j < result.width - 1; j++)
		{
			int min = 255;

			min = (tempGray[i - 1][j] < min) ? tempGray[i - 1][j] : min;
			min = (tempGray[i + 1][j] < min) ? tempGray[i + 1][j] : min;
			min = (tempGray[i][j - 1] < min) ? tempGray[i][j - 1] : min;
			min = (tempGray[i][j + 1] < min) ? tempGray[i][j + 1] : min;

			if (tempGray[i][j] - min > threshold && tempGray[i][j] * min < 0) Gray[i][j] = 255;
			else Gray[i][j] = 0;
		}
	}

	for (int i = 0; i < result.height; i++)
	{
		for (int j = 0; j < result.width; j++)
		{
			Color temp;
			temp.B = Gray[i][j]; temp.G = Gray[i][j]; temp.R = Gray[i][j];
			SetPixel(result, i, j, temp);
		}
	}
	return result;
}