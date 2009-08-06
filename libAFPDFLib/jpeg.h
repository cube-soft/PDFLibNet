#pragma once
extern "C"
{
#include "..\jpeg\jpeglib.h"
}

BOOL JpegFromDib(HANDLE			hDib,
				 LPBITMAPINFO	lpbi,     //Handle to DIB
                 int			nQuality, //JPEG quality (0-100)
				 CString		csJpeg,   //Pathname to target jpeg file
                 CString	*	pcsMsg);  //Error msg to return


RGBQUAD QuadFromWord(WORD b16);

BOOL DibToSamps(HANDLE                      hDib,
				LPBITMAPINFOHEADER			pbBmHdr,
                int                         nSampsPerRow,
                struct jpeg_compress_struct cinfo,
                JSAMPARRAY                  jsmpPixels,
                CString *                   pcsMsg,
				LPSTR						lpBits);