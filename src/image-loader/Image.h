#ifndef _IMAGE_
#define _IMAGE_

struct Color
{
	unsigned char R, G, B;
};

class Image
{
public:
	Image(const char* fname)
    {
      this->fname = fname;
    }
    const char* getFilename()
    {
      return this->fname;
    }
	virtual int Save(const char *fname) = 0;

	virtual int SetPixel(int row, int col, Color color) = 0;
	virtual int GetPixel(int row, int col, Color &color) = 0;

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int SetWidth() = 0;
	virtual int SetHeight() = 0;

private:
    const char * fname;
};

#endif
