#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"
#include "EditorFotos.h"
#include "EditorFotosDlg.h"
#include "afxdialogex.h"
#include "BMP.h"
#include "PixelRGB.h"
#include <algorithm>
#include <vector>

brg::PixelRGB getMedian(brg::BMP* image, int x, int y, int neighbourhood) {
	int iniX = x - neighbourhood / 2;
	int finX = x + neighbourhood / 2;
	int iniY = y - neighbourhood / 2;
	int finY = y + neighbourhood / 2;

	vector<int> r;
	vector<int> g;
	vector<int> b;

	for (int y = iniY; y <= finY ;y++)
	{
		for (int x = iniX; x <= finX; x++)
		{
			brg::PixelRGB& pixel = image->getPixel(x, y);
			r.push_back(pixel.r);
			g.push_back(pixel.g);
			b.push_back(pixel.b);
		}
	}

	sort(r.begin(), r.end());
	sort(g.begin(), g.end());
	sort(b.begin(), b.end());

	int pos = neighbourhood * neighbourhood / 2;

	return brg::PixelRGB(b[pos], g[pos], r[pos]);
}

#endif