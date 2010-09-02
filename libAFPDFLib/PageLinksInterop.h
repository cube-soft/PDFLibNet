#pragma once
class LinkInterop;
class AFPDFDocInterop;
class PageLinksInterop
{
private:
	void *_links;
	AFPDFDocInterop *_pdfDoc;
	void *_cache;
public:
	PageLinksInterop(void *lptr, AFPDFDocInterop *pdfdoc);
	~PageLinksInterop(void);

	LinkInterop *getLink(int iLink);
	int getLinkCount();
	bool onLink(double x, double y);

	void setPointers(int lptr,int pdfdoc);

};
