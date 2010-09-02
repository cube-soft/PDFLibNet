#include "PageMemory.h"

PageMemory::PageMemory(void)
: _bitmap(0)
, _bits(0)
, _factorW(1)
, _factorH(1)
, _renderDPI(0)
{
	Width=0;
	Height=0;
}

PageMemory::~PageMemory(void)
{
	Dispose();
}

int PageMemory::Create(HDC clientDC, int width, int height, double renderDPI, double *defcmt, double *deficmt)
{
	//Delete object
	Dispose();
	Width=width;
	Height=height;
	for(int i=0; i<6; ++i){
		defCTM[i] = defcmt[i];
		defICTM[i] =deficmt[i];
	}
	_renderDPI = renderDPI;
	BITMAPINFO bmi=GetBitmapInfo();
	bmi.bmiHeader.biHeight=-bmi.bmiHeader.biHeight;
	if((_bitmap = CreateDIBSection(clientDC, &bmi, DIB_RGB_COLORS, &_bits,NULL,0))==NULL){
//		ASSERT(1==0);
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

void PageMemory::SetDimensions(int width, int height, double renderDPI)
{
	//if(this->_bitmap != NULL )
//		Resize(width,height);
	_factorW=1;
	_factorH=1;
	_renderDPI = renderDPI;
	Width=width;
	Height=height;	
}
void PageMemory::Resize(int width, int height, double renderDPI)
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
		_renderDPI = renderDPI;
	}
}

#define MAX(a,b) a>b?a:b
int PageMemory::Draw(HDC hdc, int xSrc, int ySrc, int width, int height, int xDest, int yDest)
{
	BITMAPINFO bmi=GetBitmapInfo();
	//********START DIB
	//HDC dc;
	HDC mdc;
	//CDC tmpDC;
	//HBitmap bmpTmp;
	//dc.Attach(hdc);
	//mdc.CreateCompatibleDC(&dc);
	mdc = CreateCompatibleDC(hdc);
	HGDIOBJ pOld = SelectObject(mdc,_bitmap);
	if(_factorW!=1 || _factorH !=1)
	{
		BITMAPINFO bih = GetBitmapInfo();
		bih.bmiHeader.biHeight = -bih.bmiHeader.biHeight ; //Top-Down
		SetStretchBltMode(hdc,COLORONCOLOR );
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
		
		StretchBlt(hdc,
			xDest,
			yDest,
			w*_factorW,
			h*_factorH ,
			mdc,
			xSrc/_factorW,
			ySrc/_factorH,
			w,
			h,
			SRCCOPY);

	}else{
		

		BitBlt(hdc,xDest,yDest,width,height,mdc,xSrc,ySrc,SRCCOPY);		

		
	}
	SelectObject(mdc,pOld);
		DeleteDC(mdc);
		//dc.Detach();



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

void PageMemory::cvtUserToDev(double ux, double uy, int *dx, int *dy){
  *dx = (int)(defCTM[0] * ux + defCTM[2] * uy + defCTM[4] + 0.5);
  *dy = (int)(defCTM[1] * ux + defCTM[3] * uy + defCTM[5] + 0.5);
}
void PageMemory::cvtDevTouser(double dx, double dy, double *ux, double *uy){
  *ux = defICTM[0] * dx + defICTM[2] * dy + defICTM[4];
  *uy = defICTM[1] * dx + defICTM[3] * dy + defICTM[5];
}