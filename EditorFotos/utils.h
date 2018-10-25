#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"
#include "EditorFotos.h"
#include "EditorFotosDlg.h"
#include "afxdialogex.h"
#include "BMP.h"
#include "PixelRGB.h"
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

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

int** crearMatriz(int width, int height)
{
	int** matriz = new int*[width];
	for (int index = 0; index < width; index++) {
		matriz[index] = new int[height];
	}
	return matriz;
}

int** crearMatrizEscalaGrises(brg::BMP* image) {
	int** matriz = crearMatriz(image->getWidth(), image->getHeight());

	for (int y = 0; y < image->getHeight() ;y++)
	{
		for (int x = 0; x < image->getWidth(); x++) {
			brg::PixelRGB& pixel = image->getPixel(x, y);
			matriz[x][y] = ((0.3 * pixel.r) + (0.59 * pixel.g) + (0.11 * pixel.b));//sqrt(pixel.r * pixel.r + pixel.g * pixel.g + pixel.b * pixel.b);
		}
	}

	return matriz;
}

void destruirMatriz(int** matriz, int width)
{
	for (int index = 0; index < width; index++) {
		delete[] matriz[index];
	}
	delete[] matriz;
}

#endif