#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "util/LinkedList.h"
#include "Parsers.h"

std::string VERSION = "0.0.1";

struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return std::strcmp(a, b) < 0;
	}
};
std::map<const char*, ArgumentParser, cmp_str> dictionary;


void PrintHeader()
{
	std::cout << " _____" << std::endl;
	std::cout << "|_   _|________  ____  ___  |  Command structure : impr + <function name> + <parameter value>" << std::endl;
	std::cout << "  | | |  _   _ \\|  _ \\|  _| |  Type \"impr list\" to see the all function" << std::endl;
	std::cout << " _| |_| | | | | | |_) | |   |  For help, type \"impr help + <function name>\"" << std::endl;
	std::cout << "|_____|_| |_| |_|  __/|_|   |" << std::endl;
	std::cout << "                | |         |" << std::endl;
	std::cout << "                |_|         |  Version " << VERSION << std::endl;
}

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
	helpInput.close();
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
	dictionary.insert(std::make_pair("nearest", NearestColourParser));   //usable
	dictionary.insert(std::make_pair("levels", LevelsAdjustParser));	//usable
	dictionary.insert(std::make_pair("diffuse", ErrorDiffuseParser));	//usable
	dictionary.insert(std::make_pair("exposure", ExposureAdjustParser)); //usable
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
		throw std::string("No value given. Please specify the filename of your image.\n");
	}
}

void handlingCommand(char** arg, int length) 
{
	if (length == 1)
	{
		PrintHeader();
	}
	else if (strcmp(arg[1], "list") == 0)
	{
		std::cout << "--BASIC--\nrotate    flip    crop    scale    resize\n" << std::endl;
		std::cout << "--EDGE DETECTION--\nsobel    robert    prewitt    kirsch    laplacian\n" << std::endl;
		std::cout << "--BLUR--\nblur\n" << std::endl;
		std::cout << "--COLOR ADJUST--\ncontrast    nearest    levels    diffuse    exposure\n" << std::endl;
		std::cout << "--GRAYSCALE--\ngrayscale\n" << std::endl;
	}
	else if (strcmp(arg[1], "-version") == 0)
	{
		std::cout << "impr - version " << VERSION << std::endl;
	}
	else
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
}

int main(int argc, char* argv[]) 
{
	try {
		InitDictionary();
		handlingCommand(argv, argc);
	}
	catch (std::string str) {
		std::cout << str;
	}

	return 0;
}

