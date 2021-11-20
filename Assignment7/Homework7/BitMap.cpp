/*
 *  BitMap.cpp
 *  HW 7
 */
#define _CRT_SECURE_NO_WARNINGS

#include "BitMap.h"
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <thread>

 //Gillian Ehman

  //Constructor
BitMap::BitMap(int w, int h) :width(w), height(h)
{
	image = new uint8_t[width*height * 4];
}

//Copy constructor that makes copy based on file name, loads it from the file
BitMap::BitMap(const char* file)
{
	bool loadFailed = false;
	FILE *f = fopen(file, "rb+");
	BitMapHeader header;

	if (f == 0)
	{
		std::cout << "Unable to find file\n";
		loadFailed = true;
	}
	else
	{
		uint16_t bfType;
		fread(&bfType, sizeof(bfType), 1, f);
		if (bfType != 19778)
		{
			std::cout << "Unable to load file properly\n";
			fclose(f);
			loadFailed = true;
		}
		else
		{
			fread(&header, sizeof(BitMapHeader), 1, f);
			if (header.biBitCount < 24)
			{
				std::cout << "We only support 24 & 32-bit files\n";
				fclose(f);
				loadFailed = true;
			}
			else
			{
				std::cout << ":" << header.biBitCount << "bit image\n";

				if (header.biCompression != 0)
				{
					std::cout << "We don't support compressed files\n";
					fclose(f);
					loadFailed = true;
				}
				else
				{
					height = header.biHeight;
					bool reverseHeight = false;
					if ((int32_t)header.biHeight < 0)
					{
						std::cout << ":height reversed\n";
						height = -1 * (int32_t)height; //if error, cast to signed before do -1*....
						reverseHeight = true;
					}

					width = header.biWidth;
					image = new uint8_t[height*width * 4];

					fseek(f, header.bfOffBits, SEEK_SET);

					if (header.biBitCount == 32)
					{
						for (int x = 0; x < height; x++)
						{
							if (reverseHeight)
								fread(&image[(height - x - 1)*width * 4], sizeof(char), width * 4, f);
							else
								fread(&image[x*width * 4], sizeof(char), width * 4, f);
						}
					}
					else if (header.biBitCount == 24)
					{
						bool padding = false;
						for (int x = 0; x < height; x++)
						{
							int bytesRead = 0;
							for (int y = 0; y < width; y++)
							{
								if (reverseHeight)
									bytesRead += fread(&image[(height - x - 1)*width * 4 + y * 4], sizeof(char), 3, f);
								else
									bytesRead += fread(&image[x*width * 4 + y * 4], sizeof(char), 3, f);
								image[x*width * 4 + y * 4 + 3] = 0;
							}
							while (0 != bytesRead % 4)
							{
								char zero[4] = { 0, 0, 0, 0 };
								bytesRead += fread(zero, sizeof(char), 1, f);
								padding = true;
							}
						}
						if (padding)
							std::cout << ":padding necessary\n";
					}
					fclose(f);
				}
			}
		}
	}

	if (loadFailed)
	{
		width = 10;
		height = 10;
		image = new uint8_t[width*height * 4];
		for (int i = 0; i < width*height * 4; i++)
		{
			image[i] = 255;
		}
	}


}

//copy constructor
BitMap::BitMap(const BitMap& b) :width(b.width), height(b.height)
{
	image = new uint8_t[width*height * 4];

	for (int i = 0; i < width*height * 4; i++)
	{
		image[i] = b.image[i];
	}
}

//destructor
BitMap::~BitMap()
{
	width = 0;
	height = 0;
	if (image)
	{
		delete[] image;
		image = NULL;
	}
}

//assignment operator
BitMap& BitMap::operator=(const BitMap& b)
{
	if (this == &b)
		return *this;

	width = b.width;
	height = b.height;

	delete[] image;
	image = new uint8_t[width*height * 4];

	for (int i = 0; i < width*height * 4; i++)
	{
		image[i] = b.image[i];
	}

	return *this;
}

//saves the bit map to the file specified
void BitMap::Save(const char *file)
{
	FILE *f = fopen(file, "wb+");

	if (f == 0)
		return;

	BitMapHeader header;
	header.biWidth = width;
	header.biHeight = height;

	header.bfSize = sizeof(BitMapHeader) + 2 + (width)*height * 4;
	header.biSizeImage = (width)*height * 4;
	uint16_t bfType = 19778; // 0x4D42
	fwrite(&bfType, sizeof(bfType), 1, f);
	fwrite(&header, sizeof(header), 1, f);
	for (int x = 0; x < height; x++)
	{
		fwrite(&image[x*width * 4], sizeof(char), width * 4, f);
	}
	fclose(f);
}

