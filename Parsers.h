#pragma once
#include "bitmap/Bitmap.h"
#include "featureList.h"
#include "util/LinkedList.h"
#include "util/TypeConvert.h"
#include <string>

#define DEFAULT_OUTPUT_FILENAME "out.bmp"
#define FILE_FORMAT ".bmp"

typedef void(*ArgumentParser)(LinkedStrList* l);

void EvaluateFilename(const char* fn) {
	int length = strlen(fn);
	if (length <= 4)
		throw std::string("Invalid filename: \"") + fn + "\"\n";
	for (int i = 0; i < 4; i++) 
	{
		if (fn[length - 4 + i]!= FILE_FORMAT[i])
			throw std::string("Invalid filename: \"") + fn + "\"\n";
	}
}

void CheckLeftovers(LinkedStrList* arg) 
{
	if (arg->size != 0) {
		throw std::string("Unknown argument: \"") + arg->head->value + "\".\n";
	}
}

bool ParseOption(LinkedStrList* arg, const char* option,const char* &value) 
{
	StrNode* current = arg->head;
	while (current != nullptr)
	{
		if (strcmp(current->value, option) == 0)
		{
			if (current->next == nullptr)
				throw std::string("No value given after option \"") + option + "\"\n";
			else
			{
				StrNode* valueNode = current->next;
				value = LinkedList::Get(arg, valueNode);
				// Remove this option from the list after processing
				LinkedList::Get(arg, current);
				return 1;
			}
		}
		current = current->next;
	}
	return 0;
}

bool ParseOption(LinkedStrList* arg, const char* option) {
	StrNode* current = arg->head;
	while (current != nullptr)
	{
		if (strcmp(current->value, option) == 0)
		{
			// Remove this option from the list after processing
			LinkedList::Get(arg, current);
			return 1;
		}
		current = current->next;
	}
	return 0;

}

void GetInputBitmap(LinkedStrList* arg, Bitmap &bmp) {
	const char* infName = LinkedList::Get(arg, arg->head);
	if (!LoadBitmap(infName, bmp))
	{
		throw std::string("Cannot open file \"") + infName + "\"\n";
	}
}

void GetOutputFilename(LinkedStrList* arg, const char* &fName)
{
	if (!ParseOption(arg, "-o", fName))
	{
		fName = DEFAULT_OUTPUT_FILENAME;
	}
	EvaluateFilename(fName);
}

void SaveOutput(const char* outfName, Bitmap &bmp) {
	if (SaveBitmap(outfName, bmp))
		std::cout << "File \"" << outfName << "\" saved.\n";
	else
		throw std::string("No such filename: \"") + outfName + "\".\n";
}

// <<< MAIN PARSER SECTION >>>

// -- BASIC -- 
/*
	Use case:
	<app-name> rotate <filename> [args..]
	-o <string> : output image name (default = "out.bmp")

	-degree <integer>: integer of degree to turn the image.(default = 0) 
			+ positive : turn left
			+ negative : turn right
	
	-l : rotate 90 degree to left

	-r : rotate 90 degree to right

	-d : rotate 180 degree 
*/
void RotateParser(LinkedStrList* arg) 
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);

	const char* temp;
	bool rLeft, rRight, rDown, rDegree;
	int degree;

	rLeft = ParseOption(arg, "-l");
	rRight = ParseOption(arg, "-r");
	rDown = ParseOption(arg, "-d");
	rDegree = ParseOption(arg, "-degree", temp);

	int total = rLeft + rRight + rDown + rDegree;
	if (total > 1)
		throw std::string("There are too many rotate option. Please choose only one. \n");
	else if (total == 0)
		throw std::string("No option found.\nPlease use one of these: -l, -r, -d, -degree <integer>\n");
	if(rDegree)
		degree = ParseInt(temp);

	CheckLeftovers(arg);


	Bitmap result;
	std::cout << "Doing rotation...\n";
	if (rLeft)
		result = Turn90Degree(bm);
	else if (rRight)
		result = Turn270Degree(bm);
	else if (rDown)
		result = Turn180Degree(bm);
	else if (rDegree)
		result = Rotate(bm, degree);
	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
};

/*
	Use case:
	<app-name> flip <filename> [args..]
	-o <string> : output image name (default = "out.bmp")

	-h : flip horizontal

	-v : flip vertical
*/
void FlipParser(LinkedStrList* arg) 
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);

	bool flipH, flipV;

	flipH = ParseOption(arg, "-h");
	flipV = ParseOption(arg, "-v");
	if (flipH || flipV)
		throw std::string("No option given.\nPlease use any of these: -h , -v\n");
	CheckLeftovers(arg);


	Bitmap result;
	std::cout << "Flipping...\n";
	if (flipH && flipV)
		result = FlipAll(bm);
	else if (flipH)
		result = FlipLR(bm);
	else if (flipV)
		result = FlipUD(bm);
	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}

