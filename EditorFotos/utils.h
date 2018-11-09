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
#define cimg_use_jpeg
#include "CImg.h"
#include <cstdio>

using namespace std;
using namespace cimg_library;

void operacionConvolucion(string input, string output, vector<vector<float>>& mask, brg::BMP*& bmpInput, brg::BMP*& bmpOutput)
{
	CImg<unsigned char> imageInput(input.c_str());
	imageInput.resize(512, 512);
	imageInput.save(output.c_str());
	imageInput.save(input.c_str());

	bmpInput = new brg::BMP(input.c_str());
	bmpOutput = new brg::BMP(output.c_str());

	int maskMiddle = mask.size() / 2;
	int x0 = maskMiddle;
	int x1 = bmpInput->getWidth() - maskMiddle;
	int y0 = maskMiddle;
	int y1 = bmpInput->getHeight() - maskMiddle;

	for (int y = y0; y < y1; y++)
	{
		for (int x = x0; x < x1; x++)
		{
			int r = 0,g = 0,b = 0;
			brg::PixelRGB& center = bmpInput->getPixel(x, y);
			int i0 = x - maskMiddle;
			int i1 = x + maskMiddle;
			int j0 = y - maskMiddle;
			int j1 = y + maskMiddle;
			
			for (int i = i0, n = 0; i <= i1; i++, n++)
			{
				for (int j = j0, m = 0; j <= j1; j++, m++)
				{
					brg::PixelRGB& pixel = bmpInput->getPixel(i, j);
					r += mask[n][m] * pixel.r;
					g += mask[n][m] * pixel.g;
					b += mask[n][m] * pixel.b;
				}
			}
			b = b < 0 || b > 255 ? center.b : b;
			r = r < 0 || r > 255 ? center.r : r;
			g = g < 0 || g > 255 ? center.g : g;
			bmpOutput->setPixel(brg::PixelRGB(b, g, r), x, y);
		}
	}
}

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

void borrarImagen(string path)
{
	remove(path.c_str());
}

#endif