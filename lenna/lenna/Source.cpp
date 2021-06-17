#include <stdio.h>
#include <Windows.h>
#include "Header.h"
#pragma warning(disable : 4996)

bool isBMPfile(FILE* f)
{
	if(f == NULL)
		return false;

	BMP_signature sign;

	fseek(f, 0, SEEK_SET);
	fread(&sign, sizeof(BMP_signature), 1, f);

	return sign.data[0] == 'B' && sign.data[1] == 'M';
}

void readBMP_header(FILE* f, BMP_File BMP)
{
	if (f == NULL)
		return;
	fseek(f, 0, SEEK_SET);
	fread(&BMP.header, sizeof(BMP_header), 1, f);

	printf("*** BMP Header ***\n");
	printf("- File Size  : %d byte(s)\n", BMP.header.filesize);
	printf("- Reserved1  : %d\n", BMP.header.reserved1);
	printf("- Reserved2  : %d\n", BMP.header.reserved2);
	printf("- Data Offset: %d byte(s)\n", BMP.header.PixelArrayByte);
	
}

void readBMP_DIB(FILE* f, BMP_File BMP)
{
	if (f == NULL)
		return;

	fseek(f, sizeof(BMP_header), SEEK_SET);
	fread(&BMP.DIB, sizeof(BMP_DIB), 1, f);

	printf("*** BMP Dib ***\n");
	printf("- DIB Size               : %d byte(s)\n", BMP.DIB.DIB_size);
	printf("- Image Width            : %d\n", BMP.DIB.ImageWidth);
	printf("- Image Height           : %d\n", BMP.DIB.ImageHeight);
	printf("- Number of Color Planes : %d\n", BMP.DIB.color_planes);
	printf("- Pixel Size             : %d bit(s)\n", BMP.DIB.color_depth);
	printf("- Compress Method        : %d\n", BMP.DIB.compression_algorithm);
	printf("- Bitmap Size            : %d byte(s)\n", BMP.DIB.PixelArraySize);
	printf("- Horizontal Resolution  : %d\n", BMP.DIB.Horizontal_res);
	printf("- Vertical Resolution    : %d\n", BMP.DIB.Vertical_res);
	printf("- Number of Colors       : %d\n", BMP.DIB.NumberOfColor);
	printf("- Number of Impt Colors  : %d\n", BMP.DIB.NumberOfImportantColor);
}


void readBMP_file(char* filename, BMP_File bmp)
{
	FILE* f = fopen(filename, "rb");

	if (f == NULL || !isBMPfile(f))
	{
		printf("Error or Invalid Bmp File.\n");
		return;
	}

	readBMP_header(f, bmp);
	readBMP_DIB(f, bmp);
	fclose(f);
}