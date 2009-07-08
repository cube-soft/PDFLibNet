#pragma once

class LinkDestInterop
{
private:
	void *_ptr;
	void *_pdfDoc;
public:
	LinkDestInterop(void *ptr, void *pdfdoc);
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
