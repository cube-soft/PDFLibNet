
#include "OutlineItemA.h"

OutlineItemA::OutlineItemA() : m_Item(NULL)
{
	
}

OutlineItemA::~OutlineItemA()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	m_Item = NULL;
	
}


// OutlineItemA message handlers
wchar_t * OutlineItemA::GetTitle(void)
{
	

	CString strResult;
	char * sztitle=NULL;
	if(m_Item){
		Unicode *title=m_Item->getTitle();
		char * sztitle;
		int len = m_Item->getTitleLength();

		/* FIXME: should we procedd UCS4 better than just stripping? */
		sztitle = new char[len + 1];
		int k = 0;
		for(k = 0; k < len; k++)
			sztitle[k] = title[k];

		sztitle[k] = 0;

		strResult = sztitle;
	}
	//return sztitle;	
	return strResult.AllocSysString();
}

long OutlineItemA::GetKind(void)
{
	
	if (m_Item){
		return m_Item->getAction()->getKind();
	}
	return -1;
}

long OutlineItemA::GetKidsCount(void)
{
	
	if(m_Item){
		if(m_Item->hasKids()){
			m_Item->open();
			return m_Item->getKids()->getLength();
		}
	}

	return 0;
}

long OutlineItemA::SetOutlineItemXPDF(long lPtr)
{
	
	m_Item =(OutlineItem *)lPtr;
	return 0;
}

long OutlineItemA::GetKidPtr(long iKid)
{
	
	
	if(m_Item && m_Item->getKids() && m_Item->getKids()->getLength()>0)
	{
		if(iKid>0 && iKid<= m_Item->getKids()->getLength()){
			return (long) m_Item->getKids()->get(iKid-1);
		}
	}
	return 0;
}

long OutlineItemA::GetLinkAction(void)
{
	

	if(m_Item)
		return (long)m_Item->getAction();
	return 0;
}
