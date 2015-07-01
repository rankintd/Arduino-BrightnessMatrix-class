#include "EasyBMP/EasyBMP.h"

int main(int argc, char* argv[])
{
	const double RED_LUMINANCE_SCALE = 0.3;
	const double GREEN_LUMINANCE_SCALE = 0.59;
	const double BLUE_LUMINANCE_SCALE = 0.11;

	const int DEFAULT_NEW_SIZE_X = 8;
	const int DEFAULT_NEW_SIZE_Y = 8;

	BMP in_bitmap;
	BMP out_bitmap;
	int new_size_x;
	int new_size_y;

	//If new size is specified then use it, otherwise use default
	if (argc == 4)
	{
		new_size_x = (int)argv[2];
		new_size_y = (int)argv[3];
	}
	else
	{
		new_size_x = DEFAULT_NEW_SIZE_X;
		new_size_y = DEFAULT_NEW_SIZE_Y;
	}

	//Read in bitmap
	in_bitmap.ReadFromFile(argv[1]);

	//Greyscale
	for (int x = 0; x < in_bitmap.TellWidth(); x++)
	{
		for (int y = 0; y < in_bitmap.TellHeight(); y++)
		{
			double red_luminance = in_bitmap(x, y)->Red * RED_LUMINANCE_SCALE ;
			double green_luminance = in_bitmap(x, y)->Green * GREEN_LUMINANCE_SCALE;
			double blue_luminance = in_bitmap(x, y)->Blue * BLUE_LUMINANCE_SCALE;
			double luminance = red_luminance + green_luminance + blue_luminance;
			in_bitmap(x, y)->Red = luminance;
			in_bitmap(x, y)->Green = luminance;
			in_bitmap(x, y)->Blue = luminance;
		}
	}
	in_bitmap.SetBitDepth(8);
	CreateGrayscaleColorTable(in_bitmap);

	//Resize
	out_bitmap.SetSize(new_size_x, new_size_y);
	out_bitmap.SetBitDepth(24);

	for (int x = 0; x < out_bitmap.TellHeight(); x++)
	{
		for (int y = 0; y < out_bitmap.TellWidth(); y++)
		{
			RGBApixel Temp = GetResizePixel(in_bitmap, x, out_bitmap.TellWidth(), y, out_bitmap.TellHeight());
			*out_bitmap(x, y) = Temp;
		}
	}

	//write to file
	out_bitmap.WriteToFile("DotMatrixImage.bmp");
}