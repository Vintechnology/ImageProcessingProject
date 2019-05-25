#pragma once
#include "bitmap/Bitmap.h"
#include "featureList.h"

typedef Bitmap(*ArgumentParser)(char** arg, int length, int &offset);

Bitmap SobelParser(char** arg, int length, int &offset) 
{
	return Sobel(bmp);
}
/*
Bitmap MediumBlurParser(const Bitmap &bmp, char** arg, int length)
{
}
*/