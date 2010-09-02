// AFPDFLib.h : main header file for the AFPDFLib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CAFPDFLibApp
// See AFPDFLib.cpp for the implementation of this class
//

class CAFPDFLibApp : public CWinApp
{
public:
	CAFPDFLibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
