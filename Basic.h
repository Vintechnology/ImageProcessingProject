#pragma once
#define _USE_MATH_DEFINES
#include "bitmap/Bitmap.h"
#include <iostream>
#include <math.h>

Bitmap Rotate(const Bitmap &inbmp, int degree)
{
	Bitmap outbmp;
	outbmp.width = inbmp.width;
	outbmp.height = inbmp.height;
	outbmp.rowSize = inbmp.rowSize;
	outbmp.pixels = new unsigned char[outbmp.rowSize * outbmp.height];
	memset(outbmp.pixels, 0, outbmp.rowSize*outbmp.height);

	double radian = degree * M_PI / 180.0;

	int x0 = inbmp.width / 2;
	int y0 = inbmp.height / 2;
	for (int x = 0; x < inbmp.width; x++)
	{
		for (int y = 0; y < inbmp.height; y++)
		{
			int a = x - x0;
			int b = y - y0;
			int xx = (int)(round(a * cos(radian) - b * sin(radian)) + x0);
			int yy = (int)(round(a * sin(radian) + b * cos(radian)) + y0);
			Color color;
			if (GetPixel(inbmp, yy, xx, color)) {
				SetPixel(outbmp, y, x, color);
			}
			else {
				SetPixel(outbmp, y, x, { 0,0,0 });
			}
		}
	}
}

Bitmap Turn90Degree(Bitmap &bmp)
{
	Bitmap result;
	result.height = bmp.width;
	result.width = bmp.height;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.height * result.rowSize];

	for (int i = 0; i < result.width; i++)
	{
		for (int j = 0; j < result.height; j++)
		{
			Color temp;
			GetPixel(bmp, i, j, temp);
			SetPixel(result, result.height - j, result.width - i, temp);
		}
	}
	return result;
}

Bitmap Turn180Degree(Bitmap &bmp)
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
			Color temp;
			GetPixel(bmp, i, j, temp);
			SetPixel(result, result.height - i, result.width - j, temp);
		}
	}
	return result;
}

Bitmap Turn270Degree(Bitmap &bmp)
{
	Bitmap result;
	result.height = bmp.width;
	result.width = bmp.height;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.height * result.rowSize];

	for (int i = 0; i < result.width; i++)
	{
		for (int j = 0; j < result.height; j++)
		{
			Color temp;
			GetPixel(bmp, i, j, temp);
			SetPixel(result, j, i, temp);
		}
	}
	return result;
}

Bitmap Crop(Bitmap &bmp, int xA, int yA, int xB, int yB)
{//TODO: Crop the image inner part 2 points A(xA, yA) and B(xB, yB)
		
	int x1, y1, x2, y2;
		
	if (xA < xB)
	{
		x1 = xA; x2 = xB;
	}
	else 
	{
		x1 = xB; x2 = xA;
	}
		
	if (yA < yB)
	{
		y1 = yA; y2 = yB;
	}
	else
	{
		y1 = yB; y2 = yA;
	}
	Bitmap result;
	result.height = y2 - y1 + 1;
	result.width = x2 - x1 + 1;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.height * result.rowSize];
	for (int i = y1; i <= y2; i++)
	{
		for (int j = x1; j <= x2; j++)
		{
			Color temp;
			GetPixel(bmp, i, j, temp);
			SetPixel(result, i - y1, j - x1, temp);
		}
	}
	return result;
}

Bitmap FlipLR(const Bitmap &in)
{
	Bitmap out;
	out.height = in.height;
	out.width = in.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];
	for (int row = 0; row < in.height; row++)
	{
		for (int col = 0; col < in.width; col++)
		{
			Color color;
			GetPixel(in, row, col, color);
			SetPixel(out, row, in.width - col - 1, color);
		}
	}
	return out;
}

Bitmap FlipUD(const Bitmap &in)
{
	Bitmap out;
	out.height = in.height;
	out.width = in.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];
	for (int row = 0; row < in.height; row++)
	{
		for (int col = 0; col < in.width; col++)
		{
			Color color;
			GetPixel(in, row, col, color);
			SetPixel(out, in.height - 1 - row, col, color);
		}
	}
	return out;
}

Bitmap FlipAll(const Bitmap &in)
{
	Bitmap out;
	out.height = in.height;
	out.width = in.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];
	for (int row = 0; row < in.height; row++)
	{
		for (int col = 0; col < in.width; col++)
		{
			Color color;
			GetPixel(in, row, col, color);
			SetPixel(out, in.height - 1 - row, in.width - 1 - col, color);
		}
	}
	return out;
}

Bitmap Resize(const Bitmap &in, int newWidth, int newHeight)
{
	Bitmap out;
	double factor_x;
	double factor_y;

	factor_x = (double)newWidth / in.width;
	factor_y = (double)newHeight / in.height;
	//out.width = (int)(in.width*factor_x);
	//out.height = (int)(in.height*factor_y);

	out.width = newWidth;
	out.height = newHeight;
	out.rowSize = ((3 * out.width + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];
	for (int row = 0; row < in.height; row++)
	{
		for (int col = 0; col < in.width; col++)
		{
			Color color;
			GetPixel(in, row, col, color);
			for (int i = 0; i <= (int)(factor_y + 1); i++)
			{
				for (int j = 0; j <= (int)(factor_x + 1); j++)
				{
					SetPixel(out, (int)(row*factor_y + i), (int)(col* factor_x + j), color);
				}
			}
		}
	}
	return out;
}

Bitmap Resize(const Bitmap &in, double factor)
{
	Bitmap out;
	out.width = (int)(in.width*factor);
	out.height = (int)(in.height*factor);
	out.rowSize = ((3 * out.width + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];
	for (int row = 0; row < in.height; row++)
	{
		for (int col = 0; col < in.width; col++)
		{
			Color color;
			GetPixel(in, row, col, color);
			for (int i = 0; i <= (factor + 1); i++)
			{
				for (int j = 0; j <= (factor + 1); j++)
				{
					SetPixel(out, (int)(row*factor + i), (int)(col* factor + j), color);
				}
			}
		}
	}
	return out;
}