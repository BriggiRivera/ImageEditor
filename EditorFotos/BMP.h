#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "PixelRGB.h"

using namespace std;

namespace brg
{
	class BMP
	{
	private:

		word type;
		dword size;
		word reservedField1;
		word reservedField2;
		dword offset;

		dword size2;  //specifies the number of bytes required by the struct
		int width;  //specifies width in pixels
		int height;  //species height in pixels
		word planes; //specifies the number of color planes, must be 1
		word bitCount; //specifies the number of bit per pixel
		dword compression;//spcifies the type of compression
		dword sizeImage;  //size of image in bytes
		int xPixelsPerMeter;  //number of pixels per meter in x axis
		int yPixelsPerMeter;  //number of pixels per meter in y axis
		dword colorUsed;  //number of colors used by th ebitmap
		dword colorImportant;  //number of colors that are important
		int extraBytes; //Bytes agregados al final de cada linea
		byte* dataImage;
	
	public:
		BMP(const char* fileName);

		~BMP();

		int getWidth();

		int getHeight();

		PixelRGB& getPixel(int x, int y);
	};
}