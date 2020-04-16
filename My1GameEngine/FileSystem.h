#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
class FileSystem
{
	public:
		std::string ReadFileToString(const char* FileSystemPathToFile);
};

