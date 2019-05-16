#include "Grayscale.h"
#include <cmath>
void getGrayColor(Color color, Grayscale &pGray)
{
	pGray.I = (2 * color.R + 5 * color.G + color.B) / 8;

}
int setAGrayPixel(const Bitmap&bmp, int row, int col, Grayscale gray)
{
	if (row < 0 || row >= bmp.height
		|| col < 0 || col >= bmp.width)
		return 0;

	// color: B, G, R
	int offset = (bmp.height - 1 - row)*bmp.rowSize + col * 3;
	bmp.pixels[offset] = gray.I;
	bmp.pixels[offset + 1] = gray.I;
	bmp.pixels[offset + 2] = gray.I;
}
void convertGrayScale(const Bitmap &bmp, Bitmap &out)
{
	out.height = bmp.height;
	out.width = bmp.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];

	for (int row = 0; row < bmp.height; row++)
	{
		for (int col = 0; col < bmp.width; col++)
		{
			Color temp;
			Grayscale gray;
			GetPixel(bmp, row, col, temp);
			getGrayColor(temp, gray);
			setAGrayPixel(out, row, col, gray);
		}
	}
}
void holdRedColor(const Bitmap &bmp, Bitmap &out)
{
	out.height = bmp.height;
	out.width = bmp.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];

	for (int row = 0; row < bmp.height; row++)
	{
		for (int col = 0; col < bmp.width; col++)
		{
			Color temp;
			Grayscale gray;
			GetPixel(bmp, row, col, temp);
			if (temp.R >= 180 && temp.G <= 95 && temp.B <= 95)
			{
				SetPixel(out, row, col, temp);
			}
			else
			{
				getGrayColor(temp, gray);
				setAGrayPixel(out, row, col, gray);
			}
		}
	}
}
