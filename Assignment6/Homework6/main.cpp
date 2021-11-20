/*
 *  main.cpp
 *  HW 6
 */

#include <iostream>
#include "BitMap.h"

 //Gillian Ehman

int main(int argc, const char * argv[])
{
	/* This code tests your bitmap implementation on 24-bit and 32-bit files. The desired output
	 * files are included for comparison purposes.
	 */


	 /** Test #1: Create a new file with a gradient from yellow to green to cyan (bottom to top) */
	BitMap* bm = new BitMap(255, 255);
	for (int x = 0; x < 255; x++)
	{
		for (int y = 0; y < 255; y++)
		{
			bm->SetPixel(x, y, 255 - y, 255, y, 255);
		}
	}
	std::cout << "Processing test.bmp\n";
	bm->Save(bm, "test.bmp");

	//Test the copy constructor:
	std::cout << "Testing copy constructor.\n";
	BitMap* bmCopy = new BitMap(*bm);
	bmCopy->Save(bmCopy, "testCopy.bmp");
	bmCopy->~BitMap();

	//Test the assignment operator:
	std::cout << "Testing assignment operator.\n";
	BitMap* bmAssign = bm;
	bmCopy->Save(bmAssign, "testAssignment.bmp");

	//Test self assignment:
	std::cout << "Testing self assignment.\n";
	bmAssign = bmAssign;
	bmCopy->Save(bmAssign, "testSelfAssignment.bmp");
	bmAssign->~BitMap();

	bm->~BitMap();

	//Test for empty BitMap (unallocated array)
	std::cout << "Testing delete on unallocated array.\n";
	BitMap* emptyMap = new BitMap(0, 0);
	emptyMap->Save(emptyMap, "emptyTest.bmp");
	emptyMap->~BitMap();

	/** Test #2: A 24-bit file with reversed height; add a 40x40 red square in the lower right corner */
	std::cout << "Processing picture.bmp\n";
	bm = bm->Load("picture.bmp");
	if (bm == 0)
	{
		std::cout << "Load of 'picture.bmp' failed\n";
	}
	else {
		for (int x = 0; x < 40; x++)
		{
			for (int y = 0; y < 40; y++)
			{
				bm->SetPixel(bm->getWidth() - x - 1, y, 255, 0, 0, 255);
			}
		}
		bm->Save(bm, "picture new.bmp");
		bm->~BitMap();
	}

	/** Test #3: A 24-bit file; add a 40x40 blue square in the upper right corner */
	std::cout << "Processing train.bmp\n";
	bm = bm->Load("train.bmp");
	if (bm == 0)
	{
		std::cout << "Load of 'train.bmp' failed\n";
	}
	else {
		for (int x = 0; x < 40; x++)
		{
			for (int y = 0; y < 40; y++)
			{
				bm->SetPixel(bm->getWidth() - x - 1, bm->getHeight() - y - 1, 0, 0, 255, 255);
			}
		}
		bm->Save(bm, "train new.bmp");
		bm->~BitMap();
	}

	/** Test #4: A 24-bit file with padding; add a 40x40 blue square in the upper right corner */
	std::cout << "Processing mountain.bmp\n";
	bm = bm->Load("mountain.bmp");
	if (bm == 0)
	{
		std::cout << "Load of 'mountain.bmp' failed\n";
	}
	else {
		// Convert to grayscale
		for (int x = 0; x < bm->getWidth(); x++)
		{
			for (int y = 0; y < bm->getHeight(); y++)
			{
				uint8_t r, g, b, a;
				bm->GetPixel(x, y, r, g, b, a);
				uint8_t average = (r + g + b) / 3;
				bm->SetPixel(x, y, average, average, average, a);
			}
		}
		bm->Save(bm, "mountain new.bmp");
		bm->~BitMap();
	}

	/* Test #5: A 32-bit file with reversed height; extract the green channel */
	std::cout << "Processing canmore.bmp\n";
	bm = bm->Load("canmore.bmp");
	if (bm == 0)
	{
		std::cout << "Load of 'canmore.bmp' failed\n";
	}
	else {
		for (int x = 0; x < bm->getWidth(); x++)
		{
			for (int y = 0; y < bm->getHeight(); y++)
			{
				uint8_t r, g, b, a;
				bm->GetPixel(x, y, r, g, b, a);
				bm->SetPixel(x, y, 0, g, 0, a);
			}
		}
		bm->Save(bm, "canmore new.bmp");
		bm->~BitMap();
	}

	system("pause");
}
