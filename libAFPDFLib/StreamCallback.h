#pragma once
#include "stdafx.h"
#include "Object.h"
#include "globals.h"

#define fileStreamBufSize 8192
typedef void (__stdcall *READFROMSTREAM)(unsigned char *,int,int,int);
class StreamCallback: 
	public BaseStream {
public:

  /*StreamCallback(FILE *fA, Guint startA, GBool limitedA,
	     Guint lengthA, Object *dictA);*/
 StreamCallback(READFROMSTREAM callback,long fullLenght, Guint startA, GBool limitedA,Guint lengthA, Object *dictA);
  virtual ~StreamCallback();
  virtual Stream *makeSubStream(Guint startA, GBool limitedA,Guint lengthA, Object *dictA);
  virtual StreamKind getKind() { return strFile; }
  virtual void reset();
  virtual void close();
  virtual int getChar()
    { return (bufPtr >= bufEnd && !fillBuf()) ? EOF : (*bufPtr++ & 0xff); }
  virtual int lookChar()
    { return (bufPtr >= bufEnd && !fillBuf()) ? EOF : (*bufPtr & 0xff); }
  virtual int getPos() { return bufPos + (bufPtr - buf); }
  virtual void setPos(Guint pos, int dir = 0);
  virtual Guint getStart() { return start; }
  virtual void moveStart(int delta);
  virtual int getLength(){ return length; } 
private:

  GBool fillBuf();

  READFROMSTREAM _callback;
  long _fullLenght;
  Guint start;
  GBool limited;
  Guint length;
  char buf[fileStreamBufSize];
  char *bufPtr;
  char *bufEnd;
  Guint bufPos;
  int savePos;
  int currentPos;

  GBool saved;
};