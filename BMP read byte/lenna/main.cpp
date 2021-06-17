#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include "Header.h"

#define BMP_SOURCE		"lenna.bmp"


void demoReadBmp()
{
	BMP_File bmpSource{};
	readBMP_file((char*)BMP_SOURCE, bmpSource);
	(void)getchar();
	system("cls");
	//changeBmp(bmpSource.pixelArray);
}

int main()
{
	demoReadBmp();
}