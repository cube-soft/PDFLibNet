#include "OutlineItemInterop.h" 
#include "OutlineItemA.h"


OutlineItemInterop::OutlineItemInterop(void *lptr, void *pdfdoc)
: m_Item(new OutlineItemA)
, _pdfDoc(pdfdoc)
{
	((OutlineItemA *)m_Item)->SetOutlineItemXPDF((long)lptr);
}
OutlineItemInterop::~OutlineItemInterop(){
	m_Item=0;
}
wchar_t *OutlineItemInterop::GetTitle(){
	return ((OutlineItemA *)m_Item)->GetTitle();
}
long OutlineItemInterop::GetKind(void){
	return ((OutlineItemA *)m_Item)->GetKind();
}
long OutlineItemInterop::GetKidsCount(void){
	return ((OutlineItemA *)m_Item)->GetKidsCount();
}
long OutlineItemInterop::SetOutlineItemXPDF(long lPtr){
	return ((OutlineItemA *)m_Item)->SetOutlineItemXPDF(lPtr);
}
long OutlineItemInterop::GetKidPtr(long iKid){
	return ((OutlineItemA *)m_Item)->GetKidPtr(iKid);
}
long OutlineItemInterop::GetLinkAction(void){
	return ((OutlineItemA *)m_Item)->GetLinkAction();
}

LinkDestInterop *OutlineItemInterop::getDest(){
	if(((OutlineItemA *)m_Item)->GetKind()==LinkActionKind::actionGoTo){
		LinkDestInterop *l=new LinkDestInterop(((LinkGoTo *)((OutlineItemA *)m_Item)->GetLinkAction())->getDest(),_pdfDoc);
		return l;
	}
	return 0;
}