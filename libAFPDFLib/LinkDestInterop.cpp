#include "LinkDestInterop.h"
#include "AFPDFDoc.h"
#include "AFPDFDocInterop.h"

LinkDestInterop::LinkDestInterop(void *ptr, AFPDFDocInterop *pdfdoc)
: _ptr((void *)ptr)
, _pdfDoc(pdfdoc)
{
}

LinkDestInterop::~LinkDestInterop(void)
{
}

bool LinkDestInterop::isPageRef(){
	return ((LinkDest *)_ptr)->isPageRef();
}
int LinkDestInterop::getPageNum(){
	int page=0;
	if(_ptr!=0){
		if(((LinkDest *)_ptr)->isPageRef() ){
			Ref r= ((LinkDest *)_ptr)->getPageRef();
			page=_pdfDoc->findPage(r.num,r.gen);
		}else
			page=((LinkDest *)_ptr)->getPageNum();
	}
	return page;
}
double LinkDestInterop::getLeft(){
	return ((LinkDest *)_ptr)->getLeft();
}
double LinkDestInterop::getBottom(){
	return ((LinkDest *)_ptr)->getBottom();
}
double LinkDestInterop::getRight(){
	return ((LinkDest *)_ptr)->getRight();
}
double LinkDestInterop::getTop(){
	return ((LinkDest *)_ptr)->getTop();
}
double LinkDestInterop::getZoom(){
	return ((LinkDest *)_ptr)->getZoom();
}
bool LinkDestInterop::getChangeLeft(){
	return ((LinkDest *)_ptr)->getChangeLeft();
}
bool LinkDestInterop::getChangeTop(){
	return ((LinkDest *)_ptr)->getChangeTop();
}
bool LinkDestInterop::getChangeZoom(){
	return ((LinkDest *)_ptr)->getChangeZoom();
}