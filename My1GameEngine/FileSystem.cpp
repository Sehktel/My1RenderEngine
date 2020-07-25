#include "FileSystem.h"
#include "stb_image.h"

int FileSystem::ReadFromFileToString(const char* PathToFile, std::string* ResultString)
{
    std::ifstream MyInputFile(PathToFile, std::fstream::out | std::fstream::app); // create file or open 

    // check for errors while opening
    if (!MyInputFile)
    {
        std::cout << "Problems!" << std::endl;
        MyInputFile.close();
        return -1;
    }

    // read file to string
    char CurrentCharacter;
    std::string TempString;

    while (std::getline(MyInputFile, TempString))
    {
        *ResultString = *ResultString + TempString + '\n';
    }
    *ResultString = *ResultString + '\0';

    MyInputFile.close();
    return 0;
}

unsigned char* FileSystem::ReadRawDataFromImage(const char* PathToImage, int* ImageWidth, int* ImageHeight, int* NumberOfImagesChannel)
{
    return stbi_load(PathToImage, ImageWidth, ImageHeight, NumberOfImagesChannel, 0);
}
