#pragma once

class AFPDFDocInterop;
class LinkDestInterop
{
private:
	void *_ptr;
	AFPDFDocInterop *_pdfDoc;
public:
	LinkDestInterop(void *ptr, AFPDFDocInterop *pdfdoc);
	~LinkDestInterop(void);

	bool isPageRef();
	int getPageNum();
	double getLeft();
	double getBottom();
	double getRight();
	double getTop();
	double getZoom();
	bool getChangeLeft();
	bool getChangeTop();
	bool getChangeZoom();

};
