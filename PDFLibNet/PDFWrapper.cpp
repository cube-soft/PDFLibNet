#include "PDFWrapper.h"


using namespace System::Runtime::InteropServices;

namespace PDFLibNet
{
	#pragma managed

	bool PDFWrapper::RenderPage(IntPtr handler, System::Boolean bForce){
		long hwnd=(long)handler.ToPointer();
		long ret =_pdfDoc->RenderPage(hwnd,bForce);
		if(ret==10001)
			throw gcnew System::OutOfMemoryException(ret.ToString());
		
		return true;
	}
	

	bool PDFWrapper::RenderPage(IntPtr handler)
	{
		long hwnd=(long)handler.ToPointer();
		long ret =_pdfDoc->RenderPage(hwnd);
		if(ret==10001)
			throw gcnew System::OutOfMemoryException(ret.ToString());
		
		return true;
	}
	long PDFWrapper::PerfomLinkAction(System::Int32 linkPtr)
	{
		return _pdfDoc->ProcessLinkAction((long)linkPtr);
	}

	bool PDFWrapper::DrawPageHDC(IntPtr hdc){
		long lhdc =(long)hdc.ToPointer();
		_pdfDoc->RenderHDC(lhdc);
		return true;
	}
	void PDFWrapper::ZoomIN(){
		_pdfDoc->ZoomIN();
	}
	void PDFWrapper::ZoomOut(){
		_pdfDoc->ZoomOUT();
	}
	void PDFWrapper::FitToWidth(IntPtr handler){
		//if(doc.IsNull())
		//	throw gcnew System::NullReferenceException();
		long hwnd=(long)handler.ToPointer();
		_pdfDoc->FitScreenWidth(hwnd);
	}
	void PDFWrapper::FitToHeight(IntPtr handler){
		//if(doc.IsNull())
		//	throw gcnew System::NullReferenceException();
		long hwnd=(long)handler.ToPointer();
		_pdfDoc->FitScreenHeight(hwnd);
	}

