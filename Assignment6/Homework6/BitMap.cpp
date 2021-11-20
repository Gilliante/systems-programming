/*
 *  BitMap.cpp
 *  HW 6
 */

 //Gillian Ehman
 //I collaberated with Samantha Holstine.

#define _CRT_SECURE_NO_WARNINGS

#include "BitMap.h"
#include <stdio.h>
#include <iostream>

/* Constructor
* Create a new Bit Map of the desired height and width.Both the height and
* width should be positive.
*/
BitMap::BitMap(int width, int height)
{
	if (width == 0 || height == 0)
	{
		this->image = NULL;
	}
	this->width = width;
	this->height = height;
	this->image = new uint8_t[width * height * 4];
}


/* Destructor
 * Free the memory associated with a bit map and set the pointer to 0.
 */
BitMap::~BitMap()
{
	delete[] image;
	image = NULL;
	width = NULL;
	height = NULL;
}

//Assignment operator
BitMap& BitMap::operator=(const BitMap& source)
{
	int imageLength = source.width * source.height * 4;

	if (&source != this)
	{
		delete[] image;
		this->width = source.width;
		this->height = source.height;
		this->image = new uint8_t[imageLength];
		for (int i = 0; i < imageLength; i++)
		{
			this->image[i] = source.image[i];
		}
	}
	return *this;
}

//Copy constructor
BitMap::BitMap(const BitMap & source)
{
	int imageLength = source.width * source.height * 4;

	this->width = source.width;
	this->height = source.height;
	this->image = new uint8_t[imageLength];
	for (int i = 0; i < imageLength; i++)
	{
		image[i] = source.image[i];
	}
}

/* Load
 * Load a bitmap from the specified file.
 * Code should support 24- and 32-bit bitmaps. Internally, the code should use
 * 32-bit bitmaps.
 * If the height is negative, the file has the rows reversed; they should be reversed
 * upon loading, so that all bitmaps will be stored in the same way.
 * Returns 0 if the loading failed.
 */
BitMap* BitMap::Load(const char *file)
{
	BitMap* mapPtr;
	BitMapHeader* headerPtr = new BitMapHeader();

	// 0. If any of the operations fail, free any memory and return 0
	// 1. Open the file
	FILE* bitMapFileToRead = fopen(file, "rb+");

	if (bitMapFileToRead == 0)
	{
		std::cout << "Couldn't find file." << "\n";
		return 0;
	}
	if (bitMapFileToRead == NULL)
	{
		std::cout << "File doesn't exist." << "\n";
		return 0;
	}

	// 2a. Read the initial fields of the header; verify that the file type (BM) is correct.
	uint16_t signature = 0;

	fread(&signature, sizeof(uint16_t), 1, bitMapFileToRead);
	if (signature != 0x4D42 && signature != 0x424D)
	{
		std::cout << "File type is incorrect.\n";
		return 0;
	}

	// 2b. Read the rest of the header.
	fread(headerPtr, sizeof(BitMapHeader), 1, bitMapFileToRead);

	// 3. Verify if the file is 24 or 32 bits.
	bool is32 = false;
	bool is24 = false;

	if (headerPtr->biBitCount != 32 && headerPtr->biBitCount != 24)
	{
		std::cout << "File size is not supported.\n";
		return 0;
	}

	//checks if the bit count is 32 or 24
	if (headerPtr->biBitCount == 32)
	{
		is32 = true;
	}
	if (headerPtr->biBitCount == 24)
	{
		is24 = true;
	}

	// 4. Check if the rows are stored in reverse order (due to negative height)
	bool isReversed = false;
	signed int heightOfImage = (signed int)headerPtr->biHeight;

	if (heightOfImage < 0)
	{
		std::cout << "Rows are in reverse order.\n";
		isReversed = true;
		heightOfImage = (signed int)(headerPtr->biHeight * -1);
	}

	// 5. Make sure the file isn't compressed (compression should be 0)
	if (headerPtr->biCompression != 0)
	{
		std::cout << "File is compressed.\n";
		return 0;
	}

	//create a new map pointer varying if it's height is negative or positive
	if (isReversed)
	{
		mapPtr = new BitMap(headerPtr->biWidth, heightOfImage);
	}
	else
	{
		mapPtr = new BitMap(headerPtr->biWidth, headerPtr->biHeight);
	}

	// 6. Load the file if it is 32 bits
	// 6a. If the file is 32 bits, you can read one row at a time.
	// 6b. If the height is negative, you need to read from disk into the rows in memory in reverse order
	// 6c. 32-bit files are never padded.
	uint8_t* pixelPtr = mapPtr->image;
	if (is32)
	{
		if (isReversed)
		{
			for (int i = 1; i <= heightOfImage; i++)
			{
				fseek(bitMapFileToRead, -i * mapPtr->width * 4, SEEK_END);
				fread(pixelPtr, sizeof(uint8_t), mapPtr->width * 4, bitMapFileToRead);
				pixelPtr += mapPtr->width * 4;
			}
		}
		//if height is not reversed (negative)
		else
		{
			for (int i = 1; i <= mapPtr->width * mapPtr->height * 4; i++)
			{
				fread(&mapPtr->image[i - 1], 1, 1, bitMapFileToRead);
			}
		}
	}

	// 7. Load the file if it is 24 bits
	// 7a. If the file is 24 bits, you need to read 3 bytes at a time and make space in memory for
	//     the extra byte. (Since your internal storage should always be 32 bits.)
	// 7b. If the height is negative, you need to read from disk into the rows in memory in reverse order
	// 7c. If the length of each line isn't divisible by 4, you need to read extra padding from the file.
	//     This padding shouldn't be stored in the bitmap data in memory.
	if (is24)
	{
		int byteCount = 0;
		if (isReversed)
		{
			for (int i = heightOfImage - 1; i >= 0; i--)
			{
				for (int j = 0; j < mapPtr->width; j++)
				{
					byteCount += fread(&mapPtr->image[(i * mapPtr->width + j) * 4], sizeof(uint8_t), 3, bitMapFileToRead);
					mapPtr->image[((i * mapPtr->width + j) * 4) + 3] = 0;
				}
				//add padding if necessary
				if (byteCount % 4 != 0)
				{
					uint8_t numPadding[4];
					fread(&numPadding, sizeof(4 - (byteCount % 4)), 1, bitMapFileToRead);
				}
			}
		}
		//if height is not reversed (negative)
		else
		{
			for (int i = 0; i < heightOfImage; i++)
			{
				for (int j = 0; j < mapPtr->width; j++)
				{
					byteCount += fread(&mapPtr->image[(i * mapPtr->width + j) * 4], sizeof(uint8_t), 3, bitMapFileToRead);
					mapPtr->image[((i * mapPtr->width + j) * 4) + 3] = 0;
				}
				//add padding if necessary
				if (byteCount % 4 != 0)
				{
					uint8_t numPadding[4];
					fread(&numPadding, sizeof(4 - (byteCount % 4)), 1, bitMapFileToRead);
				}
			}
		}
	}

	// 8. Close the file.
	fclose(bitMapFileToRead);

	// 9. Return a pointer to the bitmap.
	return mapPtr;
}

