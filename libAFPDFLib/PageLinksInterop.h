#pragma once
class LinkInterop;

class PageLinksInterop
{
private:
	void *_links;
	void *_pdfDoc;
	void *_cache;
public:
	PageLinksInterop(void *lptr, void *pdfdoc);
	~PageLinksInterop(void);

	LinkInterop *getLink(int iLink);
	int getLinkCount();
	bool onLink(double x, double y);

	void setPointers(int lptr,int pdfdoc);

};
