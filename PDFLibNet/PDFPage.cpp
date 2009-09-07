#include "PDFPage.h"
#include <msclr\lock.h>

namespace PDFLibNet
{
	PDFPage::PDFPage(AFPDFDocInterop *pdfDoc,PDFPageInterop *page)
		: _page(page)
		, _loaded(true)
		, _pdfDoc(pdfDoc)
		, _pageNumber(page->getPage())
		, _thumbNail(nullptr)
		, _errorRender(gcnew Object())
		, _thumbHdc(IntPtr::Zero)
		, _isSuccesed(false)
	{
	}
	PDFPage::PDFPage(AFPDFDocInterop *pdfDoc, int page)
		: _page(0)
		, _loaded(false)
		, _pdfDoc(pdfDoc)
		, _pageNumber(page)
		, _thumbNail(nullptr)
		, _errorRender(gcnew Object())
		, _thumbHdc(IntPtr::Zero)
		, _isSuccesed(false)
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

	void PDFPage::extractImages(){
		loadPage();
		_page->extractImages();
	}

	void PDFPage::_RenderNotifyFinished(int page, bool bSuccess){
		try{
			//msclr::lock l(_errorRender);
			if(bSuccess){
				if(_thumbG!=nullptr && _thumbNail != nullptr){
					_isSuccesed = true;
					if(!_thumbHdc.Equals(IntPtr::Zero))
						_thumbG->ReleaseHdc(_thumbHdc);
					_thumbHdc = IntPtr::Zero;
				}
			}else{
				//Invalidate!
				_thumbNail=nullptr;
			}
			if(_evRenderNotifyFinished!=nullptr){
				for each(RenderNotifyFinishedHandler^ dd in _evRenderNotifyFinished->GetInvocationList()){
					dd->Invoke(page,bSuccess);
				}
			}
		}
		catch(System::ArgumentException ^){
			//Invalidate!
			_thumbNail=nullptr;
		}
		finally
		{
			_thumbG=nullptr;
		}
	}
	System::Drawing::Bitmap ^PDFPage::LoadThumbnail(int width,int height){
//		msclr::lock l(_errorRender);
		if( (_thumbNail == nullptr || _thumbNail->Width!=width || _thumbNail->Height!=height))
		{

			if(_internalRenderNotifyFinished==nullptr){		
				_internalRenderNotifyFinished=gcnew RenderNotifyFinishedHandler(this,&PDFPage::_RenderNotifyFinished);
				_gchRenderNotifyFinished = GCHandle::Alloc(_internalRenderNotifyFinished);
			}
			void *ptrCallBack = Marshal::GetFunctionPointerForDelegate(_internalRenderNotifyFinished).ToPointer();

			_thumbNail = gcnew System::Drawing::Bitmap(width,height);
			
			loadPage();
			_thumbG = System::Drawing::Graphics::FromImage(_thumbNail);
			_thumbG->Clear(System::Drawing::Color::White);
			_thumbHdc = _thumbG->GetHdc();
			if(_pdfDoc->DrawPage(_pageNumber,_thumbHdc.ToInt32(),width,height,0,true,ptrCallBack)!=0)
			{
				_thumbHdc=IntPtr::Zero;
				_thumbG=nullptr;
				_thumbNail=nullptr;
			}
		}
	
		return _thumbNail;
	}

	System::Drawing::Bitmap ^PDFPage::GetBitmap(System::Int32 width, System::Int32 height)
	{
		System::Drawing::Bitmap ^bmp=gcnew System::Drawing::Bitmap(width,height);
		loadPage();
		System::Drawing::Graphics ^g = System::Drawing::Graphics::FromImage(bmp);
		g->Clear(System::Drawing::Color::White);
		System::IntPtr hdc = g->GetHdc();
		if(_pdfDoc->DrawPage(_pageNumber,hdc.ToInt32(),width,height,0,false,0)!=0)
		{
			return nullptr;
		}
		g->ReleaseHdc();
		return bmp;
	}


