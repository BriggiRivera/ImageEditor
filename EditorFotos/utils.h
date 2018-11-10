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

int** crearMatriz(int width, int height)
{
	int** matriz = new int*[width];
	for (int index = 0; index < width; index++) {
		matriz[index] = new int[height];
	}
	return matriz;
}

void BilinealInterpolationOnePixel(brg::BMP*& bmpOutput, int** coord, int x, int y)
{
	int x0 = x, x1 = x, y0 = y, y1 = y;
	while (coord[x0][y] == -1)	x0--;
	while (coord[x1][y] == -1)	x1++;
	while (coord[x][y0] == -1)	y0--;
	while (coord[x][y1] == -1)	y1++;

	double Hfactor = 1.0 * (x - x0) / (x1 - x0);
	double Vfactor = 1.0 * (y - y0) / (y1 - y0);
	int hr = bmpOutput->getPixel(x0, y).r + (bmpOutput->getPixel(x1, y).r - bmpOutput->getPixel(x0, y).r) * Hfactor;
	int hg = bmpOutput->getPixel(x0, y).g + (bmpOutput->getPixel(x1, y).g - bmpOutput->getPixel(x0, y).g) * Hfactor;
	int hb = bmpOutput->getPixel(x0, y).b + (bmpOutput->getPixel(x1, y).b - bmpOutput->getPixel(x0, y).b) * Hfactor;
	int vr = bmpOutput->getPixel(x, y0).r + (bmpOutput->getPixel(x, y1).r - bmpOutput->getPixel(x, y0).r) * Vfactor;
	int vg = bmpOutput->getPixel(x, y0).g + (bmpOutput->getPixel(x, y1).g - bmpOutput->getPixel(x, y0).g) * Vfactor;
	int vb = bmpOutput->getPixel(x, y0).b + (bmpOutput->getPixel(x, y1).b - bmpOutput->getPixel(x, y0).b) * Vfactor;

	bmpOutput->setPixel(brg::PixelRGB((hb + vb) / 2, (hg + vg) / 2, (hr + vr) / 2), x, y);
	coord[x][y] = 1;
}

void projectiveTransformationBilinealInterpolation(string input, string output, brg::BMP*& bmpInput, brg::BMP*& bmpOutput, vector<vector<double>>& P)
{
	CImg<unsigned char> imageInput(input.c_str());
	imageInput.resize(512, 512);
	imageInput.save(output.c_str());
	imageInput.save(input.c_str());

	bmpInput = new brg::BMP(input.c_str());
	bmpOutput = new brg::BMP(output.c_str());

	int** coord = crearMatriz(bmpInput->getWidth(), bmpInput->getHeight());

	/* Projection Matrix: P
	[	a11	a12	b1
	a21	a22	b2
	c1	c2	1	]
	*/

	double a11 = P[0][0], a12 = P[0][1], b1 = P[0][2];
	double a21 = P[1][0], a22 = P[1][1], b2 = P[1][2];
	double c1 = P[2][0], c2 = P[2][1];
	int x_, y_;
	
	for (int x = 0; x < bmpInput->getWidth(); x++)
	{
		for (int y = 0; y < bmpInput->getHeight(); y++)
		{
			coord[x][y] = -1;
			bmpOutput->setPixel(brg::PixelRGB(0, 0, 0), x, y);
		}
	}	

	for (int y = 0; y<bmpInput->getHeight(); y++)
	{
		for (int x = 0; x<bmpInput->getWidth(); x++)
		{
			x_ = (a11 * x + a12 * y + b1) / (c1 * x + c2 * y + 1);
			y_ = (a21 * x + a22 * y + b2) / (c1 * x + c2 * y + 1);
			x_ = x_ < 0 ? 0 : x_;
			y_ = y_ < 0 ? 0 : y_;
			x_ = x_ >= bmpInput->getWidth() ? bmpInput->getWidth() - 1 : x_;
			y_ = y_ >= bmpInput->getHeight() ? bmpInput->getHeight() - 1 : y_;
			bmpOutput->setPixel(bmpInput->getPixel(x, y), x_, y_);
			coord[x_][y_] = 1;
		}
	}

	for (int y = 0; y < bmpInput->getHeight(); y++)
	{
		if (coord[0][y] == -1)
		{
			bmpOutput->setPixel(brg::PixelRGB(0, 0, 0), 0, y);
			coord[0][y] = 1;
		}

		if (coord[bmpInput->getWidth() - 1][y] == -1)
		{
			bmpOutput->setPixel(brg::PixelRGB(0, 0, 0), bmpInput->getWidth() - 1, y);
			coord[bmpInput->getWidth() - 1][y] = 1;
		}
	}

	for (int x = 0; x < bmpInput->getWidth(); x++)
	{
		if (coord[x][0] == -1)
		{
			bmpOutput->setPixel(brg::PixelRGB(128, 128, 128), x, 0);
			coord[x][0] = 1;
		}

		if (coord[x][bmpInput->getHeight() - 1] == -1)
		{
			bmpOutput->setPixel(brg::PixelRGB(128, 128, 128), x, bmpInput->getHeight() - 1);
			coord[x][bmpInput->getHeight() - 1] = 1;
		}
	}

	for (int y = 2; y<bmpInput->getHeight()-2; y++)
	{
		for (int x = 2; x<bmpInput->getWidth()-2; x++)
		{
			if (coord[x][y] == -1)
			{
				BilinealInterpolationOnePixel(bmpOutput, coord, x, y);
			}
		}
	}
}

void projectiveTransformation(string input, string output, brg::BMP*& bmpInput, brg::BMP*& bmpOutput, vector<vector<double>>& P)
{
	CImg<unsigned char> imageInput(input.c_str());
	imageInput.resize(512, 512);
	imageInput.save(output.c_str());
	imageInput.save(input.c_str());

	bmpInput = new brg::BMP(input.c_str());
	bmpOutput = new brg::BMP(output.c_str());
	
	/* Projection Matrix: P
	[	a11	a12	b1
		a21	a22	b2
		c1	c2	1	]	
	*/

	double a11 = P[0][0], a12 = P[0][1], b1 = P[0][2];
	double a21 = P[1][0], a22 = P[1][1], b2 = P[1][2];
	double c1  = P[2][0], c2  = P[2][1];
	int x_, y_;
	for (int y = 0; y < bmpInput->getHeight(); y++)
	{
		for (int x = 0; x < bmpInput->getWidth(); x++)
		{
			bmpOutput->setPixel(brg::PixelRGB(0, 0, 0), x, y);
		}
	}

	for (int y = 0; y<bmpInput->getHeight(); y++)
	{
		for (int x = 0; x<bmpInput->getWidth(); x++)
		{
			x_ = ( a11 * x + a12 * y + b1 ) / (c1 * x + c2 * y + 1);
			y_ = ( a21 * x + a22 * y + b2 ) / (c1 * x + c2 * y + 1);
			x_ = x_ < 0 ? 0 : x_;
			y_ = y_ < 0 ? 0 : y_;
			x_ = x_ >= bmpInput->getWidth() ? bmpInput->getWidth()-1 : x_;
			y_ = y_ >= bmpInput->getHeight() ? bmpInput->getHeight()-1 : y_;
			bmpOutput->setPixel(bmpInput->getPixel(x, y), x_, y_);
		}
	}
}

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