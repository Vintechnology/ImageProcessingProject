#pragma once
#include "Basic.h"
#include "Blur.h"
#include "EdgeDetection.h"
#include "colorAdjustments.h"
#include "Grayscale.h"

// Basic
Bitmap Rotate(const Bitmap &inbmp, int degree);
Bitmap Turn90Degree(Bitmap &bmp);
Bitmap Turn180Degree(Bitmap &bmp);
Bitmap Turn270Degree(Bitmap &bmp);
Bitmap Crop(Bitmap &bmp, int xA, int yA, int xB, int yB);
Bitmap FlipLR(const Bitmap &in);
Bitmap FlipUD(const Bitmap &in);
Bitmap FlipAll(const Bitmap &in);
Bitmap Resize(const Bitmap &in, int newWidth, int newHeight);
Bitmap Resize(const Bitmap &in, double factor);

// Blur
Bitmap MediumBlur(const Bitmap &bmp, int R);
Bitmap GaussianBlur(const Bitmap &bmp, int R, double sigma);

// Edge Detection
Bitmap Sobel(const Bitmap &in);
Bitmap Robert(const Bitmap &in);
Bitmap Prewitt(const Bitmap &in);
Bitmap LaplacianOfGaussian(Bitmap bmp);
Bitmap Kirsch(const Bitmap &in, Direction dir);

// Color Adjustment
Bitmap ContrastAdjust(const Bitmap& bmp, int value);
Bitmap NearestColour(const Bitmap& bmp, Color colours[], int colorCount);
Bitmap LevelsAdjust(const Bitmap& bmp, unsigned char input[], unsigned char output[]);
Bitmap ErrorDiffuse(const Bitmap& bmp, Color colours[], int colorCount);
Bitmap ExposureAdjust(const Bitmap& bmp, double value);

//Draw Image Histogram
int RedChannel(const Color& c, double factor);
int GreenChannel(const Color& c, double factor);
int BlueChannel(const Color& c, double factor);
int LuminanceChannel(const Color& c, double factor);
void DrawHistogram(const Bitmap& bmp, int(*channel)(const Color&, double));

// Grayscale: DONE
void convertGrayScale(const Bitmap &bmp, Bitmap &out);
void holdAColor(const Bitmap &bmp, Bitmap &out, ColorCanHold clor);