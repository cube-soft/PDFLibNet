
#include "PageLink.h"
namespace PDFLibNet
{

	PageLink::PageLink(LinkInterop *ptr)
		: _ptr(ptr)
		, _name(nullptr)
		, _bounds(System::Drawing::Rectangle::Empty)
		, _action(nullptr)
	{
	}

	PageLinkAction::PageLinkAction(LinkInterop *linkInterop)
		: _action(linkInterop->getAction())
	{
	}
}