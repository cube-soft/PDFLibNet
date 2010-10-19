#pragma once
#include "OutlineItemInterop.h"
#include "PageLink.h"
#include "LinkDest.h"
using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

namespace PDFLibNet
{
	ref class PDFWrapper;

	public ref class OutlineItem
	{
	private:
		System::String ^_title;
		OutlineItemInterop *_outline;
		PDFWrapper ^_pdfDoc;
		LinkDest ^_dest;
		System::Collections::Generic::IList<OutlineItem^> ^_childrens;
	internal:
		OutlineItem(int ptr,PDFWrapper ^pdfDoc);
	private:
		void LoadChildrens();
		!OutlineItem(){
			_outline=nullptr;
		}
		System::Int32 OutlineItem::GetKidPtr(long i);
	public:
		long DoAction();
		
		LinkActionKind GetKind(void);
		//System::Int32  GetLinkAction(void);

		property LinkDest ^Destination{
			LinkDest ^get(){
				if(_dest==nullptr)
					_dest =gcnew LinkDest(_outline->getDest());
				
				return _dest;
			}
		}
		property System::Int32 KidsCount
		{
			System::Int32 get(){
				if(_outline!=0)
					return _outline->GetKidsCount();
				return 0;
			}
			
		}
		property  System::Collections::Generic::IList<OutlineItem^> ^Childrens{
			System::Collections::Generic::IList<OutlineItem^> ^get(){
				if(_childrens==nullptr)
					LoadChildrens();
				return _childrens;
			}
		}
		property System::String ^Title{
			System::String ^get()
			{
				if(_title==nullptr){
					wchar_t *title=_outline->GetTitle();
					_title= gcnew String(title);
					delete title;
				}
				return _title;
			}
		}
		

		
		
	};

};