#ifndef PIXEL_RGB_H
#define PIXEL_RGB_H

#include "stdafx.h"

namespace brg
{
	struct PixelRGB
	{
		byte b;
		byte g;
		byte r;

		PixelRGB(byte b, byte g, byte r) {
			this->b = b;
			this->r = r;
			this->g = g;
		}
	};
}

#endif