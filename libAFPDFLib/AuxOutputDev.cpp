#include "AuxOutputDev.h"

AuxOutputDev::AuxOutputDev(SplashOutputDev *splash)
: _data_ptr(NULL)
, _splash(splash)
, _ctm(NULL)
, _ictm(NULL)
, _width(0)
, _height(0)
, _antialias(gTrue)
{
}

void *AuxOutputDev::GetDataPtr(){	return _data_ptr;}
void AuxOutputDev::SetDataPtr(void *data){	_data_ptr = data;}
double *AuxOutputDev::getDefCTM() {	return _ctm;}
double *AuxOutputDev::getDefICTM() { return _ictm;}
void AuxOutputDev::setDefCTM(double *ctm){ _ctm =ctm; }
void AuxOutputDev::setDefICTM(double *ictm){ _ictm=ictm; }

void AuxOutputDev::getModRegion(int *xMin,int *yMin, int *xMax, int *yMax){
	_splash->getModRegion(xMin,yMin,xMax,yMax);
}
void AuxOutputDev::setModRegion(int xMin,int yMin, int xMax, int yMax){

}

	
double AuxOutputDev::GetWidth(){ return _width; } 
double AuxOutputDev::GetHeight(){return _height; }

	//xpdf
SplashOutputDev *AuxOutputDev::getSplash(){ return _splash; }
void AuxOutputDev::startDoc(XRef *xrefA){ _splash->startDoc(xrefA); }
void AuxOutputDev::clearModRegion(){ _splash->clearModRegion(); }
void AuxOutputDev::setVectorAntialias(GBool antialias){ _splash->setVectorAntialias(antialias);}

// Convert between device and user coordinates.
void AuxOutputDev::cvtDevToUser(double dx, double dy, double *ux, double *uy){
	return _splash->cvtDevToUser(dx,dy,ux,uy);
}
void AuxOutputDev::cvtUserToDev(double ux, double uy, int *dx, int *dy){
	return _splash->cvtUserToDev(ux,uy,dx,dy);
}
	
#ifdef _MUPDF
	//muPDF
void AuxOutputDev::startDoc(pdf_xref *xrefA)
{}

#endif