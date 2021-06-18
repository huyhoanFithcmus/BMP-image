#include <Windows.h>
#pragma warning(disable : 4996)
#include "Header.h"
using namespace std;
bool isBmpFile(FILE* f)
{
	if (f == NULL)
		return false;

	BmpSignature signature;
	fseek(f, 0, SEEK_SET);
	fread(&signature, sizeof(BmpSignature), 1, f);

	return signature.data[0] == 'B' && signature.data[1] == 'M';
}

void readBmpHeader(FILE* f, BmpFile& bmp)
{
	if (f == NULL)
		return;

	fseek(f, 0, SEEK_SET);
	fread(&bmp.BMPheader, sizeof(BmpHeader), 1, f);
}

void readBmpDib(FILE* f, BmpFile& bmp)
{
	if (f == NULL)
		return;

	fseek(f, sizeof(BmpHeader), SEEK_SET);
	fread(&bmp.BMPdib, sizeof(BmpDib), 1, f);
}

void readBmpPixelArray(FILE* f, BmpFile& bmp)
{
	if (f == NULL)
		return;

	int widthSize = bmp.BMPdib.imageWidth * (bmp.BMPdib.pixelSize / 8);
	bmp.BMPpixelArray.paddingSize = (4 - (widthSize % 4)) % 4;
	bmp.BMPpixelArray.lineSize = widthSize + bmp.BMPpixelArray.paddingSize;
	bmp.BMPpixelArray.rawByteSize = bmp.BMPpixelArray.lineSize * bmp.BMPdib.imageHeight;
	bmp.BMPpixelArray.rawBytes = new unsigned char[bmp.BMPpixelArray.rawByteSize];

	fseek(f, bmp.BMPheader.dataOffset, SEEK_SET);
	fread(bmp.BMPpixelArray.rawBytes, bmp.BMPpixelArray.rawByteSize, 1, f);

	initPixels(bmp.BMPpixelArray, bmp.BMPdib.imageWidth, bmp.BMPdib.imageHeight);
}

void initPixels(PixelArray& pa, int width, int height)
{
	pa.rowCount = height;
	pa.columnCount = width;

	pa.pixels = new Color * [pa.rowCount];
	for (int i = 0; i < pa.rowCount; i++)
		pa.pixels[pa.rowCount - 1 - i] = (Color*)(pa.rawBytes + pa.lineSize * i);
}

void drawBmp(BmpFile bmp)
{
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);

	for (int i = 0; i < bmp.BMPdib.imageHeight; i++)
		for (int j = 0; j < bmp.BMPdib.imageWidth; j++)
		{
			Color pixel = bmp.BMPpixelArray.pixels[i][j];
			SetPixel(hdc, j, i, RGB(pixel.red, pixel.green, pixel.blue));
		}

	ReleaseDC(console, hdc);
}

void releaseBmpPixelArray(BmpFile& bmp)
{
	delete[]bmp.BMPpixelArray.rawBytes;
	delete[]bmp.BMPpixelArray.pixels;
}

void readBmpFile(char* filename, BmpFile& bmp)
{
	FILE* f = fopen(filename, "rb");

	if (!isBmpFile(f))
	{
		printf("Error :)\n");
		return;
	}

	readBmpHeader(f, bmp);
	readBmpDib(f, bmp);
	readBmpPixelArray(f, bmp);

	fclose(f);
}

