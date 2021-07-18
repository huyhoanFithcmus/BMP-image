#include <iostream>
#include <stdio.h>
#include <string.h>
#include "bmp.h"
#include <conio.h>

using namespace std;

#define BMP_SOURCE		"lenna.bmp"

void changeBmp(PixelArray& p)
{
	for (int i = 0; i < p.rowCount; i++)
		for (int j = 0; j < p.columnCount; j++)
		{
			Color* c = &p.pixels[i][j];
			int avg = 0.85 * c->red + 0.1 * c->green + 0.05 * c->blue;

			c->red = avg;
			c->green = avg;
			c->blue = avg;
		}
}

void changeBmpindex(PixelArray& p)
{
	for(int i = 0; i < p.rowCount; i++)
		for (int j = 0; j < p.rowCount; j++)
		{

		}
}

void demoReadBmp()
{
	BmpFile bmpSource;

	readBmpFile((char*)BMP_SOURCE, bmpSource);
	printBmpHeader(bmpSource);
	printBmpDib(bmpSource);

	(void)getchar();
	system("cls");

	//changeBmp(bmpSource.pixelArray);
	drawBmp(bmpSource);
	
	releaseBmpPixelArray(bmpSource);
}

int main()
{
	demoReadBmp();
	while (!_kbhit())
		_sleep(1000);
}