//sets the pixel of the image
void BitMap::SetPixel(int x, int y, uint8_t redByte, uint8_t greenByte, uint8_t blueByte, uint8_t alphaByte)
{
	// BGRA
	image[y*width * 4 + x * 4 + 0] = blueByte;
	image[y*width * 4 + x * 4 + 1] = greenByte;
	image[y*width * 4 + x * 4 + 2] = redByte;
	image[y*width * 4 + x * 4 + 3] = alphaByte;
}

//gets the pixel of the image
void BitMap::GetPixel(int x, int y, uint8_t &redByte, uint8_t &greenByte, uint8_t &blueByte, uint8_t &alphaByte) const
{
	blueByte = image[y*width * 4 + x * 4 + 0];
	greenByte = image[y*width * 4 + x * 4 + 1];
	redByte = image[y*width * 4 + x * 4 + 2];
	alphaByte = image[y*width * 4 + x * 4 + 3];
}

/*Helper method that detects the edges of the image, drawing them in black
1) Create a destination image that will store the pixel values for each pixel after applying masks. (This image will be exactly the same size as the input image -- the contents of this image are unimportant; they will be overwritten by the algorithm below.)

2) For each pixel in the destination image, compute the color value for the pixel based on the original image as follows :
 - Compute the greyscale value of the eight neighboring pixels from the original image(in each of the four cardinal directions and each of the four diagonals)
	using the formula : (blue + green + red) / 3
 - Compute Vx value by applying the vertical mask to the greyscale values computed in(1) (see how to do this below)
 - Compute Vy value by applying the horizontal mask to the greyscale values
 - Compute the new greyscale value of the pixel : gray = \sqrt[2]{ Vx ^ 2 + Vy ^ 2 } V x 2 + V y 2 2
 - Set the blue, green and red values of the pixel to(255 - gray).  (This colors the pixels that are not edges with white.)*/
