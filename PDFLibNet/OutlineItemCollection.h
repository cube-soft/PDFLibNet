#pragma once
#include "OutlineItem.h"

namespace PDFLibNet
{	
	generic<class T>
	where T:OutlineItem
	public ref class OutlineItemCollection
	: public System::Collections::Generic::List<T>
	{
	

	};
};