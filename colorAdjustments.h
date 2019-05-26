#pragma once

#include "bitmap\Bitmap.h"
#include <algorithm>

const double pi = 3.1415926;

struct HSL
{
	unsigned int H, S, L;
};

//

void RGBtoHSL(Color& rgb, HSL& hsl);
void HSLtoRGB(HSL& hsl, Color& rgb);
Bitmap ContrastAdjust(const Bitmap& bmp, int value);
Bitmap NearestColour(const Bitmap& bmp, Color colours[], int colorCount);
Bitmap LevelsAdjust(const Bitmap& bmp, unsigned char input[], unsigned char output[]);
Bitmap ErrorDiffuse(const Bitmap& bmp, Color colours[], int colorCount);
Bitmap ExposureAdjust(const Bitmap& bmp, double value);

//

void RGBtoHSL(Color& rgb, HSL& hsl)
{
	//special cases
	if (rgb.R == 0 && rgb.G == 0 && rgb.B == 0)
	{
		hsl = { 0,0,0 };
		return;
	}
	if (rgb.R == 255 && rgb.G == 255 && rgb.B == 255)
	{
		hsl = { 0,0,100 };
		return;
	}

	double r, g, b, max, min;
	r = rgb.R / 255.0;
	g = rgb.G / 255.0;
	b = rgb.B / 255.0;
	max = std::max(std::max(r, g), b);
	min = std::min(std::min(r, g), b);
	double h, s, l;

	//Lightness
	l = (max + min) / 2.0;
	hsl.L = 100 * l;

	//Saturation
	if (l > 0.5)
		s = (max - min) / (2.0 - max - min);
	else
		s = (max - min) / (max + min);
	hsl.S = 100 * s;

	//Hue
	if (max - min == 0.0 - 0.0)
		h = 0.0;
	else
		if (max == r)
			h = (g - b) / (max - min);
		else
			if (max == g)
				h = 2.0 + (b - r) / (max - min);
			else
				h = 4.0 + (r - g) / (max - min);
	hsl.H = 60 * h + 360;
	if (hsl.H > 359) hsl.H -= 360;
}

void HSLtoRGB(HSL& hsl, Color& rgb)
{
	double s, l; unsigned int h;
	h = hsl.H;
	s = hsl.S / 100.0;
	l = hsl.L / 100.0;
	double max, min, mid;

	//max
	if (l < 0.5)
		max = l * (1.0 + s);
	else
		max = l + s - l * s;

	//min
	min = 2.0 * l - max;

	//mid
	unsigned int temp = h % 120;
	mid = abs(temp - 60.0) / (-60.0) + 1;
	mid = mid * (max - min) + min;

	//rgb
	if ((h + 60) / 120 == 1)
		rgb.G = max * 255;
	else
		if ((h + 60) / 120 == 2)
			rgb.B = max * 255;
		else
			rgb.R = max * 255;

	if (h / 120 == 0)
		rgb.B = min * 255;
	else
		if (h / 120 == 1)
			rgb.R = min * 255;
		else
			rgb.G = min * 255;

	if ((h / 60) % 3 == 1)
		rgb.R = mid * 255;
	else
		if ((h / 60) % 3 == 2)
			rgb.B = mid * 255;
		else
			rgb.G = mid * 255;
}

unsigned char Truncate(int value)
{
	if (value < 0) return 0;
	if (value > 255) return 255;
	return value;
}

Bitmap ContrastAdjust(const Bitmap& bmp, int value)
{
	Bitmap result;
	result.width = bmp.width;
	result.height = bmp.height;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.rowSize * result.height];

	double factor = (255.0 + value) / (255.0 - 63.0 / 64.0 * value);

	Color c;
	for (int row = 0; row < bmp.height; row++)
		for (int col = 0; col < bmp.width; col++)
		{
			GetPixel(bmp, row, col, c);

			c.R = Truncate((factor * (c.R - 128) + 128));
			c.G = Truncate((factor * (c.G - 128) + 128));
			c.B = Truncate((factor * (c.B - 128) + 128));

			SetPixel(result, row, col, c);
		}
	return result;
}

Bitmap NearestColour(const Bitmap& bmp, Color colours[], int colorCount)
{
	//{ {0,0,0},{255,0,0},{255,255,0},{0,255,0},{0,255,255},{0,0,255},{255,0,255},{255,255,255} }

	Bitmap result;
	result.width = bmp.width;
	result.height = bmp.height;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.rowSize * result.height];

	Color c;
	for (int row = 0; row < bmp.height; row++)
		for (int col = 0; col < bmp.width; col++)
		{
			GetPixel(bmp, row, col, c);
			
			Color nearestColour;
			int minimumDistance = 255 * 255 + 255 * 255 + 255 * 255 + 1;
			for (int i = 0; i < colorCount; i++)
			{
				int rDiff = c.R - colours[i].R;
				int gDiff = c.G - colours[i].G;
				int bDiff = c.B - colours[i].B;
				int distance = rDiff * rDiff + gDiff * gDiff + bDiff * bDiff;
				if (distance < minimumDistance)
				{
					minimumDistance = distance;
					nearestColour = colours[i];
				}
			}

			SetPixel(result, row, col, nearestColour);
		}
	return result;
}

