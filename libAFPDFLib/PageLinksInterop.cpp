#include "PageLinksInterop.h"
#include "LinkInterop.h"
#include "AFPDFDoc.h"


PageLinksInterop::PageLinksInterop(void *lptr, void *pdfdoc)
: _links(lptr)
, _pdfDoc(pdfdoc)
, _cache(0)
{
	//_cache=malloc(this->getLinkCount()*sizeof(LinkInterop));
	//memset(_cache,0,this->getLinkCount()*sizeof(LinkInterop));
}

PageLinksInterop::~PageLinksInterop(void)
{
	//free(_cache);
}

LinkInterop *PageLinksInterop::getLink(int iLink){
	if(iLink>=0 && iLink<this->getLinkCount()){
		LinkInterop *l=new LinkInterop(((Links *)_links)->getLink(iLink),_pdfDoc);
		return l;
	}
	return 0;
}
int PageLinksInterop::getLinkCount(){
	return ((Links *)_links)->getNumLinks();
}
bool PageLinksInterop::onLink(double x, double y){
	
	return ((Links *)_links)->onLink(x,y)?true:false;
}

void PageLinksInterop::setPointers(int lptr,int pdfdoc){
	_pdfDoc=(void *)pdfdoc;
	_links =(void *)lptr;
	
//	free(_cache);
//	_cache=malloc(this->getLinkCount()*sizeof(LinkInterop));
//	memset(_cache,0,this->getLinkCount()*sizeof(LinkInterop));
}
