
#include "PageLink.h"
#include "PDFWrapper.h"

namespace PDFLibNet
{

	PageLink::PageLink(LinkInterop *ptr,PDFWrapper ^pdfDoc)
		: _ptr(ptr)
		, _name(nullptr)
		, _bounds(System::Drawing::Rectangle::Empty)
		, _action(nullptr)
		, _pdfDoc(pdfDoc)
	{
	}

	PageLinkAction::PageLinkAction(LinkInterop *linkInterop,PDFWrapper ^pdfDoc)
		: _action(linkInterop->getAction())
		, _pdfDoc(pdfDoc)
	{
	}

	LinkDest ^PageLinkGoTo::FindDest(System::String ^destName){
		return _pdfDoc->FindDestination(destName);	
	}
}