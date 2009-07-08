#pragma once
#include "LinkDestInterop.h"

class OutlineItemInterop
{
private:
	void *m_Item;
	void *_pdfDoc;
public:
	OutlineItemInterop(void *lPtr,void *pdfdoc);
	virtual ~OutlineItemInterop();
public:
	wchar_t *GetTitle(void);
	long GetKind(void);
	long GetKidsCount(void);
	long SetOutlineItemXPDF(long lPtr);
	long GetKidPtr(long iKid);
	long GetLinkAction(void);
	LinkDestInterop *getDest();
};