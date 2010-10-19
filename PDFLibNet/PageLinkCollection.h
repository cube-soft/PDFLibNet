#pragma once
#include "PageLink.h"

namespace PDFLibNet
{

	ref class PDFWrapper;

	generic<class T>
	where T:PageLink
	public ref class PageLinkCollection 
		:public System::Collections::Generic::List<T>
	{
	private:
		PDFWrapper ^_pdfDoc;
	internal:
		PageLinkCollection(PDFWrapper ^wrapper);
	
	};
};