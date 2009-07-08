#pragma once
#include "AFPDFDocInterop.h"
#include "PDFSearchResult.h"
#include "OutlineItem.h"
#include "OutlineItemCollection.h"
#include "PageLink.h"
#include "PageLinkCollection.h"

using namespace System;
using namespace System::Drawing;

namespace PDFLibNet {

	public delegate void PDFLoadCompletedHandler();
	public delegate void PDFLoadBeginHandler();

	public enum class PDFSearchOrder
	{
		PDFSearchFromdBegin = 0,
		PDFSearchFromCurrent = 1,
	};

	/// <summary>
	///
	/// </summary>
	public ref class PDFWrapper
	{
	internal:
		AFPDFDocInterop *_pdfDoc;
	private:
		OutlineItemCollection<OutlineItem^> ^_childrens;
		System::Collections::Generic::List<PDFSearchResult^> ^_searchResults;
		System::Collections::Generic::Dictionary<int,PageLinkCollection<PageLink ^> ^> _linksCache;
		String ^_title;
		String ^_author;
		bool _bLoading;
	public:
		PDFWrapper()
			: _pdfDoc(new AFPDFDocInterop)
			, _childrens(nullptr)
			, _searchResults(nullptr)
			, _title(nullptr)
			, _author(nullptr)

		{

		}

		long PerfomLinkAction(System::Int32 linkPtr);
		bool LoadPDF(System::String ^fileName);
		
		bool RenderPage(IntPtr handler);
		bool DrawPage(IntPtr handle);
		bool DrawPageHDC(IntPtr hdc);

		void FitToWidth(IntPtr handler);
		void FitToHeight(IntPtr handler);

		void ZoomIN();
		void ZoomOut();

		void NextPage(){
			_pdfDoc->NextPage();
		}
		void PreviousPage(){
			_pdfDoc->PreviousPage();
		}

		long FindText(String ^sText, Int32 iPage, PDFSearchOrder SearchOrder, Boolean bCaseSensitive, Boolean bBackward, Boolean bMarkAll, Boolean bWholeDoc);
		long FindFirst(String ^sText,PDFSearchOrder SearchOrder, Boolean Backward);
		long FindNext(String ^sText);
		long FindPrevious(String ^sText);
		long PrintToFile(String ^fileName, Int32 fromPage, Int32 toPage);
		property bool SearchCaseSensitive;
		property double RenderDPI {
			double get(){
				return _pdfDoc->GetRenderDPI();
			}
			void set(double value){
				_pdfDoc->SetRenderDPI(value);
			}
		}
		property String ^Author{
			String ^get(){
				if(_author==nullptr){
					char *title=_pdfDoc->GetAuthor();
					if(title!=0)
						_author = gcnew String(title);
					else 
						return String::Empty;
				}
				return _author;
			}
		}
		property String ^Title{
			String ^get(){
				
				if(_title==nullptr){
					char *title=_pdfDoc->GetTitle();
					if(title!=0)
						_title = gcnew String(title);
					else 
						return String::Empty;
				}
				return _title;
			}
		}
		property double Zoom {
			double get(){
				return _pdfDoc->GetZoom();
			}
			void set(double value){
				_pdfDoc->SetZoom(value);
			}
		}
		property long PageWidth {
			long get(){
				return _pdfDoc->GetPageWidth();
			}
		}
		property long PageHeight {
			long get(){
				return _pdfDoc->GetPageHeight();
			}
		}
		property long CurrentX {
			long get(){
				return _pdfDoc->GetCurrentX();
			}
			void set(long posX){
				_pdfDoc->SetCurrentX(posX);
			}
		}
		property long CurrentY {
			long get(){
				return _pdfDoc->GetCurrentY();
			}
			void set(long posY){
				_pdfDoc->SetCurrentY(posY);
			}
		}

		property long PageCount
		{
			long get(){
				//if(doc.IsNull())
				//	throw gcnew System::NullReferenceException();
				return _pdfDoc->GetPageCount();
			}	
		}
		property long CurrentPage{
			long get(){
				//if(doc.IsNull())
				//	throw gcnew System::NullReferenceException();
				return _pdfDoc->GetCurrentPage();
			}
			void set(long page){
				//if(doc.IsNull())
				//	throw gcnew System::NullReferenceException();
				_pdfDoc->SetCurrentPage(page);
			}
		}

		
		property  System::Collections::Generic::List<OutlineItem^> ^Outline
		{
			System::Collections::Generic::List<OutlineItem^> ^get()
			{
				if(_childrens==nullptr)
				{
					_childrens = gcnew  OutlineItemCollection<OutlineItem ^>();
					for(long i=1;i<=_pdfDoc->GetOutlineCount();i++)
						_childrens->Add(gcnew OutlineItem(_pdfDoc->GetOutlinePtr(i),this));
				}
				return _childrens;
			}
		}
		property System::Collections::Generic::List<PDFSearchResult ^> ^SearchResults
		{
			System::Collections::Generic::List<PDFSearchResult ^> ^get()
			{
				if(_searchResults==nullptr){
					_searchResults=gcnew System::Collections::Generic::List<PDFSearchResult ^>();
					
					SearchResultInterop *sr =_pdfDoc->GetSearchResults();
					long count =_pdfDoc->GetSearchCount();
					for(int i=0;i<count;i++)
						_searchResults->Add(gcnew PDFSearchResult(sr[i].Page,sr[i].x0,sr[i].y0,sr[i].x1,sr[i].y1));
				}
				return _searchResults;
			}

		}
		System::Collections::Generic::List<PageLink ^> ^GetLinks(int iPage);

		PointF PointUserToDev(PointF point);
		PointF PointDevToUser(PointF point);

		event PDFLoadCompletedHandler ^PDFLoadCompeted;
		event PDFLoadBeginHandler ^PDFLoadBegin;
	protected:	

		!PDFWrapper()
		{
			//if(!_pdfDoc.IsNull())
			_pdfDoc->Dispose();
			//_pdfDoc.Reset();
		}

		~PDFWrapper()
		{
			//if(!_pdfDoc.IsNull())
			_pdfDoc->Dispose();
			//_pdfDoc.Reset();
		}

	};
}
