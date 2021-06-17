#pragma once
#include <stdint.h>

#pragma pack(1)

using namespace std;

struct BMP_signature
{
	unsigned char data[2];
};

struct  BMP_header
{
	BMP_signature signature;
	uint32_t filesize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t PixelArrayByte;
};

struct BMP_DIB
{
	int32_t DIB_size;
	int32_t ImageWidth;
	int32_t ImageHeight;
	int16_t color_planes;
	int16_t color_depth;
	int32_t compression_algorithm;
	int32_t PixelArraySize;
	int32_t Horizontal_res;
	int32_t Vertical_res;
	int32_t NumberOfColor;
	int32_t NumberOfImportantColor;
};

struct  BMP_color
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
};

struct BMP_ColorTable
{
	BMP_color* colors;
	uint32_t length;
};

struct BMP_pixelArray
{
	unsigned char* rawBytes;
	int rawByteSize;
	int lineSize;
	char paddingSize;

	BMP_color **pixels;
	uint32_t rowCount;
	uint32_t columnCount;
};

struct BMP_File
{
	BMP_header header;
	BMP_DIB DIB;
	BMP_ColorTable ColorTable;
	BMP_pixelArray pixelArray;
};

bool isBMPfile(FILE* f);

void readBMP_header(FILE* f, BMP_File BMP);
void readBMP_DIB(FILE* f, BMP_File BMP);


void readBMP_file(char* filename, BMP_File bmp);