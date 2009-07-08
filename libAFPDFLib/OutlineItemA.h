#pragma once
#include "stdafx.h"

// AFPDFDoc2 command target
class OutlineItemA 
{
#ifndef _NETLIB_
private:
	OutlineItem *m_Item;

public:
	OutlineItemA();
	virtual ~OutlineItemA();
#endif
public:
	
	wchar_t *GetTitle(void);
	long GetKind(void);
	long GetKidsCount(void);
	long SetOutlineItemXPDF(long lPtr);
	long GetKidPtr(long iKid);
	long GetLinkAction(void);

	
};
