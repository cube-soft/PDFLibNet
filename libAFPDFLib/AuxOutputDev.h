#pragma once
#include "stdafx.h"

#ifdef _MUPDF
extern "C"
{
	#include <fitz.h>
	#include <mupdf.h>
}
#endif

class AuxOutputDev
{
private:
	void *_data_ptr;
	SplashOutputDev *_splash;
	double *_ctm;
	double *_ictm;
	double _width;
	double _height;
	bool _antialias;

public:
	AuxOutputDev(SplashOutputDev *splash);
	~AuxOutputDev();

	void *GetDataPtr();
	void SetDataPtr(void *data);

	double *getDefCTM();
	double *getDefICTM();
	void setDefCTM(double *);
	void setDefICTM(double *);

	void getModRegion(int *xMin,int *yMin, int *xMax, int *yMax);
	void setModRegion(int xMin,int yMin, int xMax, int yMax);

	

	double GetWidth();
	double GetHeight();
	void setSize(double w, double h){ _width = w; _height = h; }

	//xpdf
	SplashOutputDev *getSplash();
	void startDoc(XRef *xrefA);
	void clearModRegion();
	void setVectorAntialias(GBool antialias);
	// Convert between device and user coordinates.
	virtual void cvtDevToUser(double dx, double dy, double *ux, double *uy);
	virtual void cvtUserToDev(double ux, double uy, int *dx, int *dy);
	
#ifdef _MUPDF
	//muPDF
	void startDoc(pdf_xref *xrefA);
	void setPixmap(fz_pixmap *pixmap){ _pixmap=pixmap; }
private:
	fz_pixmap *_pixmap;
#endif
};