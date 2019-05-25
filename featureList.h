#pragma once
#include "Basic.h"
#include "Blur.h"
#include "EdgeDetection.h"

// Basic
Bitmap Turn90Degree(Bitmap bmp);
Bitmap Turn180Degree(Bitmap bmp);
Bitmap Turn270Degree(Bitmap bmp);
Bitmap Crop(Bitmap bmp, int xA, int yA, int xB, int yB);
Bitmap FlipLR(const Bitmap &in);
Bitmap FlipUD(const Bitmap &in);
Bitmap FlipAll(const Bitmap &in);
Bitmap Resize(const Bitmap &in, int newWidth, int newHeight);
Bitmap Resize(const Bitmap &in, double factor);
// Blur
Bitmap MediumBlur(Bitmap bmp, int R);
Bitmap GaussianBlur(Bitmap bmp, int R, double sigma);
// Edge Detection
Bitmap Sobel(const Bitmap &in);
Bitmap Robert(const Bitmap &in);