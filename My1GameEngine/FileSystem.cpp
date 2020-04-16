#include "FileSystem.h"

std::string FileSystem::ReadFileToString(const char* FileSystemPathToFile)
{
	std::string Line;
	std::ifstream InputFile;
	InputFile.open(FileSystemPathToFile); // open this file
	std::stringstream StrStream; // stream of strings
	StrStream << InputFile.rdbuf(); // read full file
	Line = StrStream.str(); // write full stream of strings into one string
	return Line;
}
