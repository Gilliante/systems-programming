/*
 *  main.cpp
 *  HW 3
 */

#include <iostream>
#include "BitMap.h"

int main(int argc, const char * argv[])
{
	/* This code tests your bitmap implementation on 24-bit and 32-bit files. The desired output
	 * files are included for comparison purposes.
	 */


	 /** Test #1: Create a new file with a gradient from yellow to green to cyan (bottom to top) */
	BitMap *bm = CreateBitMapPic(255, 255);
	for (int x = 0; x < 255; x++)
	{
		for (int y = 0; y < 255; y++)
		{
			SetPixel(bm, x, y, 255 - y, 255, y, 255);
		}
	}
	std::cout << "Processing test.bmp\n";
	Save(bm, "test.bmp");
	Free(bm);

	/** Test #2: A 24-bit file with reversed height; add a 40x40 red square in the lower right corner */
	std::cout << "Processing picture.bmp\n";
	bm = Load("picture.bmp");
	if (bm == 0)
	{
		std::cout << "Load of 'picture.bmp' failed\n";
	}
	else {
		for (int x = 0; x < 40; x++)
		{
			for (int y = 0; y < 40; y++)
			{
				SetPixel(bm, bm->width - x - 1, y, 255, 0, 0, 255);
			}
		}
		Save(bm, "picture new.bmp");
		free(bm);
	}

	/** Test #3: A 24-bit file; add a 40x40 blue square in the upper right corner */
	std::cout << "Processing train.bmp\n";
	bm = Load("train.bmp");
	if (bm == 0)
	{
		std::cout << "Load of 'train.bmp' failed\n";
	}
	else {
		for (int x = 0; x < 40; x++)
		{
			for (int y = 0; y < 40; y++)
			{
				SetPixel(bm, bm->width - x - 1, bm->height - y - 1, 0, 0, 255, 255);
			}
		}
		Save(bm, "train new.bmp");
		free(bm);
	}

	/** Test #4: A 24-bit file with padding; add a 40x40 blue square in the upper right corner */
	std::cout << "Processing mountain.bmp\n";
	bm = Load("mountain.bmp");
	if (bm == 0)
	{
		std::cout << "Load of 'mountain.bmp' failed\n";
	}
	else {
		// Convert to grayscale
		for (int x = 0; x < bm->width; x++)
		{
			for (int y = 0; y < bm->height; y++)
			{
				uint8_t r, g, b, a;
				GetPixel(bm, x, y, r, g, b, a);
				uint8_t average = (r + g + b) / 3;
				SetPixel(bm, x, y, average, average, average, a);
			}
		}
		Save(bm, "mountain new.bmp");
		free(bm);
	}

	/* Test #5: A 32-bit file with reversed height; extract the green channel */
	std::cout << "Processing canmore.bmp\n";
	bm = Load("canmore.bmp"); 
	if (bm == 0)
	{
		std::cout << "Load of 'canmore.bmp' failed\n";
	}
	else {
		for (int x = 0; x < bm->width; x++)
		{
			for (int y = 0; y < bm->height; y++)
			{
				uint8_t r, g, b, a;
				GetPixel(bm, x, y, r, g, b, a);
				SetPixel(bm, x, y, 0, g, 0, a);
			}
		}
		Save(bm, "canmore new.bmp");
		free(bm);
	}


	system("pause");
}