#include "afpdfdoc.h"
#include "XPDFParamsInterop.h"

#include "globalParams.h"
#include "unicodemap.h"
#include "GString.h"

extern GlobalParams *globalParams;
XPDFParamsInterop::XPDFParamsInterop()
{
	if(globalParams==0)
		InitGlobalParams("xpdfrc");
	
}

XPDFParamsInterop::XPDFParamsInterop(char *configFile)
{
	OpenFile(configFile);
}

XPDFParamsInterop::~XPDFParamsInterop(void)
{
}

void XPDFParamsInterop::OpenFile(char *configFile){
	if(globalParams!=0){
		delete globalParams;
		globalParams=0;
	}
	InitGlobalParams(configFile);
}

void  XPDFParamsInterop::setAntialias(bool antialias){
	globalParams->setAntialias(antialias?"yes":"no");
}
bool XPDFParamsInterop::getAntialias(){
	return globalParams->getAntialias()?true:false;
}
void  XPDFParamsInterop::setVectorAntialias(bool antialias){
	globalParams->setVectorAntialias(antialias?"yes":"no");
}
bool XPDFParamsInterop::getVectorAntialias(){
	return globalParams->getVectorAntialias()?true:false;
}
void  XPDFParamsInterop::setPrintError(bool printError){
	globalParams->setErrQuiet(printError?0:1);
}
bool XPDFParamsInterop::getPrintError(){
	return globalParams->getErrQuiet()?true:false;
}
void  XPDFParamsInterop::setPrintErrorFile(char *fileName){
	freopen( fileName, "w", stderr );
	freopen( fileName, "w", stdout );
}

void  XPDFParamsInterop::setEncoding(char *encoding){
	globalParams->setTextEncoding(encoding);
}
char *XPDFParamsInterop::getEncoding(){
	return globalParams->getTextEncoding()->getEncodingName()->getCString();
}
void  XPDFParamsInterop::setTextPageBreaks(bool textpagebreak){
	globalParams->setTextPageBreaks(textpagebreak?1:0);
}
bool XPDFParamsInterop::getTextPageBreaks(){
	return globalParams->getTextPageBreaks()==gTrue?true:false;
}
void  XPDFParamsInterop::setTextEOL(bool textEol){
	globalParams->setTextEOL(textEol?"yes":"no");
}
bool XPDFParamsInterop::getTextEOL(){
	return globalParams->getTextEOL()==gTrue?true:false;
}
void  XPDFParamsInterop::setEnableFreeType(bool enableFreeType){
	globalParams->setEnableFreeType(enableFreeType?"yes":"no");
}
bool XPDFParamsInterop::getEnableFreeType(){
	return globalParams->getEnableFreeType()==gTrue?true:false;
}
