
#include "SearchResultInterop.h"

SearchResultInterop::SearchResultInterop(int page, int left, int top,int right,int bottom)
: Page(page)
, x0(left)
, y0(top)
, x1(right)
, y1(bottom)
{
}

SearchResultInterop::~SearchResultInterop(void)
{
}
