/*
 *  BitMap.h
 *  HW 6
 */

#ifndef BITMAPPIC_H
#define BITMAPPIC_H

#include <cstdio>
#include <vector>
#include <cstdint>

 //Gillian Ehman
 //I collaberated with Samantha Holstine.

struct BitMapHeader {
	BitMapHeader()
		:zero(0), bfOffBits(sizeof(BitMapHeader) + 2), biSize(40), biPlanes(1),
		biBitCount(32), biCompression(0), biSizeImage(0), biXPelsPerMeter(2835), biYPelsPerMeter(2835),
		biClrUsed(0), biClrImportant(0) {}

	uint32_t bfSize;
	uint32_t zero;
	uint32_t bfOffBits;

	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

class BitMap {
public:
	//BitMap(); //-A default constructor
	BitMap(int width, int height); //- A constructor
	~BitMap(); //-A destructor which frees all memory associated with the linked list:
	BitMap &operator=(const BitMap &); //-An Assignment operator
	BitMap(const BitMap &); //-A copy constructor
	BitMap* Load(const char *file);
	void Save(BitMap *bm, const char *file);
	void GetPixel(int x, int y, uint8_t &redByte, uint8_t &greenByte, uint8_t &blueByte, uint8_t &alphaByte);
	void SetPixel(int x, int y, uint8_t redByte, uint8_t greenByte, uint8_t blueByte, uint8_t alphaByte);
	uint32_t getWidth();
	uint32_t getHeight();
	uint8_t* getImage();

private:
	uint32_t width, height;
	uint8_t *image;
};
#endif