	System::Drawing::Image ^PDFPage::GetImage(int index)
	{
		extractImages();
		unsigned char *bytes = _page->getImageBytes(index);
		int size = _page->getImageSize(index);		//Size in bytes
		int width = _page->getImageWidth(index);
		int height = _page->getImageHeight(index);	
		int nComp =_page->getImageNumComps(index);	//Number of bytes per pixel
		int iType = _page->getImageType(index);
		int bCount=0;
		cli::array<unsigned char> ^manbytes=nullptr;
		System::IO::MemoryStream  ^ms=nullptr;
		System::Drawing::Imaging::BitmapData ^bmpd;
		System::Drawing::Bitmap ^bmp;

		if(bytes){
			switch(iType){
				case 0 : //JPG
					manbytes=gcnew cli::array<unsigned char>(size);
					for(int i=0;i<size;++i)
						manbytes[i]=bytes[i];
					ms =gcnew System::IO::MemoryStream(manbytes);
					return System::Drawing::Image::FromStream(ms);
				case 1: //PBM
					manbytes=gcnew cli::array<unsigned char>(size);
					for(int i=0;i<size;++i)
						manbytes[i]=bytes[i];
					bmp = gcnew System::Drawing::Bitmap(width,height,System::Drawing::Imaging::PixelFormat::Format8bppIndexed);
					bmpd =bmp->LockBits(System::Drawing::Rectangle(0,0,width,height), System::Drawing::Imaging::ImageLockMode::WriteOnly, bmp->PixelFormat);
					bCount=bmpd->Stride * bmp->Height;
					//A lot faster
					Marshal::Copy(manbytes,0,bmpd->Scan0,size);

					bmp->UnlockBits(bmpd);
					return bmp;
					break; 
				case 2://PPM
					manbytes=gcnew cli::array<unsigned char>(size);
					for(int i=0;i<size;++i)
						manbytes[i]=bytes[i];

					if(nComp==4)	
						bmp = gcnew System::Drawing::Bitmap(width,height,System::Drawing::Imaging::PixelFormat::Format32bppRgb);
					else if(nComp==3)
						bmp = gcnew System::Drawing::Bitmap(width,height,System::Drawing::Imaging::PixelFormat::Format24bppRgb);
					else if(nComp==2)
						bmp = gcnew System::Drawing::Bitmap(width,height,System::Drawing::Imaging::PixelFormat::Format16bppRgb565);
					else if(nComp==1)
						bmp = gcnew System::Drawing::Bitmap(width,height,System::Drawing::Imaging::PixelFormat::Format8bppIndexed);

					bmpd =bmp->LockBits(System::Drawing::Rectangle(0,0,width,height), System::Drawing::Imaging::ImageLockMode::WriteOnly, bmp->PixelFormat);
					bCount =bmpd->Stride * bmp->Height;
					//A lot faster
					Marshal::Copy(manbytes,0,bmpd->Scan0,size);
					
					bmp->UnlockBits(bmpd);
					return bmp;

				break;
			}
			
		}
		return nullptr;

	}


	System::String ^PDFPage::ExtractText(System::Drawing::Rectangle rect)
	{
		loadPage();
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
			_gchwriteToStream = GCHandle::Alloc(_writeToStream);
		}
	}
	xPDFStream::xPDFStream(System::IO::Stream ^stream,System::Text::Encoding ^enc)
		: StreamWriter(stream,enc)
		, _writeToStream(nullptr)
	{
		this->AutoFlush=true;
		if(this->_writeToStream==nullptr){		
			_writeToStream=gcnew  WriteToStreamHandler(this,&xPDFStream::_WriteToStreamFunc);
			_gchwriteToStream = GCHandle::Alloc(_writeToStream);
		}
	}
	xPDFStream::xPDFStream(String ^fileName)
		:StreamWriter(fileName)
	{
		this->AutoFlush=true;
		if(this->_writeToStream==nullptr){		
			_writeToStream=gcnew  WriteToStreamHandler(this,&xPDFStream::_WriteToStreamFunc);
			_gchwriteToStream = GCHandle::Alloc(_writeToStream);
		}
	}

	void xPDFStream::_WriteToStreamFunc(wchar_t *str, int len)
	{
		Write(gcnew String(str));		
	}
	
	xPDFStream::!xPDFStream(){
		GC::Collect();
		if(this->_gchwriteToStream.IsAllocated){		
			this->_gchwriteToStream.Free();
		}
	}
	xPDFStream::~xPDFStream(){}

	void *xPDFStream::GetWritePointer()
	{
		return Marshal::GetFunctionPointerForDelegate(this->_writeToStream).ToPointer();
	}

	xPDFBinaryReader::xPDFBinaryReader(System::IO::Stream ^stream)
		: BinaryReader(stream)
		, _readFromStream(nullptr)
		, _readLock(gcnew System::Object())
	{
		if(this->_readFromStream==nullptr){		
			_readFromStream=gcnew ReadFromStreamHandler(this,&xPDFBinaryReader::_ReadFromStreamFunc);
			_gchReadFromStream = GCHandle::Alloc(_readFromStream);
			_ptrReadFromStream = Marshal::GetFunctionPointerForDelegate(this->_readFromStream).ToPointer();
		}
	}
	xPDFBinaryReader::!xPDFBinaryReader(){
		GC::Collect();
		if(this->_gchReadFromStream.IsAllocated){		
			this->_gchReadFromStream.Free();
		}
	}
	xPDFBinaryReader::~xPDFBinaryReader(){}

	void xPDFBinaryReader::_ReadFromStreamFunc(unsigned char *buffer,int dir, int pos, int len)
	{
		msclr::lock l(_readLock);
		if(this->BaseStream->Position != pos)
		{
			if(this->BaseStream->CanSeek){
				if(dir==1)
					this->BaseStream->Seek(pos,System::IO::SeekOrigin::Begin);
				else if(dir==-1)
					this->BaseStream->Seek(pos,System::IO::SeekOrigin::End);
				else if(dir==0)
					this->BaseStream->Seek(pos,System::IO::SeekOrigin::Current);
			}else{
				throw gcnew System::InvalidOperationException();
			}
		}
		System::Runtime::InteropServices::Marshal::Copy(this->ReadBytes(len),0,IntPtr(buffer),len);
	}

	void *xPDFBinaryReader::GetReadPointer()
	{
		return _ptrReadFromStream;
	}
		
	/*
	GCHandle _gchReadFromStream;
		ReadFromStreamHandler ^_readFromStream;
		void _ReadFromStreamFunc(unsigned char *buffer, int pos, int len);
	internal:
		void *GetReadPointer();
	public:
		
		!xPDFBinaryReader();
		~xPDFBinaryReader();*/
}