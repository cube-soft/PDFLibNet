#pragma once
#include "AFPDFDocInterop.h"
#include "OutlineItemCollection.h"
#include "PDFWrapper.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

namespace PDFLibNet
{
	public enum class LinkActionKind {
	  actionGoTo,			// go to destination
	  actionGoToR,			// go to destination in new file
	  actionLaunch,			// launch app (or open document)
	  actionURI,			// URI
	  actionNamed,			// named action
	  actionMovie,			// movie action
	  actionUnknown			// anything else
	};

	public ref class OutlineItem
	{
	private:
		OutlineItemInterop *_outline;
		PDFWrapper ^_pdfDoc;
		System::Collections::Generic::IList<OutlineItem^> ^_childrens;
	internal:
		OutlineItem(int ptr,PDFWrapper ^pdfDoc);
	private:
		void LoadChildrens();
	public:
		System::String ^GetTitle(void);
		PDFLibNet::LinkActionKind GetKind(void);
		System::Int32  GetLinkAction(void);
		System::Int32 GetKidsCount(void);
		
		property  System::Collections::Generic::IList<OutlineItem^> ^Childrens{
			System::Collections::Generic::IList<OutlineItem^> ^get(){
				if(_childrens==nullptr)
					LoadChildrens();
				return _childrens;
			}
		}
	};

};