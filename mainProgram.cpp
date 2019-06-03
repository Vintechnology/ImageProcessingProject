#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
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

void GetHelp(const char* command)
{
	std::string output ="";
	std::string check;
	check.append(command);
	check = "#" + check;
	
	std::ifstream helpInput;
	helpInput.open("usage.txt");
	if (helpInput.fail())
		throw std::string("Unable to find usage.txt in your computer");
	while (strcmp(output.c_str(), check.c_str()) != 0)
	{
		getline(helpInput, output);
	}
	getline(helpInput, output, '#');
	std::cout << output;
}

void HelpParser(LinkedStrList* arg)
{
	while (arg->head != nullptr)
	{
		const char* command = LinkedList::Get(arg, arg->head);
		if (dictionary.count(command) == 0)
		{
			throw std::string("No such feature \"") + LinkedList::Get(arg, arg->head) + "\"\n";
		}
		GetHelp(command);
	}
}

void InitDictionary()
{
	// Utility
	dictionary.insert(std::make_pair("help", HelpParser));				 //usable
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
	dictionary.insert(std::make_pair("laplaction", LaplacianOfGaussianParser)); //usable
	dictionary.insert(std::make_pair("kirsch", KirschParser));			//usable
	// Blur
	dictionary.insert(std::make_pair("blur", BlurParser));               // usable
	// Color Adjust
	dictionary.insert(std::make_pair("contrast", ContrastAdjustParser)); //usable
	dictionary.insert(std::make_pair("nearest", NearestColourParser));
	dictionary.insert(std::make_pair("levels", LevelsAdjustParser));
	dictionary.insert(std::make_pair("diffuse", ErrorDiffuseParser));
	dictionary.insert(std::make_pair("exposure", ExposureAdjustParser)); //usable
	// Grayscale
	dictionary.insert(std::make_pair("grayscale", GrayscaleParser));     // usable
}

void EvaluateCommand(char** arg, int length) {
	if (length == 1) 
	{
		throw std::string("No command given.\n");
	}
	if (dictionary.count(arg[1]) == 0 && strcmp(arg[1],"help") !=0 && strcmp(arg[1],"-version") != 0) 
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
	if (strcmp(arg[1], "list") == 0)
	{
		std::cout << "--BASIC--\nrotate\tflip\tcrop\tscale\tresize" << std::endl;
		std::cout << "--EDGE DETECTION--\nsobel\trobert\tprewitt\tkirsch\tlaplacian" << std::endl;
		std::cout << "--BLUR--\nblur" << std::endl;
		std::cout << "--COLOR ADJUST--\ncontrast\tnearest\tlevels\tdiffuse\texposure" << std::endl;
		std::cout << "--GRAYSCALE--\ngrayscale" << std::endl;
	}
	else if (strcmp(arg[1], "-version") == 0)
	{
		std::cout << "impr - version 0.0.1" << std::endl;
	}
	else
	{
		ArgumentParser parseFunc = dictionary.at(arg[1]);

		LinkedStrList *argList = new LinkedStrList;
		LinkedList::Init(argList);

		for (int i = 2; i < length; i++) {
			LinkedList::AddTail(argList, arg[i]);
		}

		parseFunc(argList);
		LinkedList::Delete(argList);
	}
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

