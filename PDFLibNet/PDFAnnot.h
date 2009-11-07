#pragma once
#include "PDFPage.h"

namespace PDFLibNet
	{
	ref class PDFAnnot
	{
	internal:
		PDFAnnot(PDFPage ^page);
	public:
		property System::String ^Caption
		{
			System::String ^get(){
				return nullptr;
			}
		}

	};
}