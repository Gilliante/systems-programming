/*
 *  BitMap.h
 *  HW 3
 */

#ifndef BITMAPPIC_H
#define BITMAPPIC_H

#include <cstdio>
#include <vector>
#include <cstdint>

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

struct BitMap {
	uint32_t width, height;
	uint8_t *image;
};

typedef BitMap* BitMapPointer;

/* CreateBitMapPic
 * Create a new Bit Map of the desired height and width. Both the height and
 * width should be positive.
 */
BitMap *CreateBitMapPic(int width, int height);


/* Free
 * Free the memory associated with a bit map and set the pointer to 0.
 */
void Free(BitMapPointer &bm);

/* Load
 * Load a bitmap from the specified file.
 * Code should support 24- and 32-bit bitmaps. Internally, the code should use
 * 32-bit bitmaps.
 * If the height is negative, the file has the rows reversed; they should be reversed
 * upon loading, so that all bitmaps will be stored in the same way.
 * Returns 0 if the loading failed.
 */
BitMap *Load(const char *file);

/* Save
 * Write a bitmap to the specified file.
 * Code should write 32-bit bitmaps with positive height.
 */
void Save(BitMap *bm, const char *file);

/* GetPixel
 * Load the RGB values from a given pixel. (0, 0) is the bottom left of the image.
 */
void GetPixel(BitMap *bm, int x, int y, uint8_t &redByte, uint8_t &greenByte, uint8_t &blueByte, uint8_t &alphaByte);


/* SetPixel
 * Write the RGB values to a given pixel. (0, 0) is the bottom left of the image.
 */
void SetPixel(BitMap *bm, int x, int y, uint8_t redByte, uint8_t greenByte, uint8_t blueByte, uint8_t alphaByte);

#endif