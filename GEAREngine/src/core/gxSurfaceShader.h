#ifndef GXSURFACESHADER_H
#define GXSURFACESHADER_H

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>

class gxSurfaceShader
{
public:
	bool findQuat(std::string::const_iterator& start, std::string::const_iterator& end, bool bStrict=true);
	bool findOpeningRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findClosingRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findOpeningCurlyBrace(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findClosingCurlyBrace(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findComma(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findEqualTo(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findAnyValidCharForName(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findAnyValidAlphaNumeric(std::string::const_iterator& start, std::string::const_iterator& end);
	bool findAnyValidNumeric(std::string::const_iterator& start, std::string::const_iterator& end);
	bool parseKeyWord(std::string::const_iterator& start, std::string::const_iterator& end, std::string& name);
	bool parseArgInsideRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end, std::vector<std::string>& args, int argCount);
	bool parseNameInsideQuats(std::string::const_iterator& start, std::string::const_iterator& end, std::string& name);
	bool parseColorProperty(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseMainTexProperty(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseProperties(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseSubShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parseShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);
	bool parse(std::string::const_iterator& start, std::string::const_iterator& end, int& depth);

public:
	bool load(const char* filename);
};
#endif