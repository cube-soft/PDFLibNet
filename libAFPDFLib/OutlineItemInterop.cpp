#include "OutlineItemInterop.h" 
#include "OutlineItemA.h"
#include "AFPDFDocInterop.h"

OutlineItemInterop::OutlineItemInterop(void *lptr, AFPDFDocInterop *pdfdoc)
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
	if(m_Item!=0)
		return ((OutlineItemA *)m_Item)->GetKidsCount();
	return 0;
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
		LinkGoTo *lnk = (LinkGoTo *)((OutlineItemA *)m_Item)->GetLinkAction();
		LinkDest * dest = lnk->getDest();
		GString  * namedDest;
		if(dest==NULL){
			if ((namedDest = lnk->getNamedDest()) == NULL)
				return 0;
			return _pdfDoc->findDest(namedDest->getCString());
		}

		return new LinkDestInterop(dest,_pdfDoc);
	}
	return 0;
}