#pragma once
#include <string>
#include <fstream> // class declarations for file stream objects
#include <iostream>
#include <sstream>

class FileSystem
{
public:
	int ReadFromFileToString(const char* PathToFile, std::string* ResultString);
};
