#pragma once
#include "stdafx.h"
#define			DIB_COPY_BATCH_SIZE 2048

class PageMemory
{
private:
	void *_bits;
	HBITMAP _bitmap;

	BITMAPINFO GetBitmapInfo();
public:
	PageMemory(void);
	~PageMemory(void);

	int Create(HDC clientDC, int width, int height);
	int SetDIBits(HDC clientDC,const void *lpBits);
	void SetDimensions(int width, int height);

	int Draw(CBitmap *bmp, int xSrc, int ySrc, int width, int height, int xDest, int yDest);
	int Draw(HDC hdc, int xSrc, int ySrc, int width, int height, int xDest, int yDest);
	int Draw(HBITMAP hdc, int xSrc, int ySrc, int width, int height, int xDest, int yDest);

	void *GetDIBits();

	void Dispose();

	int Width;
	int Height;

};
