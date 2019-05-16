#include "Bitmap.h"
struct Grayscale
{
	unsigned char I;
};
void convertGrayScale(const Bitmap &bmp, Bitmap &out);
void holdRedColor(const Bitmap &bmp, Bitmap &out);