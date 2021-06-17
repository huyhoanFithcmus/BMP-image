#include <stdio.h>
#include <Windows.h>
#pragma warning(disable : 4996)
#include "bmp.h"

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
	fread(&bmp.header, sizeof(BmpHeader), 1, f);
}

void readBmpDib(FILE* f, BmpFile& bmp)
{
	if (f == NULL)
		return;

	fseek(f, sizeof(BmpHeader), SEEK_SET);
	fread(&bmp.dib, sizeof(BmpDib), 1, f);
}


void printBmpHeader(BmpFile bmp)
{
	BmpHeader header = bmp.header;

	printf("*** BMP Header ***\n");
	printf("- File Size  : %d byte(s)\n", header.fileSize);
	printf("- Reserved1  : %d\n", header.reserved1);
	printf("- Reserved2  : %d\n", header.reserved2);
	printf("- Data Offset: %d byte(s)\n", header.dataOffset);
}

void printBmpDib(BmpFile bmp)
{
	BmpDib dib = bmp.dib;

	printf("*** BMP Dib ***\n");
	printf("- DIB Size               : %d byte(s)\n", dib.dibSize);
	printf("- Image Width            : %d\n", dib.imageWidth);
	printf("- Image Height           : %d\n", dib.imageHeight);
	printf("- Number of Color Planes : %d\n", dib.colorPlaneCount);
	printf("- Pixel Size             : %d bit(s)\n", dib.pixelSize);
	printf("- Compress Method        : %d\n", dib.compressMethod);
	printf("- Bitmap Size            : %d byte(s)\n", dib.bitmapByteCount);
	printf("- Horizontal Resolution  : %d\n", dib.horizontalResolution);
	printf("- Vertical Resolution    : %d\n", dib.verticalResolution);
	printf("- Number of Colors       : %d\n", dib.colorCount);
	printf("- Number of Impt Colors  : %d\n", dib.importantColorCount);
}

void readBmpPixelArray(FILE* f, BmpFile& bmp)
{
	if (f == NULL)
		return;

	int widthSize = bmp.dib.imageWidth * (bmp.dib.pixelSize / 8);
	bmp.pixelArray.paddingSize = (4 - (widthSize % 4)) % 4;
	bmp.pixelArray.lineSize = widthSize + bmp.pixelArray.paddingSize;
	bmp.pixelArray.rawByteSize = bmp.pixelArray.lineSize * bmp.dib.imageHeight;
	bmp.pixelArray.rawBytes = new unsigned char[bmp.pixelArray.rawByteSize];

	fseek(f, bmp.header.dataOffset, SEEK_SET);
	fread(bmp.pixelArray.rawBytes, bmp.pixelArray.rawByteSize, 1, f);

	initPixels(bmp.pixelArray, bmp.dib.imageWidth, bmp.dib.imageHeight);
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

	for (int i = 0; i < bmp.dib.imageHeight ; i++)
		for (int j = 0; j < bmp.dib.imageWidth; j++)
		{
			Color pixel = bmp.pixelArray.pixels[i][j];
			SetPixel(hdc, j + 20, i, RGB(pixel.red, pixel.green, pixel.blue));
		}
	ReleaseDC(console, hdc);
}

void releaseBmpPixelArray(BmpFile& bmp)
{
	delete[]bmp.pixelArray.rawBytes;
	delete[]bmp.pixelArray.pixels;
}

void readBmpFile(char* filename, BmpFile& bmp)
{
	FILE* f = fopen(filename, "rb");

	if (f == NULL || !isBmpFile(f))
	{
		printf("Error or Invalid Bmp File.\n");
		return;
	}

	readBmpHeader(f, bmp);
	readBmpDib(f, bmp);
	readBmpPixelArray(f, bmp);

	fclose(f);
}