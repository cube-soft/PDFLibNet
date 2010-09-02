#include "StreamCallback.h"

StreamCallback::StreamCallback(READFROMSTREAM callback,long fullLenght, Guint startA, GBool limitedA, Guint lengthA, Object *dictA)
: BaseStream(dictA)
{
  _callback = callback;
  _fullLenght = fullLenght;
  currentPos=startA;
  start = startA;
  limited = limitedA;
  length = lengthA;
  bufPtr = bufEnd = buf;
  bufPos = start;
  savePos = 0;
  saved = gFalse;

}

StreamCallback::~StreamCallback() {
  close();  
}

Stream *StreamCallback::makeSubStream(Guint startA, GBool limitedA, Guint lengthA, Object *dictA) {
  return new StreamCallback(_callback,_fullLenght, startA, limitedA, lengthA, dictA);
}


void StreamCallback::reset() 
{
  savePos = currentPos+start;
  currentPos =start;

  saved = gTrue;
  bufPtr = bufEnd = buf;
  bufPos = start;
}


void StreamCallback::close() {
  if (saved) {
	currentPos=savePos;	
    saved = gFalse;
  }
}

GBool StreamCallback::fillBuf() {
  int n;
  gLockMutex(&gStreamMutex);
  __try { 	  
	  if(_callback==0)
		  return gFalse;
	  
	  bufPos += bufEnd - buf;
	  bufPtr = bufEnd = buf;
	  if (limited && bufPos >= start + length) 
		return gFalse;

	  if (limited && bufPos + fileStreamBufSize > start + length) {
		n = start + length - bufPos;
	  } else {
		n = fileStreamBufSize;
	  }
	  if(bufPos + n > _fullLenght)
		  n = _fullLenght - bufPos;
	  _callback((unsigned char *)buf,1,bufPos,n);
	  //n = fread(buf, 1, n, f);
	  bufEnd = buf + n;
	  if (bufPtr >= bufEnd) 
		return gFalse;
	  
	  return gTrue;
  }
  __finally 
  {
	  gUnlockMutex(&gStreamMutex);
  }
}


void StreamCallback::setPos(Guint pos, int dir) {
  if (dir >= 0) {
	currentPos = pos;
    bufPos = pos;
  } else {
	
    if (pos > _fullLenght)
      pos = (Guint)_fullLenght;
    
    bufPos = _fullLenght-pos;
	currentPos = bufPos;
  }
  bufPtr = bufEnd = buf;
}

void StreamCallback::moveStart(int delta) {
  start += delta;
  bufPtr = bufEnd = buf;
  bufPos = start;
}

/*
//------------------------------------------------------------------------
// FileStream
//------------------------------------------------------------------------

StreamCallback::StreamCallback(FILE *fA, Guint startA, GBool limitedA,
		       Guint lengthA, Object *dictA):
    BaseStream(dictA) {
  f = fA;
  start = startA;
  limited = limitedA;
  length = lengthA;
  bufPtr = bufEnd = buf;
  bufPos = start;
  savePos = 0;
  saved = gFalse;
}

StreamCallback::~StreamCallback() {
  close();
}

Stream *StreamCallback::makeSubStream(Guint startA, GBool limitedA,
				  Guint lengthA, Object *dictA) {
  return new StreamCallback(f, startA, limitedA, lengthA, dictA);
}

void StreamCallback::reset() {
#if HAVE_FSEEKO
  savePos = (Guint)ftello(f);
  fseeko(f, start, SEEK_SET);
#elif HAVE_FSEEK64
  savePos = (Guint)ftell64(f);
  fseek64(f, start, SEEK_SET);
#else
  savePos = (Guint)ftell(f);
  fseek(f, start, SEEK_SET);
#endif
  saved = gTrue;
  bufPtr = bufEnd = buf;
  bufPos = start;
}

void StreamCallback::close() {
  if (saved) {
#if HAVE_FSEEKO
    fseeko(f, savePos, SEEK_SET);
#elif HAVE_FSEEK64
    fseek64(f, savePos, SEEK_SET);
#else
    fseek(f, savePos, SEEK_SET);
#endif
    saved = gFalse;
  }
}

GBool StreamCallback::fillBuf() {
  int n;

  bufPos += bufEnd - buf;
  bufPtr = bufEnd = buf;
  if (limited && bufPos >= start + length) {
    return gFalse;
  }
  if (limited && bufPos + fileStreamBufSize > start + length) {
    n = start + length - bufPos;
  } else {
    n = fileStreamBufSize;
  }
  n = fread(buf, 1, n, f);
  bufEnd = buf + n;
  if (bufPtr >= bufEnd) {
    return gFalse;
  }
  return gTrue;
}

void StreamCallback::setPos(Guint pos, int dir) {
  Guint size;

  if (dir >= 0) {
#if HAVE_FSEEKO
    fseeko(f, pos, SEEK_SET);
#elif HAVE_FSEEK64
    fseek64(f, pos, SEEK_SET);
#else
    fseek(f, pos, SEEK_SET);
#endif
    bufPos = pos;
  } else {
#if HAVE_FSEEKO
    fseeko(f, 0, SEEK_END);
    size = (Guint)ftello(f);
#elif HAVE_FSEEK64
    fseek64(f, 0, SEEK_END);
    size = (Guint)ftell64(f);
#else
    fseek(f, 0, SEEK_END);
    size = (Guint)ftell(f);
#endif
    if (pos > size)
      pos = (Guint)size;
#ifdef __CYGWIN32__
    //~ work around a bug in cygwin's implementation of fseek
    rewind(f);
#endif
#if HAVE_FSEEKO
    fseeko(f, -(int)pos, SEEK_END);
    bufPos = (Guint)ftello(f);
#elif HAVE_FSEEK64
    fseek64(f, -(int)pos, SEEK_END);
    bufPos = (Guint)ftell64(f);
#else
    fseek(f, -(int)pos, SEEK_END);
    bufPos = (Guint)ftell(f);
#endif
  }
  bufPtr = bufEnd = buf;
}

void StreamCallback::moveStart(int delta) {
  start += delta;
  bufPtr = bufEnd = buf;
  bufPos = start;
}*/