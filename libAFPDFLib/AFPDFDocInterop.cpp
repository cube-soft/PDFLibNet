#pragma once
#include "AFPDFDocInterop.h"
#include "AFPDFDoc.h"

AFPDFDocInterop::AFPDFDocInterop()
: _ptr(new AFPDFDoc)
, _resultsPtr(0)
, _pageLinks(0,0)
{
}
AFPDFDocInterop::~AFPDFDocInterop(){
	delete _ptr;
	free(_resultsPtr);
}
long AFPDFDocInterop::LoadFromFile(char * sFileName){
	return ((AFPDFDoc *)_ptr)->LoadFromFile(sFileName);
}
long AFPDFDocInterop::RenderPage(long lhWnd){
	return ((AFPDFDoc *)_ptr)->RenderPage(lhWnd);
}
long AFPDFDocInterop::GetCurrentX(void){
	return ((AFPDFDoc *)_ptr)->GetCurrentX();
}
void AFPDFDocInterop::SetCurrentX(long newVal){
	((AFPDFDoc *)_ptr)->SetCurrentX(newVal);
}
long AFPDFDocInterop::GetCurrentY(void){
	return ((AFPDFDoc *)_ptr)->GetCurrentY();
}
void AFPDFDocInterop::SetCurrentY(long newVal){
	((AFPDFDoc *)_ptr)->SetCurrentY(newVal);
}
void AFPDFDocInterop::ZoomIN(){
	((AFPDFDoc *)_ptr)->ZoomIN();
}
void AFPDFDocInterop::ZoomOUT(){
	((AFPDFDoc *)_ptr)->ZoomOut();
}

long AFPDFDocInterop::GetCurrentPage(void){
	return ((AFPDFDoc *)_ptr)->GetCurrentPage();
}
void AFPDFDocInterop::SetCurrentPage(long newVal){
	return ((AFPDFDoc *)_ptr)->SetCurrentPage(newVal);
}
long AFPDFDocInterop::RenderBitmap(long lhWnd){
	return ((AFPDFDoc *)_ptr)->RenderBitmap(lhWnd);
}
long AFPDFDocInterop::GetPageCount(void){
	return ((AFPDFDoc *)_ptr)->GetPageCount();
}
long AFPDFDocInterop::FitScreenWidth(long lhWnd){
	return ((AFPDFDoc *)_ptr)->FitScreenWidth(lhWnd);
}
long AFPDFDocInterop::FitScreenHeight(long lhWnd){
	return ((AFPDFDoc *)_ptr)->FitScreenHeight(lhWnd);
}
long AFPDFDocInterop::GetPageWidth(void){
	return ((AFPDFDoc *)_ptr)->GetPageWidth();
}
long AFPDFDocInterop::GetPageHeight(void){
	return ((AFPDFDoc *)_ptr)->GetPageHeight();
}

