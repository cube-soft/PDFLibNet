#include "PageMemory.h"
#include "smoothbitmap.h"
PageMemory::PageMemory(void)
: _bitmap(0)
, _bits(0)
, _factorW(1)
, _factorH(1)
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
	//if(this->_bitmap != NULL )
//		Resize(width,height);
	_factorW=1;
	_factorH=1;
	Width=width;
	Height=height;	
}
void PageMemory::Resize(int width, int height)
{
	if(Width!=width || Height!=height){
/*		
		HBITMAP hbmNew = ScaleBitmap(_bitmap,width,height);
	
		if(_bitmap!=NULL){
			DeleteObject(_bitmap);
			_bitmap=NULL;
		}
		_bitmap=hbmNew;
		
		Width=width;
		Height=height;	
*/
		_factorW = (float)width/((float)Width);
		_factorH = (float)height/((float)Height);
	}
}
int PageMemory::Draw(CBitmap *bmp, int xSrc, int ySrc, int width, int height, int xDest, int yDest)
{
	return TRUE;
}

#define MAX(a,b) a>b?a:b
int PageMemory::Draw(HDC hdc, int xSrc, int ySrc, int width, int height, int xDest, int yDest)
{
	BITMAPINFO bmi=GetBitmapInfo();
	//********START DIB
	CDC dc;
	CDC mdc;
	CDC tmpDC;
	CBitmap bmpTmp;
	dc.Attach(hdc);
	mdc.CreateCompatibleDC(&dc);
	HGDIOBJ pOld = mdc.SelectObject(_bitmap);
	if(_factorW!=1 || _factorH !=1)
	{
		BITMAPINFO bih = GetBitmapInfo();
		bih.bmiHeader.biHeight = -bih.bmiHeader.biHeight ; //Top-Down
		dc.SetStretchBltMode(COLORONCOLOR );
		/*
		StretchDIBits(hdc,
			xDest*_factorW,
			yDest*_factorH,		//Location 
			width*_factorW,		//Width dest
			height*_factorH,	//Height dest
			xSrc,
			-ySrc,			//Source position
			width,
			height,		//Source Size
			_bits,				//DIB Bits
			&bih,				//Bitmap Info Header
			DIB_RGB_COLORS,		//RGB Data
			SRCCOPY );			//Copy the source
		*/
		int w=width;
		int h=height;
		//ZoomOut
		if(width*_factorW<w || height*_factorH<h)
		{
			w*=_factorW;
			h*=_factorH;
		}
		
		dc.StretchBlt(
			xDest,
			yDest,
			w*_factorW,
			h*_factorH ,
			&mdc,
			xSrc/_factorW,
			ySrc/_factorH,
			w,
			h,
			SRCCOPY);

	}else{
		

		dc.BitBlt(xDest,yDest,width,height,&mdc,xSrc,ySrc,SRCCOPY);		

		
	}
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
