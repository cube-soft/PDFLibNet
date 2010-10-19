#include <aconf.h>
#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "gmem.h"
#include "config.h"
#include "Error.h"
#include "GfxState.h"
#include "Object.h"
#include "Stream.h"
#include "ImagesMemoryOutputDev.h"

ImagesMemoryOutputDev::ImagesMemoryOutputDev() {
  dumpJPEG = gTrue;
  imgNum = 0;
  ok = gTrue;
}

ImagesMemoryOutputDev::~ImagesMemoryOutputDev() {
	_Images.Clear();
  //gfree(fileName);
  //gfree(fileRoot);
}

void ImagesMemoryOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str, int width, int height, GBool invert, GBool inlineImg) {
	int c;
	int size, i;
	ImageInfo *img =new ImageInfo();
	int initBufSize=4096;
	int bufSize=initBufSize;
	++imgNum;
	byte *buff=0;
	img->bytes =(byte *)malloc(bufSize);
	if(!img->bytes)
					throw "Memory allocation failure!";
	img->height=height;
	img->width =width;
	img->name=0;
	/*
	img->name=new char[strlen(ref->getName())];
	sprintf(img->name,"%s",ref->getName());*/
	img->ismask=true;
	img->index=imgNum;
	img->type=str->getKind() == strDCT && !inlineImg;
	int count=0;
  // dump JPEG file
  if (dumpJPEG && str->getKind() == strDCT && !inlineImg) {
    
    // initialize stream
    str = ((DCTStream *)str)->getRawStream();
    str->reset();
		
    // copy the stream
    while ((c = str->getChar()) != EOF)
	{
		//Resize buffer preserving items
		if(count+1>bufSize){
			bufSize+=initBufSize;
			img->bytes=(byte *)realloc(img->bytes,bufSize+1);
			if(!img->bytes)
					throw "Memory allocation failure!";
		}
		img->bytes[count]=c;
		count++;
	}
	img->size=count;
	img->numcomps=3;
    str->close();
  // dump PBM file (P4)
  } else {
    // initialize stream
    str->reset();

    // copy the stream
    size = height * ((width + 7) / 8);
    for (i = 0; i < size; ++i) {
      c = str->getChar();
	  //Resize buffer preserving items
		if(count+1>bufSize){
			bufSize+=initBufSize;
			img->bytes=(byte *)realloc(img->bytes,bufSize+1);
			if(!img->bytes)
					throw "Memory allocation failure!";
		}
		img->bytes[count]=c;
		count++;
    }
	img->size=count;
	img->numcomps=3;
    str->close();
  }
  _Images.Add(*img);
}
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

#define RGB(r, g ,b)  ((DWORD) (((BYTE) (r) | ((WORD) (g) << 8)) | (((DWORD) (BYTE) (b)) << 16))) 

void ImagesMemoryOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,int width, int height,GfxImageColorMap *colorMap,int *maskColors, GBool inlineImg) 
{
  ImageStream *imgStr;
  Guchar *p;
  GfxRGB rgb;
  int x, y;
  int c;
  int size, i;

  
	ImageInfo *img =new ImageInfo();
	int initBufSize=16384;
	int bufSize=initBufSize;
	
	byte *buff=0;
	img->bytes =(byte *)malloc(bufSize);
	if(!img->bytes)
		throw "Memory allocation failure!";
	img->height=height;
	img->width =width;

	
	img->name=0;
	//if(ref && ref->getName()){
		//img->name=new char[1024];
		//sprintf(img->name,"%s",ref->getName());
	//}
	img->ismask=true;
	img->index=imgNum;
	img->type=0;

	int count=0;
	
  // dump JPEG file
  if (dumpJPEG && str->getKind() == strDCT &&
      (colorMap->getNumPixelComps() == 1 ||
       colorMap->getNumPixelComps() == 3) &&
      !inlineImg) {

    // initialize stream
    str = ((DCTStream *)str)->getRawStream();
    str->reset();

    // copy the stream
    while ((c = str->getChar()) != EOF)
	{
		//Resize buffer preserving items
		if(count+1>bufSize){
			bufSize+=initBufSize;
			img->bytes=(byte *)realloc(img->bytes,bufSize+1);
			if(!img->bytes)
					throw "Memory allocation failure!";
		}
		img->bytes[count]=c;
		count++;
	}
	img->size=count;
    str->close();

  // dump PBM file (P4)
  } else if (colorMap->getNumPixelComps() == 1 &&
	     colorMap->getBits() == 1) {

	img->type=1;
    // initialize stream
    str->reset();

    // copy the stream
	fprintf(stderr,"Copy Stream PBM");
    size = height * ((width + 7) / 8);
    for (i = 0; i < size; ++i) {
		c  =str->getChar() ^ 0xff;
		//Resize buffer preserving items
		if(count+1>bufSize){
			bufSize+=initBufSize;
			img->bytes=(byte *)realloc(img->bytes,bufSize+1);
			if(!img->bytes)
					throw "Memory allocation failure!";
		}
		img->bytes[count]=c;
		count++;
    }
	img->size=count;
    str->close();

  // dump PPM file (P6)
  } else {
	img->type=2;
    // initialize stream
    imgStr = new ImageStream(str, width, colorMap->getNumPixelComps(),colorMap->getBits());
    imgStr->reset();
	img->numcomps = colorMap->getNumPixelComps();
    // for each line...
	for (y = 0; y < height; ++y) 
	{
		// write the line
		p = imgStr->getLine();
		for (x = 0; x < width; ++x) 
		{
			colorMap->getRGB(p, &rgb);
			
			//Resize buffer preserving items
			if((count+colorMap->getNumPixelComps()+1)>bufSize){
				bufSize+=initBufSize;
				img->bytes=(byte *)realloc(img->bytes,bufSize+1);
				if(!img->bytes)
					throw "Memory allocation failure!";
			}
			//byte rgbc = RGB(colToByte(rgb.r),colToByte(rgb.g),colToByte(rgb.b));
			//img->bytes[count]= rgbc;
			//we need a bitmap of 24 bits
			img->bytes[count]  =(byte)colToByte(rgb.r);
			
			if(img->numcomps==1){
				GfxColor colr;
				colorMap->getColor(p,&colr);
				img->bytes[count]= RGB(colToByte(rgb.r),colToByte(rgb.g),colToByte(rgb.b));
			}
			if(img->numcomps>=2)
				img->bytes[count+1]=(byte)colToByte(rgb.g);
			if(img->numcomps>=3)
				img->bytes[count+2]=(byte)colToByte(rgb.b);
			if(img->numcomps==4)
				img->bytes[count+3]=255;
			
			p += colorMap->getNumPixelComps();
			count+=colorMap->getNumPixelComps();
		}
	}
	img->size=count;
	
    delete imgStr;
  }
  _Images.Add(*img);
}

int ImagesMemoryOutputDev::getImageCount(){
	return _Images.GetCount();
}

int ImagesMemoryOutputDev::getImageNumComps(int imgIndex){
	if(imgIndex>=0 && imgIndex<_Images.GetCount()){
		ImageInfo *img=&_Images[imgIndex];
		return img->numcomps;
	}
	return 0;
}

byte *ImagesMemoryOutputDev::getImageBytes(int imgIndex){
	if(imgIndex>=0 && imgIndex<_Images.GetCount()){
		ImageInfo *img=&_Images[imgIndex];
		return img->bytes;
	}
	return 0;
}

int ImagesMemoryOutputDev::getImageSize(int imgIndex){
	if(imgIndex>=0 && imgIndex<_Images.GetCount()){
		ImageInfo *img=&_Images[imgIndex];
		return img->size;
	}
	return 0;
}

int ImagesMemoryOutputDev::getImageType(int imgIndex){
	if(imgIndex>=0 && imgIndex<_Images.GetCount()){
		ImageInfo *img=&_Images[imgIndex];
		return img->type;
	}
	return -1;
}


int ImagesMemoryOutputDev::getImageHeight(int imgIndex){
	if(imgIndex>=0 && imgIndex<_Images.GetCount()){
		ImageInfo *img=&_Images[imgIndex];
		return img->height;
	}
	return 0;
}

int ImagesMemoryOutputDev::getImageWidth(int imgIndex){
	if(imgIndex>=0 && imgIndex<_Images.GetCount()){
		ImageInfo *img=&_Images[imgIndex];
		return img->width;
	}
	return 0;
}

char *ImagesMemoryOutputDev::getImageName(int imgIndex){
		if(imgIndex>=0 && imgIndex<_Images.GetCount()){
		ImageInfo *img=&_Images[imgIndex];
		return img->name;
	}
	return 0;
}