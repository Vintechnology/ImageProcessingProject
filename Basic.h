#include "Bitmap.h"
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