Bitmap LevelsAdjust(const Bitmap& bmp, unsigned char input[], unsigned char output[])
{
	Bitmap result;
	result.width = bmp.width;
	result.height = bmp.height;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.rowSize * result.height];

	//Apply gamma correction
	double gamma = 1.0;
	double midtoneNormal = input[1] / 255.0;
	if (input[1] < 128)
	{
		midtoneNormal *= 2;
		gamma = 1 + 9 * (1 - midtoneNormal);
		gamma = std::min(gamma, 9.99);
	}
	else
		if (input[1] > 128)
		{
			midtoneNormal = midtoneNormal * 2 - 1;
			gamma = 1 - midtoneNormal;
			gamma = std::max(gamma, 0.01);
		}
	double gammaCorrection = 1 / gamma;

	Color c;
	for (int row = 0; row < bmp.height; row++)
		for (int col = 0; col < bmp.width; col++)
		{
			GetPixel(bmp, row, col, c);

			//Apply the input levels
			c.R = Truncate(255.0 * (c.R - input[0]) / (double)(input[2] - input[0]));
			c.G = Truncate(255.0 * (c.G - input[0]) / (double)(input[2] - input[0]));
			c.B = Truncate(255.0 * (c.B - input[0]) / (double)(input[2] - input[0]));

			//Apply the midtones
			if (input[1] != 128)
			{
				c.R = Truncate(255 * pow(c.R / 255.0, gammaCorrection));
				c.G = Truncate(255 * pow(c.G / 255.0, gammaCorrection));
				c.B = Truncate(255 * pow(c.B / 255.0, gammaCorrection));
			}

			//Apply the output levels
			c.R = Truncate((c.R / 255.0) * (output[1] - output[0]) + output[0]);
			c.G = Truncate((c.G / 255.0) * (output[1] - output[0]) + output[0]);
			c.B = Truncate((c.B / 255.0) * (output[1] - output[0]) + output[0]);

			SetPixel(result, row, col, c);
		}
	return result;
}

Bitmap ErrorDiffuse(const Bitmap& bmp, Color colours[], int colorCount)
{
	Bitmap result;
	result.width = bmp.width;
	result.height = bmp.height;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.rowSize * result.height];

	//{ {0,0,0},{255,0,0},{255,255,0},{0,255,0},{0,255,255},{0,0,255},{255,0,255},{255,255,255} }

	Color c;
	for (int row = 0; row < bmp.height; row++)
		for (int col = 0; col < bmp.width; col++)
		{
			GetPixel(bmp, row, col, c);

			Color nearestColour;
			int minimumDistance = 255 * 255 + 255 * 255 + 255 * 255 + 1;
			for (int i = 0; i < colorCount; i++)
			{
				int rDiff = c.R - colours[i].R;
				int gDiff = c.G - colours[i].G;
				int bDiff = c.B - colours[i].B;
				int distance = rDiff * rDiff + gDiff * gDiff + bDiff * bDiff;
				if (distance < minimumDistance)
				{
					minimumDistance = distance;
					nearestColour = colours[i];
				}
			}

			int rTemp = c.R - nearestColour.R;
			int gTemp = c.G - nearestColour.G;
			int bTemp = c.B - nearestColour.B;
			Color cTemp;
			if (GetPixel(bmp, row, col + 1, cTemp) != 0)
			{
				cTemp.R = Truncate(7.0 / 16.0 * rTemp + cTemp.R);
				cTemp.G = Truncate(7.0 / 16.0 * gTemp + cTemp.G);
				cTemp.B = Truncate(7.0 / 16.0 * bTemp + cTemp.B);
				SetPixel(bmp, row, col + 1, cTemp);
			}
			if (GetPixel(bmp, row + 1, col + 1, cTemp) != 0)
			{
				cTemp.R = Truncate(1.0 / 16.0 * rTemp + cTemp.R);
				cTemp.G = Truncate(1.0 / 16.0 * gTemp + cTemp.G);
				cTemp.B = Truncate(1.0 / 16.0 * bTemp + cTemp.B);
				SetPixel(bmp, row + 1, col + 1, cTemp);
			}
			if (GetPixel(bmp, row + 1, col, cTemp) != 0)
			{
				cTemp.R = Truncate(5.0 / 16.0 * rTemp + cTemp.R);
				cTemp.G = Truncate(5.0 / 16.0 * gTemp + cTemp.G);
				cTemp.B = Truncate(5.0 / 16.0 * bTemp + cTemp.B);
				SetPixel(bmp, row + 1, col, cTemp);
			}
			if (GetPixel(bmp, row + 1, col - 1, cTemp) != 0)
			{
				cTemp.R = Truncate(3.0 / 16.0 * rTemp + cTemp.R);
				cTemp.G = Truncate(3.0 / 16.0 * gTemp + cTemp.G);
				cTemp.B = Truncate(3.0 / 16.0 * bTemp + cTemp.B);
				SetPixel(bmp, row + 1, col - 1, cTemp);
			}

			SetPixel(result, row, col, nearestColour);
		}
	return result;
}

Bitmap ExposureAdjust(const Bitmap& bmp, double value)
{
	Bitmap result;
	result.width = bmp.width;
	result.height = bmp.height;
	result.rowSize = ((3 * result.width + 3) / 4) * 4;
	result.pixels = new unsigned char[result.rowSize * result.height];

	Color c;
	for (int row = 0; row < bmp.height; row++)
		for (int col = 0; col < bmp.width; col++)
		{
			GetPixel(bmp, row, col, c);

			c.R = Truncate(c.R * pow(2, value));
			c.G = Truncate(c.G * pow(2, value));
			c.B = Truncate(c.B * pow(2, value));

			SetPixel(result, row, col, c);
		}
	return result;
}
