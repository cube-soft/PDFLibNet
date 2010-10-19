#pragma once
extern "C"
{
#include "..\jpeg\jpeglib.h"
}

#define USES_CONVERSION ;

BOOL JpegFromDib(HANDLE             hDib,
                 LPBITMAPINFO       lpbi,     //Handle to DIB
                 int                nQuality, //JPEG quality (0-100)
                 const string_type& csJpeg,   //Pathname to target jpeg file
                 string_type&       pcsMsg);  //Error msg to return


RGBQUAD QuadFromWord(WORD b16);

BOOL DibToSamps(HANDLE                      hDib,
                LPBITMAPINFOHEADER          pbBmHdr,
                int                         nSampsPerRow,
                struct jpeg_compress_struct cinfo,
                JSAMPARRAY                  jsmpPixels,
                string_type&                pcsMsg,
                LPSTR                       lpBits);