/*
	Use case:
	<app-name> crop <filename> [args..]
	-o <string> : output image name (default = "out.bmp")

	Define top, bottom, left, right of a box which you want to crop
	-t <double> : a number between 0.0 (downmost) and 1.0 (topmost). (default = 1.0)
	-b <double> : a number between 0.0 (downmost) and 1.0 (topmost). (default = 0.0)

	-r <double> : a number between 0.0 (leftmost) and 1.0 (rightmost). (default = 1.0)
	-l <double> : a number between 0.0 (leftmost) and 1.0 (rightmost). (default = 0.0)
*/
void CheckBoundary(const char* option, double value)
{
	if (value > 1.0 || value < 0.0)
		throw std::string("Value of \"") + option + "\" is out of range. [0.0,1.0]\n";
}
void CropParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);

	const char* temp;
	double left = 0.0, right = 1.0, top = 1.0, down = 0.0;

	if (ParseOption(arg, "-l", temp))
	{
		left = ParseDouble(temp);
		CheckBoundary("-l", left);
	}

	if (ParseOption(arg, "-r", temp))
	{
		right = ParseDouble(temp);
		CheckBoundary("-r", right);
	}

	if (ParseOption(arg, "-t", temp))
	{
		top = ParseDouble(temp);
		CheckBoundary("-t", top);
	}

	if (ParseOption(arg, "-d", temp))
	{
		down = ParseDouble(temp);
		CheckBoundary("-d", down);
	}
	CheckLeftovers(arg);

	if (down > top || left > right)
		throw std::string("Illegal box values: top must larger than down and right must larger than left\n");

	Bitmap result;
	std::cout << "Cropping...\n";
	result = Crop(bm, left*bm.width, (1.0 - top)*bm.height, right*bm.width, (1.0 - down)*bm.height);
	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}

/*
	Use case:
	<app-name> scale <filename> [args..]
	-o <string> : output image name (default = "out.bmp")

	-f <double> : factor
*/
void ScaleParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);
	const char* temp;
	double factor;
	if (ParseOption(arg, "-f", temp))
	{
		factor = ParseDouble(temp);
	}
	else
	{
		throw std::string("No scale factor given. Please specify with \"-f\" option.\n");
	}

	CheckLeftovers(arg);

	Bitmap result;
	std::cout << "Scaling...\n";
	result = Resize(bm, factor);
	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}

/*
	Use case:
	<app-name> resize <filename> [args..]
	-o <string> : output image name (default = "out.bmp")

	-w <integer> : resize width. (default = original width)
	-h <integer> : resize height. (default = original height)
*/
void ResizeParser(LinkedStrList* arg) 
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);

	const char* temp;
	int width = bm.width;
	int height = bm.height;
	if (ParseOption(arg, "-w", temp))
	{
		width = ParseInt(temp);
		if (width <= 0)
		{
			throw std::string("Invalid width size: width cannot be zero or a negative number./n");
		}
	}

	if (ParseOption(arg, "-h", temp))
	{
		height = ParseInt(temp);
		if (height <= 0)
		{
			throw std::string("Invalid height size: height cannot be zero or a negative number.\n");
		}
	}

	if (height == bm.height&&width == bm.width)
	{
		throw std::string("The resize width and height are the same with original image. No need to resize.\n");
	}

	CheckLeftovers(arg);

	Bitmap result;
	std::cout << "Resizing...\n";
	result = Resize(bm, width, height);
	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}
// -- EDGE DETECTION --

/*
	Use case:
	<app-name> sobel <filename> [args..]
	-o <string> : output image name (default = "out.bmp")
*/
void SobelParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char * outfName = nullptr;
	GetOutputFilename(arg, outfName);
	CheckLeftovers(arg);

	std::cout << "Doing Sobel edge detection..." << std::endl;
	Bitmap result = Sobel(bm);
	std::cout << "Completed!" << std::endl;
	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}

/*
	Use case:
	<app-name> robert <filename> [args..]
	-o <string> : output image name (default = "out.bmp")
*/
void RobertParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char * outfName = nullptr;
	GetOutputFilename(arg, outfName);
	CheckLeftovers(arg);

	std::cout << "Doing Robert edge detection..." << std::endl;
	Bitmap result = Robert(bm);
	std::cout << "Completed!" << std::endl;
	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}

