#include <iostream>
#include <string>
#include <map>
#include "util/LinkedList.h"
#include "Parsers.h"

struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};
std::map<const char*, ArgumentParser, cmp_str> dictionary;



void InitDictionary()
{
	// Basic
	dictionary.insert(std::make_pair("rotate", RotateParser));           // usable
	dictionary.insert(std::make_pair("flip", FlipParser));               // usable
	dictionary.insert(std::make_pair("crop", CropParser));               // usable
	dictionary.insert(std::make_pair("scale", ScaleParser));             // usable
	dictionary.insert(std::make_pair("resize", ResizeParser));           // usable
	// Edge Detection
	dictionary.insert(std::make_pair("sobel", SobelParser));             // usable
	dictionary.insert(std::make_pair("robert", RobertParser));           // usable
	dictionary.insert(std::make_pair("prewitt", PrewittParser));         // usable
	// Blur
	dictionary.insert(std::make_pair("blur", BlurParser));               // usable
	// Color Adjust
	dictionary.insert(std::make_pair("contrast", ContrastAdjustParser));
	dictionary.insert(std::make_pair("nearest", NearestColourParser));
	dictionary.insert(std::make_pair("levels", LevelsAdjustParser));
	dictionary.insert(std::make_pair("diffuse", ErrorDiffuseParser));
	dictionary.insert(std::make_pair("exposure", ExposureAdjustParser));
	// Grayscale
	dictionary.insert(std::make_pair("grayscale", GrayscaleParser));     // usable
}

void EvaluateCommand(char** arg, int length) {
	if (length == 1) 
	{
		throw std::string("No command given.\n");
	}
	if (dictionary.count(arg[1]) == 0) 
	{
		throw std::string("No such command \"") + arg[1] + "\".\n";
	}

	if (length == 2)
	{
		throw std::string("No path given. Please specify the filename of your image.\n");
	}
}

void handlingCommand(char** arg, int length) 
{
	EvaluateCommand(arg, length); 
	ArgumentParser parseFunc = dictionary.at(arg[1]);

	LinkedStrList *argList = new LinkedStrList;
	LinkedList::Init(argList);

	for (int i = 2; i < length; i++) {
		LinkedList::AddTail(argList, arg[i]);
	}

	parseFunc(argList);
	LinkedList::Delete(argList);
}

void main(int argc, char* argv[]) 
{
	try {
		InitDictionary();
		handlingCommand(argv, argc);
	}
	catch (std::string str) {
		std::cout << str;
	}

}

