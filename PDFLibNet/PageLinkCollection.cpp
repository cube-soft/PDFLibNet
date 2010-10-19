#include "PageLinkCollection.h"

namespace PDFLibNet
{
	generic<class T>
	PageLinkCollection<T>::PageLinkCollection(PDFWrapper ^wrapper)
	: _pdfDoc(wrapper)
	{
	}
}