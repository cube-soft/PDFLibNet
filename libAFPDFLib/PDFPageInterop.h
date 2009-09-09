#pragma once
#include "PageLinksInterop.h"

class PDFPageInterop 
{
	
private:
	void *_images;
	void *_pdfDoc;			//AFPDFDoc
	void *_pdfPage;			//Page
	void *_selectionArray;  //DynArray
	void *_text;				// TextPage: extracted text
	double dpi;
	int _page;
	double ctm[6];
	double _ctmdpi;
private:
	void takeText();
	void CvtUserToDev(int xu,int yu,int *xd,int *yd);
	void CvtUserToDevDpi(double dpi,int xu,int yu,int *xd,int *yd);
public:
	PDFPageInterop(int page, void *lptr,void *pdfDoc);
	~PDFPageInterop(void);

	PageLinksInterop *getLinks();
	int getPage();
	void getTextStream(void *stream);
	wchar_t *getText();
	wchar_t *extractText(); //Extract selected text
	wchar_t *extractText(int x1, int y1, int x2, int y2); //Extract text from coordinates
	bool hasText(int x, int y);	//Return true if the point is in text bounds
	void extractImages();
	int getImagesCount();
	unsigned char *getImageBytes(int index);
	int getImageSize(int index);
	int getImageType(int index);
	int getImageWidth(int index);
	int getImageHeight(int index);
	int getImageNumComps(int index);
	//bool hasImage(int x, int y); //Return true if the point is in image bounds
	//void getImageBox(int x, int y, int *x1,int *y1, int *x2, int *y2); //Return the image bounds
	//void exportImage(wchar_t *fileName,int quality,int x1, int y1, int x2, int y2);
	void getTextCharBox(int x, int y, int *x1,int *y1, int *x2, int *y2);
	void getTextWordBox(int x, int y, int *x1,int *y1, int *x2, int *y2);
	void addSelection(double dpi, int x1, int y1, int x2, int y2);
	void clearSelection();
	double getPageWidth();
	double getPageHeight();
	char *PDFPageInterop::getISOANum();
	//void *getFonts();
	void RenderSelection(double dpi, long hdc,unsigned long color,unsigned long linecolor,int linewidth);

};