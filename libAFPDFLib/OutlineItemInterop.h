#pragma once
#include "LinkDestInterop.h"

class AFPDFDocInterop;
class OutlineItemInterop
{
private:
	void *m_Item;
	AFPDFDocInterop *_pdfDoc;
public:
	OutlineItemInterop(void *lPtr, AFPDFDocInterop *pdfdoc);
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