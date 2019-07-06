#include <stdio.h>
#include <string.h>
#include "Bitmap.h"

#pragma pack(1)

struct BMFHeader
{
	char signature[2];
	unsigned int size;
	unsigned int reserved;
	unsigned int pixelAddress;
};

struct DIBHeader
{
	unsigned int size;
	unsigned int width;
	unsigned int height;
	unsigned short colorPlane;
	unsigned short colorDepth;
	unsigned char reserved[24];
};

int LoadBitmap(const char *fname, Bitmap &bmp)
{
	FILE *f = fopen(fname, "rb");
	if (f == NULL)
		return 0;
	
	// read BMFHeader
	BMFHeader bmfhd;
	if(fread(&bmfhd, sizeof(BMFHeader), 1, f) == 0)
		return 0;
	
	if(bmfhd.signature[0] != 'B' || bmfhd.signature[1] != 'M')
		return 0;
		
	// read DIBHeader
	DIBHeader dibhd;
	if(fread(&dibhd, sizeof(DIBHeader), 1, f) == 0)
		return 0;
		
	if(dibhd.colorDepth != 24 || dibhd.height < 0)
		return 0;
	
	// read pixels (24 bits = 3 bytes/pixel)
	bmp.width = dibhd.width;
	bmp.height = dibhd.height;
	bmp.rowSize = ((3 * bmp.width + 3)/4)*4;
	
	bmp.pixels = new unsigned char[bmp.height * bmp.rowSize];
	if(bmp.pixels == NULL)
		return 0;
		
	fseek(f, bmfhd.pixelAddress, SEEK_SET);
	if(fread(bmp.pixels, bmp.rowSize, bmp.height, f) < bmp.height)
		return 0;
		
	fclose(f);
	
	return 1;
}

int SaveBitmap(const char *fname, const Bitmap &bmp)
{
	FILE *f = fopen(fname, "wb");
	if(f == NULL)
		return 0;
	
	// write BMFHeader	
	BMFHeader bmfhd;
	bmfhd.signature[0] = 'B';
	bmfhd.signature[1] = 'M';
	bmfhd.size = sizeof(BMFHeader) + sizeof(DIBHeader) + bmp.rowSize*bmp.height;
	bmfhd.reserved = 0;
	bmfhd.pixelAddress = sizeof(BMFHeader) + sizeof(DIBHeader);
	if(fwrite(&bmfhd, sizeof(BMFHeader), 1, f) == 0)
		return 0;
	
	// write DIBHeader
	DIBHeader dibhd;
	dibhd.size = sizeof(DIBHeader);
	dibhd.width = bmp.width;
	dibhd.height = bmp.height;
	dibhd.colorPlane = 1;
	dibhd.colorDepth = 24;
	memset(&dibhd.reserved, 0, sizeof(dibhd.reserved));
	if(fwrite(&dibhd, sizeof(DIBHeader), 1, f) == 0)
		return 0;
	
	// write pixels
	if(fwrite(bmp.pixels, bmp.height, bmp.rowSize, f) < bmp.height)
		return 0;
		
	fclose(f);
	
	return 1;
}

void DisposeBitmap(Bitmap &bmp)
{
	if(bmp.pixels != NULL)
	{
		delete[] bmp.pixels;
		bmp.pixels = NULL;
	}
}
