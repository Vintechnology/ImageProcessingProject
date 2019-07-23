#ifndef _BITMAP_
#define _BITMAP_
#include <iostream>
#include <cstring>
#include <string>
#include "Image.h"

class Bitmap: public Image
{
  public:
    Bitmap(const char* filename);
    ~Bitmap();

	virtual int Save(const char *fname);

	virtual int SetPixel(int row, int col, Color color);
	virtual int GetPixel(int row, int col, Color &color);

	virtual int GetWidth();
	virtual int GetHeight();
	virtual int SetWidth();
	virtual int SetHeight();
    
  private:

}

#endif
