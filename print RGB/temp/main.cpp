#include "Header.h"

using namespace std;

#define BMP_SOURCE		"lenna.bmp"

void Bmp()
{
	BmpFile bmp;
	readBmpFile((char*)BMP_SOURCE, bmp);
	drawBmp(bmp);
	releaseBmpPixelArray(bmp);
}

void RGB_Out(double &x, double &y)
{
	BmpFile bmpSource;
	readBmpFile((char*)BMP_SOURCE, bmpSource);
	PixelArray PX;

	releaseBmpPixelArray(bmpSource);
}

unsigned char* ReadBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");

    if (f == NULL)
        throw "Error.";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = (int)&info[18];
    int height = (int)&info[22];

    cout << endl;
    cout << "  Name: " << filename << endl;
    cout << " Width: " << width << endl;
    cout << "Height: " << height << endl;

    int row_padded = (width * 3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    for (int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for (int j = 0; j < width * 3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j + 2];
            data[j + 2] = tmp;

            cout << "R: " << (int)data[j] << " G: " << (int)data[j + 1] << " B: " << (int)data[j + 2] << endl;
        }
    }

    fclose(f);
    return data;
}

void RGB()
{
    cout << "enter x:" << endl;
    int x;
    cin >> x;
    cout << "enter y:" << endl;
    int y;
    cin >> y;
    BmpFile bmp;
    readBmpFile((char*)BMP_SOURCE, bmp);
    cout <<"R :"<<(int)bmp.BMPpixelArray.pixels[x][y].red << endl;
    cout <<"B :"<<(int)bmp.BMPpixelArray.pixels[x][y].blue << endl;
    cout <<"G :"<<(int)bmp.BMPpixelArray.pixels[x][y].green << endl;
}

int main()
{
    RGB();
    cout << "press enter to show your Lenna " << endl;;
    system("pause");
	Bmp();
}

