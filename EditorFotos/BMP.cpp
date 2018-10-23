#include "stdafx.h"
#include "BMP.h"

using namespace brg;

BMP::BMP(const char* fileName)
{
	fstream file(fileName, ios::in | ios::app | ios::binary);
	if (!file.fail())
	{
		file.read((char*)&type, sizeof(word));
		file.read((char*)&size, sizeof(dword));
		file.read((char*)&reservedField1, sizeof(word));
		file.read((char*)&reservedField2, sizeof(word));
		file.read((char*)&offset, sizeof(dword));

		if (type == 19778)
		{
			file.read((char*)&size2, sizeof(dword));  //specifies the number of bytes required by the struct
			file.read((char*)&width, sizeof(int));  //specifies width in pixels
			file.read((char*)&height, sizeof(int));  //species height in pixels
			file.read((char*)&planes, sizeof(word)); //specifies the number of color planes, must be 1
			file.read((char*)&bitCount, sizeof(word)); //specifies the number of bit per pixel
			file.read((char*)&compression, sizeof(dword)); //spcifies the type of compression
			file.read((char*)&sizeImage, sizeof(dword));  //size of image in bytes
			file.read((char*)&xPixelsPerMeter, sizeof(int));  //number of pixels per meter in x axis
			file.read((char*)&yPixelsPerMeter, sizeof(int));  //number of pixels per meter in y axis
			file.read((char*)&colorUsed, sizeof(dword));  //number of colors used by th ebitmap
			file.read((char*)&colorImportant, sizeof(dword));  //number of colors that are important

			file.seekg(offset);
			dataImage = new byte[this->sizeImage];
			file.read((char*)dataImage, this->sizeImage);

			extraBytes = (sizeImage - width*height * 3) / height;
		}
	}
	file.close();
}

BMP::~BMP(){
	if (dataImage != NULL)
		delete[] dataImage;
}

int BMP::getWidth()
{
	return width;
}

int BMP::getHeight()
{
	return height;
}

PixelRGB& BMP::getPixel(int x, int y)
{
	return *(PixelRGB*)&dataImage[(y * (width * 3 + extraBytes)) + x * 3];
}

void BMP::setPixel(PixelRGB& pixel, int x, int y)
{
	dataImage[(y * (width * 3 + extraBytes)) + x * 3] = pixel.b;
	dataImage[(y * (width * 3 + extraBytes)) + x * 3 + 1] = pixel.g;
	dataImage[(y * (width * 3 + extraBytes)) + x * 3 + 2] = pixel.r;
}