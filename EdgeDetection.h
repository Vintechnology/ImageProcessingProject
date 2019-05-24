#include "Bitmap.h"
#include <math.h>

Bitmap Sobel(const Bitmap &in)
{
	Bitmap out;
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

	out.height = in.height;
	out.width = in.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];

	const int sobel_X[] = { -1, 0, +1, -2, 0, +2, -1, 0, +1 };
	const int sobel_Y[] = { -1, -2, -1, 0, 0, 0, +1, +2, +1 };

	double Gx;
	double Gy;

	for (int y = 1; y<temp.height - 1; y++)
	{
		for (int x = 1; x < temp.width - 1; x++)
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

	out.height = in.height;
	out.width = in.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];

	const int robertX[] = { +1, 0, 0, -1 };
	const int robertY[] = { 0, +1, -1, 0 };

	for (int row = 1; row < temp.height - 1; row++)
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
	return out;
}

Bitmap Prewitt(const Bitmap &in)
{
	Bitmap out;
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

	out.height = in.height;
	out.width = in.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];

	const int prewittY[] = { -1, -1, -1, 0, 0, 0, +1, +1, +1 };
	const int prewittX[] = { -1, 0, +1, -1, 0, +1, -1, 0, +1 };

	for (int row = 1; row < temp.height - 1; row++)
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
	return out;
}