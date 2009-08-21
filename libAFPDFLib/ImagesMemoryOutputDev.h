//========================================================================
//
// ImageOutputDev.h
//
// Copyright 1998-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef IMAGEOUTPUTDEV_H
#define IMAGEOUTPUTDEV_H

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include <stdio.h>
#include "gtypes.h"
#include "OutputDev.h"
#include "DynArray.h"
class GfxState;
typedef unsigned char byte;
//------------------------------------------------------------------------
// ImageOutputDev
//------------------------------------------------------------------------
struct ImageInfo {
	byte *bytes;
	char *name;
	int width;
	int height;
	int index;
	int type; //0=JPG, 1=PBM, 2=PPM
	int size;
	int numcomps;
	GBool ismask;
};

class ImagesMemoryOutputDev: public OutputDev {
public:

  // Create an OutputDev which will write images to files named
  // <fileRoot>-NNN.<type>.  Normally, all images are written as PBM
  // (.pbm) or PPM (.ppm) files.  If <dumpJPEG> is set, JPEG images are
  // written as JPEG (.jpg) files.
  ImagesMemoryOutputDev();

  // Destructor.
  virtual ~ImagesMemoryOutputDev();

  // Check if file was successfully created.
  virtual GBool isOk() { return ok; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() { return gFalse; }

  // Does this device need non-text content?
  virtual GBool needNonText() { return gTrue; }

  //---- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() { return gFalse; }

  //----- image drawing
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg);
  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			 int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg);


  int getImageCount();
  byte *getImageBytes(int imgIndex);
  char *getImageName(int imgIndex);
  int getImageWidth(int imgIndex);
  int getImageHeight(int imgIndex);
  int getImageSize(int imgIndex);
  int getImageType(int imgIndex);
  int getImageNumComps(int imgIndex);
  void exportToFile(wchar_t *fileName, int quality=90);
private:
	DynArray<ImageInfo> _Images;
	GBool dumpJPEG;		// set to dump native JPEG files
	int imgNum;			// current image number
	GBool ok;			// set up ok?
};

#endif
