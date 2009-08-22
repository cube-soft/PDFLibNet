#pragma once
#include "AFPDFDocInterop.h"
#include "PDFPageInterop.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;

namespace PDFLibNet
{
	public delegate void WriteToStreamHandler(wchar_t *text, int len);
	public delegate void ReadFromStreamHandler(unsigned char *buffer,int dir,  int pos, int len);
	
	public ref class xPDFStream
		: public System::IO::StreamWriter {
	private:
		GCHandle _gchwriteToStream;
		WriteToStreamHandler ^_writeToStream;
		void _WriteToStreamFunc(wchar_t *str,int len);
	internal:
		void *GetWritePointer();
	public:
		xPDFStream::xPDFStream(System::IO::Stream ^stream,System::Text::Encoding ^enc);
		xPDFStream(System::IO::Stream ^stream);
		xPDFStream(String ^fileName);
		!xPDFStream();
		~xPDFStream();
	};

	public ref class xPDFBinaryReader
		: public System::IO::BinaryReader
	{
	private:
		GCHandle _gchReadFromStream;
		void *_ptrReadFromStream;
		ReadFromStreamHandler ^_readFromStream;
		void _ReadFromStreamFunc(unsigned char *buffer,int dir,  int pos, int len);
	internal:
		void *GetReadPointer();
	public:
		xPDFBinaryReader(System::IO::Stream ^stream);
		!xPDFBinaryReader();
		~xPDFBinaryReader();
	};
	public ref class PDFPage
	{
	private:
		PDFPageInterop *_page;
		AFPDFDocInterop *_pdfDoc;
		System::String ^_text;
		bool _loaded;
		int _pageNumber;
		void loadPage();
		void extractImages();
	internal:
		PDFPage(AFPDFDocInterop *pdfDoc, PDFPageInterop *page);
		PDFPage(AFPDFDocInterop *pdfDoc, int page);
		
	public:
		~PDFPage(void);
		property int ImagesCount{
			int get(){
				extractImages();
				return _page->getImagesCount();
			}
		}
		System::Drawing::Image ^GetImage(int index);
		String ^ExtractText(System::Drawing::Rectangle rect);
		//String ^ExtractWord(System::Drawing::Point p);
		//String ^ExtractWord(System::Drawing::Point p, int iWord, int iCount);

		property String ^ISOAName
		{
			String ^get(){
				char *iso = _page->getISOANum();
				String ^isoS = gcnew String(iso);
				delete iso;
				return isoS;
			}
		}
		property double Width
		{
			double get(){
				return _page->getPageWidth();
			}
		}
		property double Height
		{
			double get(){
				return _page->getPageHeight();
			}
		}

		property System::IO::StreamWriter ^TextStream
		{
			System::IO::StreamWriter ^get(){
				loadPage();
				System::IO::MemoryStream ^mems =  gcnew System::IO::MemoryStream();
				xPDFStream ^str =gcnew xPDFStream((System::IO::Stream ^)mems);
				
				_page->getTextStream(str->GetWritePointer());
				//swr->Flush();
				mems->Flush();
				return str;
			}
		}
		property System::String ^Text{
			String ^get(){
				loadPage();
				if(_text==nullptr){
					wchar_t *text=_page->getText();
					if(text!=0){
						_text = gcnew String(text);
					}else 
						return String::Empty;
				}
				return _text;
			}
		}
		Boolean HasText(Int32 x, Int32 y){
			loadPage();
			if(_page)
				return _page->hasText(x,y);
			return false;
		}

	};
}