long AFPDFDocInterop::GetOutlineCount(void){
	return ((AFPDFDoc *)_ptr)->GetOutlineCount();
}
long AFPDFDocInterop::GetOutlinePtr(long iOutline){
	return ((AFPDFDoc *)_ptr)->GetOutlinePtr(iOutline);
}
long AFPDFDocInterop::ProcessLinkAction(long lPtrLinkAction){
	return ((AFPDFDoc *)_ptr)->ProcessLinkAction(lPtrLinkAction);
}	
void AFPDFDocInterop::Dispose(){
	((AFPDFDoc *)_ptr)->Dispose();
}
float AFPDFDocInterop::GetRenderDPI()
{
	return ((AFPDFDoc *)_ptr)->GetRenderDPI();
}
void AFPDFDocInterop::SetRenderDPI(float newVal){
	((AFPDFDoc *)_ptr)->SetRenderDPI(newVal);
}
wchar_t *AFPDFDocInterop::GetPDFVersion(void){
	return ((AFPDFDoc *)_ptr)->GetPDFVersion();
}
long AFPDFDocInterop::FindString(const char *sText, long iPage, long SearchOrder, bool bCaseSensitive, bool bBackward, bool bMarkAll, bool bWholeDoc){
	return ((AFPDFDoc *)_ptr)->FindText(sText,iPage,SearchOrder,bCaseSensitive,bBackward,bMarkAll,bWholeDoc);
}
long AFPDFDocInterop::FindNext(const char *sText){
	return ((AFPDFDoc *)_ptr)->FindNext(sText);
}
long AFPDFDocInterop::FindPrior(const char *sText){
	return ((AFPDFDoc *)_ptr)->FindPrior(sText);
}
long AFPDFDocInterop::FindFirst(const char *sText, long SearchOrder, bool Backward){
	return ((AFPDFDoc *)_ptr)->FindFirst(sText,SearchOrder,Backward);
}
long AFPDFDocInterop::RenderHDC(long lHdc){
	return ((AFPDFDoc *)_ptr)->RenderHDC(lHdc);
}
long AFPDFDocInterop::GetSearchPage(void){
	return ((AFPDFDoc *)_ptr)->GetSearchPage();
}
long AFPDFDocInterop::SetSearchPage(long lNewValue){
	return ((AFPDFDoc *)_ptr)->SetSearchPage(lNewValue);
}
bool AFPDFDocInterop::GetSearchCaseSensitive(void){
	return ((AFPDFDoc *)_ptr)->GetSearchCaseSensitive();
}
void AFPDFDocInterop::SetSearchCaseSensitive(bool newVal){
	((AFPDFDoc *)_ptr)->SetSearchCaseSensitive(newVal);
}

 void AFPDFDocInterop::NextPage(){
	((AFPDFDoc *)_ptr)->NextPage();
}
 void AFPDFDocInterop::PreviousPage(){
	((AFPDFDoc *)_ptr)->PreviousPage();
}

 SearchResultInterop *AFPDFDocInterop::GetSearchResults(){
	 CPDFSearchResult *res=((AFPDFDoc *)_ptr)->GetSearchResults();
	 long count = ((AFPDFDoc *)_ptr)->GetSearchCount();
	 if(_resultsPtr!=0)
		 free(_resultsPtr);
	 _resultsPtr = (SearchResultInterop *)malloc(count*sizeof(SearchResultInterop));
	 for(int i=0;i<count;i++){
		 SearchResultInterop *item=new SearchResultInterop(res[i].PageFound,res[i].left,res[i].top,res[i].right,res[i].bottom);
		 _resultsPtr[i]=*item;
	 }
	 return (SearchResultInterop *)_resultsPtr;
 }

 long AFPDFDocInterop::GetSearchCount(){
	 return ((AFPDFDoc *)_ptr)->GetSearchCount();
 }

 long AFPDFDocInterop::PrintToFile(const char *fileName,int fromPage,int toPage){
	 return ((AFPDFDoc *)_ptr)->PrintToFile(fileName,fromPage,toPage);
 }

 double AFPDFDocInterop::GetZoom(){
	return  (double)((AFPDFDoc *)_ptr)->GetRenderDPI();	 
 }
 void AFPDFDocInterop::SetZoom(double newValue){
	((AFPDFDoc *)_ptr)->SetRenderDPI((FLOAT)newValue);
 }

 char * AFPDFDocInterop::GetTitle(){
	return ((AFPDFDoc *)_ptr)->getTitle();
 }

 char * AFPDFDocInterop::GetAuthor(){
	return ((AFPDFDoc *)_ptr)->getAuthor();
 }

 PageLinksInterop *AFPDFDocInterop::getPageLinksInterop(long iPage){
	 if(iPage>=0 && iPage<this->GetPageCount()){
		PageLinksInterop *pl =new PageLinksInterop(((AFPDFDoc *)_ptr)->GetLinksPage(iPage),_ptr);
		return pl;
	 }
	 return 0;
 }

 void AFPDFDocInterop::cvtUserToDev(double ux, double uy, int *dx, int *dy){
		((AFPDFDoc *)_ptr)->cvtUserToDev(ux,uy,dx,dy);	
	}

	void AFPDFDocInterop::cvtDevToUser(double ux, double uy, double *dx, double *dy){
		((AFPDFDoc *)_ptr)->cvtDevToUser(ux,uy,dx,dy);
	}