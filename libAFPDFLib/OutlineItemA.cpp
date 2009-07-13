
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
	//UnicodeMap *uMap = globalParams->getTextEncoding(); //getUnicodeMap(&GString("GBK"));
	wchar_t *ret;
	int j,n;
	//char buf[8];
	//GString *title=new GString();
	char *s;
	if(m_Item /*&& uMap!=NULL*/){
		ret =new wchar_t[m_Item->getTitleLength()];
		//12/July/2009 - Allow windows to map unicode characters, 
		for (j = 0; j < m_Item->getTitleLength(); j++) {
		  //n = uMap->mapUnicode(m_Item->getTitle()[j], buf, sizeof(buf));
		  //title->append(buf, n);
		  ret[j]=(wchar_t)m_Item->getTitle()[j];
		}
		ret[j]='\0';
		//s = title->getCString();		
	}else{
		return L"\0";
	}
	//USES_CONVERSION;
	//ret =A2W(s);
	return ret;
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
