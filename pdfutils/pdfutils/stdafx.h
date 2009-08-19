// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif			

//#include <afxwin.h>         // MFC core and standard components
#include <shlobj.h>
#include <atlstr.h>         // MFC core and standard components
#include <aconf.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <math.h>

// TODO: reference additional headers your program requires here
#include "parseargs.h"
#include "GString.h"
#include "gmem.h"
#include "GlobalParams.h"
#include "Object.h"
#include "Stream.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "PDFDoc.h"
#include "CharTypes.h"
#include "UnicodeMap.h"
#include "PDFDocEncoding.h"
#include "TextOutputDev.h"
#include "Error.h"
#include "config.h"
#include "SplashBitmap.h"
#include "Splash.h"
#include "SplashOutputDev.h"