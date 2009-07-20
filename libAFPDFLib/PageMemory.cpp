
#include "PageMemory.h"

PageMemory::PageMemory(void)
: _bitmap(0)
, _bits(0)
{
	Width=0;
	Height=0;
}

PageMemory::~PageMemory(void)
{
	Dispose();
}

int PageMemory::Create(HDC clientDC, int width, int height)
{
	//Delete object
	Dispose();
	Width=width;
	Height=height;
	BITMAPINFO bmi=GetBitmapInfo();
	bmi.bmiHeader.biHeight=-bmi.bmiHeader.biHeight;
	if((_bitmap = CreateDIBSection(clientDC, &bmi, DIB_RGB_COLORS, &_bits,NULL,0))==NULL){
		ASSERT(1==0);
		return FALSE;
	}
	
	return TRUE;
}

BITMAPINFO PageMemory::GetBitmapInfo()
{
	BITMAPINFO bmi;
	ZeroMemory(&bmi,sizeof(bmi));
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth=Width;
	//By default bitmaps are bottom up images which means 1st scanline is bottom when and last is top one.
	bmi.bmiHeader.biHeight=-Height;
	bmi.bmiHeader.biPlanes=1;
	bmi.bmiHeader.biBitCount=24;
	bmi.bmiHeader.biCompression=BI_RGB;

	bmi.bmiColors[0].rgbBlue = 0;
	bmi.bmiColors[0].rgbGreen = 0;
	bmi.bmiColors[0].rgbRed = 0;
	bmi.bmiColors[0].rgbReserved = 0;

	return bmi;
}

int PageMemory::SetDIBits(HDC clientDC,const void *lpBits)
{
	BITMAPINFO bmi=GetBitmapInfo();
	::SetDIBits(clientDC,_bitmap,0,Height,lpBits,&bmi,DIB_RGB_COLORS);

	return TRUE;
}

void PageMemory::SetDimensions(int width, int height)
{
	Width=width;
	Height=height;
}

int PageMemory::Draw(CBitmap *bmp, int xSrc, int ySrc, int width, int height, int xDest, int yDest)
{
	return TRUE;
}

int PageMemory::Draw(HDC hdc, int xSrc, int ySrc, int width, int height, int xDest, int yDest)
{
	BITMAPINFO bmi=GetBitmapInfo();
	//********START DIB
	CDC mdc;
	CDC dc;
	//mdc.Attach(hdc);
	dc.Attach(hdc);
	mdc.CreateCompatibleDC(&dc);
	HGDIOBJ pOld = mdc.SelectObject(_bitmap);
	
/*
	//11/07/09
	//SetDIBitsToDevice Fails when the bitmap is too big
	//so is necessary to write on batch
	//Write on batchs of DIB_COPY_BATCH_SIZE 2048
	if(bmi.bmiHeader.biHeight>2000){
		int batchs = bmi.bmiHeader.biHeight%DIB_COPY_BATCH_SIZE;
		int linesCopied=0;
		int linesToCopy;
		for(int i=0;i<batchs;i++){
			if(linesCopied + DIB_COPY_BATCH_SIZE > -bmi.bmiHeader.biHeight)
				linesToCopy = -bmi.bmiHeader.biHeight-linesCopied;
			else
				linesToCopy = -bmi.bmiHeader.biHeight;
			int currentLine = i*DIB_COPY_BATCH_SIZE;
			SetDIBitsToDevice(mdc.m_hDC,0,currentLine,bmi.bmiHeader.biWidth,
				linesToCopy,0,0,currentLine,linesToCopy,
				_bits,
				&bmi,DIB_RGB_COLORS);
		}
	}else{
		SetDIBitsToDevice(mdc.m_hDC,0,0,bmi.bmiHeader.biWidth,
			-bmi.bmiHeader.biHeight,0,0,0,-bmi.bmiHeader.biHeight,
			_bits,
			&bmi,DIB_RGB_COLORS);
	}
*/
	dc.BitBlt(xDest,yDest,Width,Height,&mdc,xSrc,ySrc,SRCCOPY);


	mdc.SelectObject(pOld);
	mdc.DeleteDC();
	dc.Detach();

	//********END DIB
	return TRUE;
}

int PageMemory::Draw(HBITMAP bitmap, int xSrc, int ySrc, int width, int height, int xDest, int yDest)
{
	return 0;
}

void *PageMemory::GetDIBits()
{
	return _bits;
}

void PageMemory::Dispose()
{
	if(_bitmap!=NULL){
		DeleteObject(_bitmap);
	}
	_bitmap=NULL;
	_bits=NULL;
}
