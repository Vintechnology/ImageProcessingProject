#include <iostream>
#include <string>
#include <map>
#include "Parsers.h"

std::map<const char*, ArgumentParser> dictionary;

void InitDictionary()
{
	dictionary.insert(std::make_pair("sobel", SobelParser));
}

void main(int argc, char* argv[]) 
{
	/*
	for (int i = 0; i < argc; i++) {
		std::cout << argv[i] << std::endl;
	}
	*/
	if (argc == 1) {
		std::cout << "No command given" << std::endl;
		return;
	}
	if (dictionary.count(argv[2]) == 0) {
		std::cout << "No such command " << argv[2] << std::endl;
		return;
	}
	ArgumentParser parseFunc = dictionary.at(argv[2]);
	int offIndex;
	Bitmap result = parseFunc(argv, argc, offIndex);

}

