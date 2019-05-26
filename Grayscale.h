#pragma once
#include "bitmap\Bitmap.h"
#include "Blur.h"
#include <cmath>
struct Grayscale
{
	unsigned char I;
};

struct HSVcolor
{
	double H, S, V;
};

enum colorCanHold { RED, YELLOW, GREEN, BLUE, PURPLE };

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

HSVcolor convertRGBtoHSV(Color in)
{
	HSVcolor    out;
	double      min, max, delta;

	min = in.R < in.G ? in.R : in.G;
	min = min  < in.B ? min : in.B;

	max = in.R > in.G ? in.R : in.G;
	max = max  > in.B ? max : in.B;

	out.V = max;                                
	delta = max - min;
	if (delta < 0.00001)
	{
		out.S = 0;
		out.H = 0; 
		return out;
	}
	if (max > 0.0) {
		out.S = (delta / max);                  
	}
	else {
		out.S = 0.0;
		out.H = NAN;                            
		return out;
	}
	if (in.R >= max)                         
		out.H = (in.G - in.B) / delta;        
	else
		if (in.G >= max)
			out.H = 2.0 + (in.B - in.R) / delta;  
		else
			out.H = 4.0 + (in.R - in.G) / delta;  

	out.H *= 60.0;

	if (out.H < 0.0)
		out.H += 360.0;

	return out;
}

Color convertHSVtoRGB(HSVcolor in)
{
	double      hh, p, q, t, ff;
	long        i;
	Color         out;

	if (in.S <= 0.0)
	{
		out.R = in.V;
		out.G = in.V;
		out.B = in.V;
		return out;
	}
	hh = in.H;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.V * (1.0 - in.S);
	q = in.V * (1.0 - (in.S * ff));
	t = in.V * (1.0 - (in.S * (1.0 - ff)));

	switch (i)
	{
	case 0:
		out.R = in.V;
		out.G = t;
		out.B = p;
		break;
	case 1:
		out.R = q;
		out.G = in.V;
		out.B = p;
		break;
	case 2:
		out.R = p;
		out.G = in.V;
		out.B = t;
		break;

	case 3:
		out.R = p;
		out.G = q;
		out.B = in.V;
		break;
	case 4:
		out.R = t;
		out.G = p;
		out.B = in.V;
		break;
	case 5:
	default:
		out.R = in.V;
		out.G = p;
		out.B = q;
		break;
	}
	return out;
}

void holdAColor(const Bitmap &bmp, Bitmap &out, colorCanHold clor)
{
	out.height = bmp.height;
	out.width = bmp.width;
	out.rowSize = ((out.width * 3 + 3) / 4) * 4;
	out.pixels = new unsigned char[out.rowSize*out.height];
	int delta = 30;

	int temp3 = clor * 60;
	double minCos = cos(delta / (180 * PI));
	for (int row = 0; row < bmp.height; row++)
	{
		for (int col = 0; col < bmp.width; col++)
		{
			Color temp;
			HSVcolor HSVtemp;
			Grayscale gray;
			GetPixel(bmp, row, col, temp);
			HSVtemp = convertRGBtoHSV(temp);
			if (cos(((int)HSVtemp.H - temp3) / (180 * PI)) > minCos)
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