/*
	Use case:
	<app-name> prewitt <filename> [args..]
	-o <string> : output image name (default = "out.bmp")
*/
void PrewittParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char * outfName = nullptr;
	GetOutputFilename(arg, outfName);
	CheckLeftovers(arg);

	std::cout << "Doing Prewitt edge detection..." << std::endl;
	Bitmap result = Sobel(bm);
	std::cout << "Completed!" << std::endl;
	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}

// -- BLURRING --

/*
	Use case:
	<app-name> blur <filename> [args..]
	-o <string> : output image name (default = "out.bmp")
	-m : medium blur (default option)

	-g : gaussian blur
		-sigma <double> : blur variance (default = 2.0)

	-r <integer> : blur radius (default = 1)

*/
void BlurParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	bool useGaussian = ParseOption(arg, "-g");
	bool useMedium = ParseOption(arg, "-m");
	if (useGaussian && useMedium)
		throw std::string("Cannot use both Medium and Gaussian. Please choose one.\n");

	const char* temp = nullptr;
	int r = 1;
	if (ParseOption(arg, "-r", temp))
	{
		r = ParseInt(temp);
	}

	double sigma = 2.0;
	if (useGaussian && ParseOption(arg, "-sigma", temp))
	{
		sigma = ParseDouble(temp);
	}
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);
	CheckLeftovers(arg);

	std::cout << "Doing blur..." << std::endl;
	Bitmap result;
	if (useGaussian)
		result = GaussianBlur(bm, r, sigma);
	else
		result = MediumBlur(bm, r);
	std::cout << "Completed!" << std::endl;

	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}

// -- COLOR ADJUSTMENT -- 

/*
	Use case:
	<app-name> contrast <filename> [args..]
	-o : output image name (default = "out.bmp")
*/
void ContrastAdjustParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);


	CheckLeftovers(arg);

	Bitmap result;
	std::cout << "Cropping...\n";

	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);

}

/*
	Use case:
	<app-name> nearest <filename> [args..]
	-o : output image name (default = "out.bmp")
*/
void NearestColourParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);


	CheckLeftovers(arg);

	Bitmap result;
	std::cout << "Cropping...\n";

	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);

}

/*
	Use case:
	<app-name> levels <filename> [args..]
	-o : output image name (default = "out.bmp")
*/
void LevelsAdjustParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);


	CheckLeftovers(arg);

	Bitmap result;
	std::cout << "Cropping...\n";

	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);

}

/*
	Use case:
	<app-name> diffuse <filename> [args..]
	-o : output image name (default = "out.bmp")
*/
void ErrorDiffuseParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);


	CheckLeftovers(arg);

	Bitmap result;
	std::cout << "Cropping...\n";

	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);

}

/*
	Use case:
	<app-name> exposure <filename> [args..]
	-o : output image name (default = "out.bmp")
*/
void ExposureAdjustParser(LinkedStrList* arg)
{
	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);


	CheckLeftovers(arg);

	Bitmap result;
	std::cout << "Cropping...\n";

	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);

}
// -- GRAYSCALE -- 

/*
	Use case:
	<app-name> grayscale <filename> [args..]
	-o : output image name (default = "out.bmp")

	-hold <string> : don't grayscale a color.
					 Any one of these value: "red", "yellow", "green", "blue", "purple" 
*/
void GrayscaleParser(LinkedStrList* arg)
{

	Bitmap bm;
	GetInputBitmap(arg, bm);
	const char* outfName = nullptr;
	GetOutputFilename(arg, outfName);

	const char* c;
	bool hold = false;
	ColorCanHold color;
	if (ParseOption(arg, "-hold", c))
	{
		hold = true;
		if (strcmp(c, "red") == 0)
		{
			color = ColorCanHold::RED;
		}
		else if (strcmp(c, "yellow") == 0)
		{
			color = ColorCanHold::YELLOW;
		}
		else if (strcmp(c, "green") == 0)
		{
			color = ColorCanHold::GREEN;
		}
		else if (strcmp(c, "blue") == 0)
		{
			color = ColorCanHold::BLUE;
		}
		else if (strcmp(c, "purple") == 0)
		{
			color = ColorCanHold::PURPLE;
		}
		else
		{
			throw std::string("Unknown color value \"") + c + "\"\n";
		}
	}

	CheckLeftovers(arg);

	Bitmap result;
	std::cout << "Doing grayscale...\n";
	if (hold)
	{
		holdAColor(bm, result, color);
	}
	else 
	{
		convertGrayScale(bm, result);
	}
	std::cout << "Complete!" << std::endl;


	SaveOutput(outfName, result);
	DisposeBitmap(bm);
	DisposeBitmap(result);
}