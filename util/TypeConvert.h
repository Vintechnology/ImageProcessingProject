#pragma once
#include <string>

int ParseInt(const char* value) 
{
	int length = strlen(value);
	int iValue = 0;
	int sign = 1;
	for (int i = 0; i < length; i++)
	{
		if (value[i] == '-')
		{
			if (i == 0)
				sign = -1;
			else
				throw std::string("Parse error: cannot convert \"") + value + "\" to int.\n";
			continue;
		}
		int parseVal = value[i] - '0';
		if (parseVal < 0 || parseVal > 9)
			throw std::string("Parse error: cannot convert \"") + value + "\" to int.\n";
		iValue = iValue * 10 + parseVal;
	}
	return sign * iValue;
}

double ParseDouble(const char* value)
{
	int length = strlen(value);
	double dValue = 0.0;
	double scale = 10.0;
	double frac = 1.0;
	bool passed = false;
	for (int i = 0; i < length; i++)
	{
		if (value[i] == '.') {
			if (passed)
				throw std::string("Invalid double: \"") + value + "\".\n";
			scale = 1.0;
			frac = 0.1;
			passed = true;
			continue;
		}
		int parseVal = value[i] - '0';
		if (parseVal < 0 || parseVal > 9)
			throw std::string("Parse error: cannot convert \"") + value + "\" to double.\n";
		dValue = dValue * scale + parseVal * frac;
		frac *= 0.1;
	}
	return dValue;
}