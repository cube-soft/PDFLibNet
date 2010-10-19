#include "OutlineItem.h"
#include "OutlineItemCollection.h"
#include "PDFWrapper.h"
namespace PDFLibNet {

	OutlineItem::OutlineItem(int ptr, PDFWrapper ^pdfDoc)
		: _outline(new OutlineItemInterop((void *)ptr,pdfDoc->_pdfDoc))
		, _pdfDoc(pdfDoc)
		, _childrens(nullptr)
		, _title(nullptr)
		, _dest(nullptr)
	{
	}


	PDFLibNet::LinkActionKind OutlineItem::GetKind(void){
		long kind =_outline->GetKind();
		return (LinkActionKind)Enum::ToObject(LinkActionKind::typeid,kind);
	}

	System::Int32 OutlineItem::GetKidPtr(long i){
		return _outline->GetKidPtr(i);
	}

	void OutlineItem::LoadChildrens()
	{
		if(_childrens==nullptr){
			_childrens = gcnew OutlineItemCollection<OutlineItem ^>();
			for(long i=1;i<=this->KidsCount;i++)
				_childrens->Add(gcnew OutlineItem(this->GetKidPtr(i),_pdfDoc));
		}
	}

	long OutlineItem::DoAction()
	{
		long lptr = _outline->GetLinkAction();
		return _pdfDoc->PerfomLinkAction(lptr);
	}

};