	bool PDFWrapper::LoadPDF(System::String ^fileName){
		
		
		PDFLoadBegin();

		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(fileName);
		char *singleByte= (char*)ptr.ToPointer();
		try{
			_bLoading=true;
			_childrens=nullptr;
			_title=nullptr;
			_author=nullptr;
			_linksCache.Clear();
			if(_searchResults!=nullptr)
				_searchResults->Clear();

			if(_pdfDoc->LoadFromFile(singleByte)==4) //errorEncrypted
				throw gcnew System::Security::SecurityException();
			_pdfDoc->SetCurrentPage(1);	
			_bLoading=false;
			PDFLoadCompeted();
		}catch(System::AccessViolationException ^e){
			throw gcnew System::AccessViolationException("Something is wrong with the file");
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return true;
	} 


	long PDFWrapper::FindText(String ^sText, Int32 iPage, PDFSearchOrder SearchOrder, Boolean bCaseSensitive, Boolean bBackward, Boolean bMarkAll, Boolean bWholeDoc){
		IntPtr ptr = Marshal::StringToCoTaskMemUni(sText);
		wchar_t *singleByte= (wchar_t*)ptr.ToPointer();

		try{
			if(_searchResults!=nullptr){
				_searchResults->Clear();
				_searchResults=nullptr;
			}
			
			return _pdfDoc->FindString(singleByte,(long)iPage,(long)SearchOrder,(bool)bCaseSensitive,(bool)bBackward,(bool)bMarkAll,(bool)bWholeDoc);
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return 0;
	}

	long PDFWrapper::FindFirst(String ^sText,PDFSearchOrder SearchOrder, Boolean bBackward)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemUni(sText);
		wchar_t *singleByte= (wchar_t*)ptr.ToPointer();
		
		try{
			if(_searchResults!=nullptr){
				_searchResults->Clear();
				_searchResults=nullptr;
			}
			
			return _pdfDoc->FindFirst(singleByte,(long)SearchOrder,bBackward);
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return 0;
	}

	long PDFWrapper::FindNext(String ^sText){
		IntPtr ptr = Marshal::StringToCoTaskMemUni(sText);
		wchar_t *singleByte= (wchar_t*)ptr.ToPointer();
		try{
			if(_searchResults!=nullptr){
				_searchResults->Clear();
				_searchResults=nullptr;
			}
			
			return _pdfDoc->FindNext(singleByte);
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return 0;
	}
	long PDFWrapper::FindPrevious(String ^sText){
		IntPtr ptr = Marshal::StringToCoTaskMemUni(sText);
		wchar_t *singleByte= (wchar_t*)ptr.ToPointer();
		try{
			if(_searchResults!=nullptr){
				_searchResults->Clear();
				_searchResults=nullptr;
			}
			
			return _pdfDoc->FindPrior(singleByte);
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return 0;
	}

	long  PDFWrapper::PrintToFile(String ^fileName, Int32 fromPage, Int32 toPage)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(fileName);
		char *singleByte= (char*)ptr.ToPointer();
		int ret;
		try{
			if(_searchResults!=nullptr){
				_searchResults->Clear();
				_searchResults=nullptr;
			}
			
			ret = _pdfDoc->PrintToFile(singleByte,(int)fromPage,(int)toPage);
			if(ret==-1)
				throw gcnew Exception("Permissions error");
			if(ret==-2)
				throw gcnew Exception("Status error");
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return 0;
	}

	System::Collections::Generic::List<PageLink ^> ^PDFWrapper::GetLinks(int iPage)
	{
		PageLinkCollection<PageLink ^> ^col;
		if(!_bLoading && !this->_linksCache.TryGetValue(iPage,col)){
			col =gcnew PageLinkCollection<PageLink ^>(this);
			PageLinksInterop *pl = _pdfDoc->getPageLinksInterop(iPage);
			if(pl!=0){
				for(int i=0;i<pl->getLinkCount();i++){
					col->Add(gcnew PageLink(pl->getLink(i),this));
				}
				_linksCache.Add(iPage,col);
			}
		}
		return col;
	}
	System::Drawing::PointF PDFWrapper::PointUserToDev(System::Drawing::PointF point){
		int x;
		int y;
		_pdfDoc->cvtUserToDev(point.X,point.Y,&x,&y);
		System::Drawing::PointF c((float)x,(float)y);
		return c;
	}
	System::Drawing::PointF PDFWrapper::PointDevToUser(System::Drawing::PointF point){
		double x;
		double y;
		_pdfDoc->cvtDevToUser(point.X,point.Y,&x,&y);
		System::Drawing::PointF c(x,y);
		return c;
	}

	long PDFWrapper::ExportJpg(System::String ^fileName, int quality)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(fileName);
		char *singleByte= (char*)ptr.ToPointer();
		int ret;
		try{
			_pdfDoc->SaveJpg(singleByte,quality);
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return 0;		
	}

	long PDFWrapper::ExportText(System::String ^fileName, System::Int32 firstPage, System::Int32 lastPage,System::Boolean physLayout,System::Boolean rawOrder)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(fileName);
		char *singleByte= (char*)ptr.ToPointer();
		int ret;
		try{
			_pdfDoc->SaveTxt(singleByte,firstPage,lastPage,physLayout,rawOrder,false);
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return 0;		
	}
	long PDFWrapper::ExportHtml(System::String ^fileName, System::Int32 firstPage, System::Int32 lastPage,System::Boolean noFrames,System::Boolean noMerge, System::Boolean complexMode)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(fileName);
		char *singleByte= (char*)ptr.ToPointer();
		int ret;
		try{
//			_pdfDoc->SaveHtml(singleByte,firstPage,lastPage,noFrames,noMerge,complexMode);
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		return 0;		
	}

	LinkDest ^PDFWrapper::FindDestination(String ^destName){
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(destName);
		char *singleByte= (char*)ptr.ToPointer();
		int ret;
		try{
			return gcnew LinkDest(_pdfDoc->findDest(singleByte));
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
		
		return nullptr;
	}

}
