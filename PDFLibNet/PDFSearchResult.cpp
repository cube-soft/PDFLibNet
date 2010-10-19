
#include "PDFSearchResult.h"

namespace PDFLibNet
{
	PDFSearchResult::PDFSearchResult(long page,int left, int top, int right, int bottom)
	: _position(left,top,right-left,bottom-top)
	, _pageNumber(page)
	{
	}
}