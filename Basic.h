#include "bitmap\Bitmap.h"
#include <iostream>

Bitmap Turn90Degree(Bitmap bmp)
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

Bitmap Turn180Degree(Bitmap bmp)
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

Bitmap Turn270Degree(Bitmap bmp)
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

Bitmap Crop(Bitmap bmp, int xA, int yA, int xB, int yB)
{//TODO: Crop the image inner part 2 points A(xA, yA) and B(xB, yB)
	if (xA < 0 || xA >= bmp.height || yA < 0 || yA >= bmp.width
		|| xB < 0 || xB >= bmp.height || yB < 0 || yB >= bmp.width)
	{
		std::cout << "Incorrect input!!! (please check if A or B is bigger than the image size)" << std::endl;
	}
	else
	{
		int x1, y1, x2, y2;
		if (xA < xB){
			x1 = xA; x2 = xB;
		}
		else {
			x1 = xB; x2 = xA;
		}
		if (yA < yB){
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

Bitmap IntegrateImages(Bitmap input1, Bitmap input2, float A, float B)
{// A and B is the clarity of image "input1" and image "input2"
	if ((input1.height != input2.height) || (input1.width != input2.width)) std::cout << "The input image size is not correct!!!" << std::endl;
	else
	{
		Bitmap result;
		result.height = input1.height;
		result.width = input1.width;
		result.rowSize = ((3 * result.width + 3) / 4) * 4;
		result.pixels = new unsigned char[result.height * result.rowSize];

		float factor = A / (A + B);

		for (int i = 0; i < result.height; i++)
		{
			for (int j = 0; j < result.width; j++)
			{
				Color temp1, temp2, temp;
				GetPixel(input1, i, j, temp1);
				GetPixel(input2, i, j, temp2);
				temp.B = temp1.B*factor + temp2.B*(1 - factor);
				temp.G = temp1.G*factor + temp2.G*(1 - factor);
				temp.R = temp1.R*factor + temp2.R*(1 - factor);
				SetPixel(result, i, j, temp);
			}
		}
		return result;
	}
}