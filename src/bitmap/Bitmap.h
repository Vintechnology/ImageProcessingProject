#ifndef _IMAGE_
#define _IMAGE_

struct Color
{
	unsigned char R, G, B;
};

class Image
{
public:
	Image(const char* fname);
	int Save(const char *fname);
	~Image();

	int SetPixel(int row, int col, Color color);
	int GetPixel(int row, int col, Color &color);

	int GetWidth();
	int GetHeight();
	int SetWidth();
	int SetHeight();

	//void AdjustBrightness(const Bitmap &bmp, double factor);
	//void Enlarge(const Bitmap &inbmp, Bitmap &outbmp, int factor);
private:
	int width;
	int height;
	int rowSize;
	unsigned char *pixels;
};
#endif