/* Save
 * Write a bitmap to the specified file.
 * Code should write 32-bit bitmaps with positive height.
 */
void BitMap::Save(BitMap *bm, const char *file)
{
	FILE* bitMapFile = fopen(file, "wb+");

	// 2. Create a header and write it to the file
	uint16_t signature = 0x4D42; //adds BM to file

	fwrite(&signature, sizeof(uint16_t), 1, bitMapFile);

	BitMapHeader header;

	header.biWidth = bm->width;
	header.biHeight = bm->height;
	header.biBitCount = 32;
	header.bfOffBits = sizeof(BitMapHeader) + 2;
	fwrite(&header, sizeof(BitMapHeader), 1, bitMapFile);

	// 3. Write the bitmap data to the file
	fwrite(&bm->width, sizeof(uint32_t), 1, bitMapFile);
	fwrite(&bm->height, sizeof(uint32_t), 1, bitMapFile);
	fwrite(bm->image, sizeof(uint8_t), bm->width * bm->height * 4, bitMapFile);

	// 4. Close the file
	fclose(bitMapFile);
}

/* GetPixel
 * Load the RGB values from a given pixel. (0, 0) is the bottom left of the image.
 */
void BitMap::GetPixel(int x, int y, uint8_t &redByte, uint8_t &greenByte, uint8_t &blueByte, uint8_t &alphaByte)
{
	int index = (y * this->width + x) * 4;

	blueByte = this->image[index];
	greenByte = this->image[index + 1];
	redByte = this->image[index + 2];
	alphaByte = this->image[index + 3];
}


/* SetPixel
 * Write the RGB values to a given pixel. (0, 0) is the bottom left of the image.
 */
void BitMap::SetPixel(int x, int y, uint8_t redByte, uint8_t greenByte, uint8_t blueByte, uint8_t alphaByte)
{
	int index = (y * this->width + x) * 4;

	this->image[index] = blueByte;
	this->image[index + 1] = greenByte;
	this->image[index + 2] = redByte;
	this->image[index + 3] = alphaByte;
}

//returns the width of the map
uint32_t BitMap::getWidth()
{
	return width;
}

//returns the height of the map
uint32_t BitMap::getHeight()
{
	return height;
}

//returns the image of the map
uint8_t* BitMap::getImage()
{
	return image;
}