BitMap* BitMap::CalculateGrayScale(BitMap* destination, int start, int end) const
{
	int col = 4; //x/width
	int row = start; //y/height

	/*When computing Vx, you multiple the grayscale value of the pixels above to the left, above, and above and to the right of the center pixel
		by weights of - 1, -2, and -1 respectively and then add them up.The grayscale value of the pixels below the center pixel are similarly multiplied
		by weights of 1, 2, and 1 respectively, and these are added to the previous sum.This sum is the value for Vx. (The original value for this
			pixel and its left and right neighbors are ignored.) Vy is computed similarly, except that instead of the weights being in rows, the
		weights are in columns. Current pixel is in center and pixels on boundary are ignored.

		Vx and Vy are computed from the original image and then used in the formula above to set the final pixel value in the new image.It is important
		that the original image is not changed. (If you try to change the original image you will get an error because the functions are all const.)*/
	while (row < end - 1)
	{
		uint8_t topLeftgrayScale = 0;
		uint8_t topLeftBlue = 0;
		uint8_t topLeftGreen = 0;
		uint8_t topLeftRed = 0;

		uint8_t topMiddlegrayScale = 0;
		uint8_t topMiddleBlue = 0;
		uint8_t topMiddleGreen = 0;
		uint8_t topMiddleRed = 0;

		uint8_t topRightgrayScale = 0;
		uint8_t topRightBlue = 0;
		uint8_t topRightGreen = 0;
		uint8_t topRightRed = 0;

		uint8_t leftgrayScale = 0;
		uint8_t leftBlue = 0;
		uint8_t leftGreen = 0;
		uint8_t leftRed = 0;

		uint8_t rightgrayScale = 0;
		uint8_t rightBlue = 0;
		uint8_t rightGreen = 0;
		uint8_t rightRed = 0;

		uint8_t bottomLeftgrayScale = 0;
		uint8_t bottomLeftBlue = 0;
		uint8_t bottomLeftGreen = 0;
		uint8_t bottomLeftRed = 0;

		uint8_t bottomMiddlegrayScale = 0;
		uint8_t bottomMiddleBlue = 0;
		uint8_t bottomMiddleGreen = 0;
		uint8_t bottomMiddleRed = 0;

		uint8_t bottomRightgrayScale = 0;
		uint8_t bottomRightBlue = 0;
		uint8_t bottomRightGreen = 0;
		uint8_t bottomRightRed = 0;
		uint8_t alpha = 0;

		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				if (x != 0 && y != 0)
				{
					//top left pixel
					if (x == -1 && y == 1)
					{
						GetPixel(col + (x * 4), row + y, topLeftRed, topLeftGreen, topLeftBlue, alpha);
						topLeftgrayScale = (topLeftBlue + topLeftGreen + topLeftRed) / 3;
					}
					//left pixel
					else if (x == -1 && y == 0)
					{
						GetPixel(col + (x * 4), row + y, leftRed, leftGreen, leftBlue, alpha);
						leftgrayScale = (leftBlue + leftGreen + leftRed) / 3;
					}
					//bottom left pixel
					else if (x == -1 && y == -1)
					{
						GetPixel(col + (x * 4), row + y, bottomLeftRed, bottomLeftGreen, bottomLeftBlue, alpha);
						bottomLeftgrayScale = (bottomLeftBlue + bottomLeftGreen + bottomLeftRed) / 3;
					}
					//top middle pixel
					else if (x == 0 && y == 1)
					{
						GetPixel(col + (x * 4), row + y, topMiddleRed, topMiddleGreen, topMiddleBlue, alpha);
						topMiddlegrayScale = (topMiddleBlue + topMiddleGreen + topMiddleRed) / 3;
					}
					//bottom middle pixel
					else if (x == 0 && y == -1)
					{
						GetPixel(col + (x * 4), row + y, bottomMiddleRed, bottomMiddleGreen, bottomMiddleBlue, alpha);
						bottomMiddlegrayScale = (bottomMiddleBlue + bottomMiddleGreen + bottomMiddleRed) / 3;
					}
					//top right pixel
					else if (x == 1 && y == 1)
					{
						GetPixel(col + (x * 4), row + y, topRightRed, topRightGreen, topRightBlue, alpha);
						topRightgrayScale = (topRightBlue + topRightGreen + topRightRed) / 3;
					}
					//right pixel
					else if (x == 1 && y == 0)
					{
						GetPixel(col + (x * 4), row + y, rightRed, rightGreen, rightBlue, alpha);
						rightgrayScale = (rightBlue + rightGreen + rightRed) / 3;
					}
					//bottom right pixel
					else if (x = 1 && y == -1)
					{
						GetPixel(col + (x * 4), row + y, bottomRightRed, bottomRightGreen, bottomRightRed, alpha);
						bottomRightgrayScale = (bottomRightBlue + bottomRightGreen + bottomRightRed) / 3;
					}
				}
			}
		}
		int8_t vx = ((topLeftgrayScale * -1) + (topMiddlegrayScale * -2) + (topRightgrayScale * -1)) + ((bottomLeftgrayScale * 1) +
			(bottomMiddlegrayScale * 2) + (bottomRightgrayScale * 1));
		int8_t vy = ((topLeftgrayScale * -1) + (leftgrayScale * -2) + (bottomLeftgrayScale * -1)) + ((topRightgrayScale * 1) +
			(rightgrayScale * 2) + (bottomRightgrayScale * 1));

		uint8_t gray = sqrt(pow(vx, 2) + pow(vy, 2));

		/*Extra Credit: In parallel, combine the image generated from edge detection with the original image as follows: If the pixel value is above a
		certain threshold (feel free to play with the threshold - choose something that produces an appropriate edge effect), then the pixel gets its color
		from the original image.  Otherwise, the pixel gets its values from the edges image. */

		//***DEAR KARIN, IF YOU WANT TO TEST THE EXTRA CREDIT, SET ISTESTINGEXTRA CREDIT BOOL BELOW TO TRUE****.
		bool isTestingExtraCredit = false;
		if (isTestingExtraCredit == true)
		{
			if (gray < 150 - gray)
			{
				uint8_t blue = 0;
				uint8_t green = 0;
				uint8_t red = 0;
				GetPixel(col, row, red, green, blue, alpha);
				destination->SetPixel(col, row, red, green, blue, alpha);
			}
			else
			{
				destination->SetPixel(col, row, 255 - gray, 255 - gray, 255 - gray, 255 - gray);
			}
		}
		else
		{
			destination->SetPixel(col, row, 255 - gray, 255 - gray, 255 - gray, 255 - gray);
		}

		col += 1;
		if (col >= (width - 4))
		{
			row++;
			col = 4;
		}
	}
	return destination;
}

//Method that uses threads to calculate the image with edge detection helper method
BitMap* BitMap::DetectEdges(int numThreads) const
{
	BitMap* destination = new BitMap(width, height);
	std::thread** threads = new std::thread*[numThreads];

	for (int i = 0; i < numThreads; i++)
	{
		if (i == 0)
		{
			threads[i] = new std::thread(&BitMap::CalculateGrayScale, this, destination, 1, (height - 1) / (i + 1));
		}
		else
		{
			threads[i] = new std::thread(&BitMap::CalculateGrayScale, this, destination, height / i, (height - 1) / (i + 1));
		}
		threads[i]->join();
	}
	for (int i = 0; i < numThreads; i++)
	{
		delete threads[i];
	}
	delete[] threads;

	return destination;
}
