#include "PDFPage.h"
namespace PDFLibNet
{
	PDFPage::PDFPage(AFPDFDocInterop *pdfDoc,PDFPageInterop *page)
		: _page(page)
		, _loaded(true)
		, _pdfDoc(pdfDoc)
		, _pageNumber(page->getPage())
	{
	}
	PDFPage::PDFPage(AFPDFDocInterop *pdfDoc, int page)
		: _page(0)
		, _loaded(false)
		, _pdfDoc(pdfDoc)
		, _pageNumber(page)
	{
	}


	void PDFPage::loadPage()
	{
		if(!_loaded)
		{
			PDFPageInterop *p=_pdfDoc->getPage(_pageNumber);
			if(p){
				_page=p;
				_loaded=true;
			}
		}
	}

	PDFPage::~PDFPage(void){
		if(_page && _loaded)
			delete _page;
	}

	System::String ^PDFPage::ExtractText(System::Drawing::Rectangle rect)
	{

		wchar_t * res = _page->extractText(rect.Left,rect.Top,rect.Right,rect.Bottom);
		return gcnew String(res);
	}
	
	xPDFStream::xPDFStream(System::IO::Stream ^stream)
		: StreamWriter(stream,System::Text::Encoding::UTF8)
		, _writeToStream(nullptr)
	{
		this->AutoFlush=true;
		if(this->_writeToStream==nullptr){		
			_writeToStream=gcnew  WriteToStreamHandler(this,&xPDFStream::_WriteToStreamFunc);
			_gchwriteToStream = GCHandle::Alloc(_gchwriteToStream);
		}
	}
	xPDFStream::xPDFStream(System::IO::Stream ^stream,System::Text::Encoding ^enc)
		: StreamWriter(stream,enc)
		, _writeToStream(nullptr)
	{
		this->AutoFlush=true;
		if(this->_writeToStream==nullptr){		
			_writeToStream=gcnew  WriteToStreamHandler(this,&xPDFStream::_WriteToStreamFunc);
			_gchwriteToStream = GCHandle::Alloc(_gchwriteToStream);
		}
	}
	xPDFStream::xPDFStream(String ^fileName)
		:StreamWriter(fileName)
	{
		this->AutoFlush=true;
		if(this->_writeToStream==nullptr){		
			_writeToStream=gcnew  WriteToStreamHandler(this,&xPDFStream::_WriteToStreamFunc);
			_gchwriteToStream = GCHandle::Alloc(_gchwriteToStream);
		}
	}

	void xPDFStream::_WriteToStreamFunc(wchar_t *str, int len)
	{
		Write(gcnew String(str));	
		
	}
	
	xPDFStream::!xPDFStream(){
		if(this->_gchwriteToStream.IsAllocated){		
			this->_gchwriteToStream.Free();
		}
	}
	void *xPDFStream::GetWritePointer()
	{
		return Marshal::GetFunctionPointerForDelegate(this->_writeToStream).ToPointer();
	}
	
}