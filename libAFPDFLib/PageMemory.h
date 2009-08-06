#pragma once
#include "stdafx.h"
#define			DIB_COPY_BATCH_SIZE 2048

class PageMemory
{
private:
	void *_bits;
	HBITMAP _bitmap;

	BITMAPINFO GetBitmapInfo();

	float _factorW;
	float _factorH;
	double _renderDPI;
	int _newWidth;
	int _newHeight;

public:
	PageMemory(void);
	~PageMemory(void);

	int Create(HDC clientDC, int width, int height, double renderDPI);
	int SetDIBits(HDC clientDC,const void *lpBits);
	void SetDimensions(int width, int height, double renderDPI);

	void Resize(int width, int height, double renderDPI);
	int Draw(HDC hdc, int xSrc, int ySrc, int width, int height, int xDest, int yDest);
	int Draw(HBITMAP hdc, int xSrc, int ySrc, int width, int height, int xDest, int yDest);

	double getRenderDPI(){
		return _renderDPI;
	}
	void setRenderDPI(double value){
		_renderDPI = value;
	}
	void *GetDIBits();

	void Dispose();

	int Width;
	int Height;

};
