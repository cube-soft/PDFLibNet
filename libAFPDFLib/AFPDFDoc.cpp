// AFPDFDoc.cpp : implementation file
#include "AFPDFDoc.h"
#define CACHE_BITMAPS
	
	#define FIND_DPI 300
	#define PRINT_DPI 72
	#define SPACE_X 16
	#define SPACE_Y 16
	static char EmptyChar[1]={'\0'};
	
	#define MAX_BITMAP_CACHE 8
	static CBitmap *_bitmapCache[MAX_BITMAP_CACHE+1]={0};
	static int _pageCached[MAX_BITMAP_CACHE+1]={0};
	static int _countCached =-1;

	void InvalidateBitmapCache(){
		for(int i=0;i<=MAX_BITMAP_CACHE;i++){
			if(_bitmapCache[i]!=0){
				_bitmapCache[i]->DeleteObject();
				delete _bitmapCache[i];
			}
			_pageCached[i]=0;
			_bitmapCache[i]=0;
		}
	}

	void RemoveFromCache(int page){
		for(int i=0;i<=MAX_BITMAP_CACHE;i++){
			if(_pageCached[i]==page){
				_bitmapCache[i]->DeleteObject();
				delete _bitmapCache[i];
				_bitmapCache[i]=0;
				return;
			}
		}
	}

	void AddBitmapCache(CBitmap *bmp, int page){
		//if exists and is not equal, delete
		for(int i=0;i<=MAX_BITMAP_CACHE;i++){
			if(_pageCached[i]==page){
				if( _bitmapCache[i]!=0 && _bitmapCache[i]!=bmp){
					_bitmapCache[i]->DeleteObject();
					delete _bitmapCache[i];
					_bitmapCache[i]=bmp;
					return;
				}
			}
		}
		//Add in an empty bin
		for(int i=0;i<=MAX_BITMAP_CACHE;i++){
			if(_pageCached[i]==0){
				_bitmapCache[i]=bmp;
				_pageCached[i]=page;
				return;
			}
		}
		//Begin to replace old cache
		if((++_countCached)>=MAX_BITMAP_CACHE)
			_countCached=0;

		//If new bin is busy, delete
		if(_bitmapCache[_countCached]!=0){
			_bitmapCache[_countCached]->DeleteObject();
			delete _bitmapCache[_countCached];
			_bitmapCache[_countCached]=0;
		}
		//Save in cache
		_bitmapCache[_countCached]=bmp;
		_pageCached[_countCached]=page;
	}

	CBitmap *GetBitmapCache(int page){
		for(int i=0;i<=MAX_BITMAP_CACHE;i++){
			if(_pageCached[i]==page){
				return _bitmapCache[i];
			}
		}
		return 0;
	}



	Unicode *GetUnicodeString(LPCTSTR str, int length)
	{
		Unicode * ucstring = new Unicode[length + 1];
		int j;

		if (ucstring == NULL)
			return NULL;

		/* it is almost safe to transform from UCS2 to UCS4 this way */
		for( j = 0; j < length; j++)
			ucstring[j] = str[j];

		ucstring[j] = 0;
		return ucstring;
	}

	static char *getDicString(Dict *infoDict,char *key,UnicodeMap *uMap)
	{
		Object obj;
		GString *s1;
		GBool isUnicode;
		Unicode u;
		char buf[8];
		int i, n;

	
		if (infoDict->lookup(key, &obj)->isString()) 
		{
			//fputs(text1, f);
			s1 = obj.getString();
			if ((s1->getChar(0) & 0xff) == 0xfe && (s1->getChar(1) & 0xff) == 0xff) {
				isUnicode = gTrue;
				i = 2;
			} else {
				isUnicode = gFalse;
				i = 0;
			}
			if(s1->getLength()>0)
				return s1->getCString();

		}
		return EmptyChar;
		
	}

	CPDFSearchResult::CPDFSearchResult() 
	: PageFound(0)
	{
		//
	}

	CPDFSearchResult::CPDFSearchResult(CRect r,long lPage) 
	: CRect(r),
	PageFound(lPage)
	{
		//
	}




	//---------------------------------------
	AFPDFDoc::AFPDFDoc()
	: m_PDFDoc(NULL)
	, m_splashOut(NULL)
	, m_bitmapBytes(NULL)
	, m_CurrentPage(1)
	, m_Bitmap(NULL)
	, m_ViewOffsetX(0)
	, m_ViewOffsetY(0)
	, m_renderDPI(72)
	, m_LastRenderDPI(72)
	, m_Rotation(0)
	, m_HideMarks(FALSE)
	, m_SearchStarted(FALSE)
	, m_PageWidth(0)
	, m_PageHeight(0)
	, m_renderingThread(0)
	, m_PageToRenderByThread(0)
	{
		// To keep the application running as long as an OLE automation 
		//	object is active, the constructor calls AfxOleLockApp.
		
		TCHAR szExe[MAX_PATH];
		int size = ::GetModuleFileName(NULL, szExe, MAX_PATH); 
		TCHAR *pLastSlash = _tcsrchr(szExe, _T('\\'));
		
		if (pLastSlash){
			// Truncate at slash to get app dir
			*pLastSlash=L'\0';
		}

		char *baseDir = new char[wcslen((const wchar_t *)szExe)+1];
		sprintf(baseDir,"%S",szExe);

		globalParams = new GlobalParams("auto");
		globalParams->setEnableT1lib("no");
		globalParams->setEnableFreeType("yes");
		globalParams->setErrQuiet(gTrue);
		globalParams->setAntialias("yes");
		
		globalParams->setupBaseFonts(baseDir);
		delete baseDir;
		m_Bitmap=0;
		m_PDFDoc=0;
		m_splashOut=0;
		m_bitmapBytes=0;
		m_Outline=0;
	}

	AFPDFDoc::~AFPDFDoc()
	{
		this->Dispose();
	}
	void AFPDFDoc::Dispose(){
		InvalidateBitmapCache();

		if(globalParams!=NULL){
			delete globalParams;
			globalParams=0;
		}
		
		if (m_splashOut!=NULL)
		{
			delete m_splashOut;
			m_splashOut=0;
		}
		
		if (m_PDFDoc!=NULL)
		{
			delete m_PDFDoc;
			m_PDFDoc=0;
		}	
	}


	long AFPDFDoc::LoadFromFile(char *FileName){
		return this->LoadFromFile(FileName,NULL,NULL);
	}
	long AFPDFDoc::LoadFromFile(char *FileName,char *user_password){
		return this->LoadFromFile(FileName,user_password,NULL);
	}

	void AFPDFDoc::SetUserPassword(char *user_password){
		m_UserPassword = user_password;
	}
	void AFPDFDoc::SetOwnerPassword(char *owner_password){
		m_OwnerPassword = owner_password;
	}

	// AFPDFDoc message handlers
	long AFPDFDoc::LoadFromFile(char *FileName, char *user_password, char *owner_password)
	{		
		if(user_password!=NULL)
			m_UserPassword = user_password;
		if(owner_password!=NULL)
			m_OwnerPassword = owner_password;
		//Si la existia lo eliminamos
		if (m_splashOut!=NULL){
			delete m_splashOut;
			m_splashOut=0;
		}
		//Si ya existia la eliminamos
		if (m_PDFDoc!=0){
			delete m_PDFDoc;
			m_PDFDoc=0;
		}
		InvalidateBitmapCache();

		//Establecemos el color del papel
		SplashColor paperColor;
		paperColor[0] = 0xff;
		paperColor[1] = 0xff;
		paperColor[2] = 0xff;
		
		//Note: the alignment is given by GDI requirements: bitmaps have to be 16-bit aligned.
		m_splashOut = new SplashOutputDev(splashModeBGR8, 4, gFalse, paperColor);
		
		//Intentamos abrir el documento sin clave
		m_PDFDoc = new PDFDoc(new GString(FileName), NULL,NULL);
		
		//Esperamos a que se carge correctamente, o que ocurra un error
		while (!m_PDFDoc->isOk()) 
		{
			//En caso de que este encriptado con clave
			if (m_PDFDoc->getErrorCode() == errEncrypted)
			{
				//Si no se especifico clave salimos
				if(m_OwnerPassword.GetLength()<=0 && m_UserPassword.GetLength()<=0) {
					delete m_PDFDoc;
					m_PDFDoc=NULL;
					return errEncrypted;
				}else{
					//Si no se especifico una de las claves, usamos la misma para ambos
					if(m_UserPassword.GetLength()<=0){
						m_UserPassword=m_OwnerPassword;
					}else if(m_OwnerPassword.GetLength()<=0){
						m_OwnerPassword = m_UserPassword;
					}				
					//Intentamos abrir con clave
					m_PDFDoc = new PDFDoc(new GString(FileName), 
							new GString(m_OwnerPassword.GetBuffer()), 
							new GString(m_UserPassword.GetBuffer()));
				}
			} 
			if (!m_PDFDoc->isOk())
			{
				//En caso de error, regresamos el codigo de error
				delete m_PDFDoc;
				m_PDFDoc=NULL;
				return m_PDFDoc->getErrorCode();
			}
			
		}

		//El archivo de cargo correctamente
		m_splashOut->startDoc(m_PDFDoc->getXRef());
		m_Outline = m_PDFDoc->getOutline();

		m_LastPageRendered = -1;
		m_CurrentPage = 0;
		m_SearchPage = 0;
		
		return S_OK;
	}


	
	long AFPDFDoc::RenderPage(long lhWnd)
	{

		if (m_PDFDoc != NULL) {

			int bmWidth, bmHeight;
			/*
			//Heuristically check if we have enough memory ;-)
			double newbytes = (m_renderDPI/72.0)*(m_renderDPI/72.0) * m_PDFDoc->getPageCropWidth(m_CurrentPage)* m_PDFDoc->getPageCropHeight(m_CurrentPage); //new approx. number of pixels
			newbytes = newbytes * 3 + newbytes*2; //24-bit splashbitmap, 16-bit gdi-bitmap
			newbytes=(newbytes*1.2); //Safety area;
			newbytes -= m_splashOut->getBitmap()->getWidth()*m_splashOut->getBitmap()->getHeight()*3; //substract old 24-bit splash bitmap size
			void* testAllocation = malloc((int)newbytes);
			if (testAllocation==0 && newbytes>0){
				//We better dont zoom this far in!
				return errOutOfMemory+1;
			} else {
				free (testAllocation);
			}
			*/
			try{
				m_splashOut->clearModRegion();
				//Wait for previous threads and delete them
				if (m_renderingThread!=0)
				{
					DWORD exitcode=0;
					GetExitCodeThread(m_renderingThread->m_hThread,&exitcode);

					while (exitcode==STILL_ACTIVE){
						GetExitCodeThread(m_renderingThread->m_hThread,&exitcode);
						Sleep(50);
					}
					delete m_renderingThread;
					m_renderingThread=NULL;
				}

				if (m_renderDPI!=m_LastRenderDPI){
					//Invalidate prerendered page
					m_LastPageRenderedByThread=-1;
					//Invalidate cache
					InvalidateBitmapCache();
					m_Bitmap=0;
				}else
					//Read from cache
					m_Bitmap = ::GetBitmapCache(m_CurrentPage);

				if(m_Bitmap==0){
					//Run thread, wait and delete
					if (m_LastPageRenderedByThread != m_CurrentPage)
					{
						m_PageToRenderByThread = m_CurrentPage;
						m_renderingThread = AfxBeginThread((AFX_THREADPROC)AFPDFDoc::RenderingThread,(LPVOID) this,THREAD_PRIORITY_HIGHEST,0,CREATE_SUSPENDED);
						m_renderingThread->m_bAutoDelete=FALSE;
						m_renderingThread->ResumeThread();
						
						DWORD exitcode;
						GetExitCodeThread(m_renderingThread->m_hThread,&exitcode);
						while (exitcode==STILL_ACTIVE){
							GetExitCodeThread(m_renderingThread->m_hThread,&exitcode);
							Sleep(50);
						}
						delete m_renderingThread;
					}
					
					//Rendered bitmap by xpdf
					SplashBitmap * bitmap = m_splashOut->getBitmap();
					bmWidth = bitmap->getWidth();
					bmHeight = bitmap->getHeight();
					m_PageHeight = bmHeight;
					m_PageWidth  = bmWidth;

					CDC clientDC;
					clientDC.Attach(::GetWindowDC((HWND)lhWnd));

					//Bitmap Rebuild
					if(!m_Bitmap || (m_Bitmap && ( m_Bitmap->GetBitmapDimension().cx != bmWidth || m_Bitmap->GetBitmapDimension().cy != bmHeight)))
					{
						//RemoveFromCache(m_CurrentPage);
						m_Bitmap = new CBitmap();
						m_Bitmap->CreateCompatibleBitmap(&clientDC,bmWidth,bmHeight);		
						AddBitmapCache(m_Bitmap,m_CurrentPage);
					}

					//********START DIB
					CDC mdc;
					mdc.CreateCompatibleDC(&clientDC);
					CBitmap* pOld = mdc.SelectObject(m_Bitmap);
					
					BITMAPINFO bmi;
					ZeroMemory(&bmi,sizeof(bmi));
					bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
					bmi.bmiHeader.biWidth=bmWidth;
					bmi.bmiHeader.biHeight=bmHeight;
					bmi.bmiHeader.biPlanes=1;
					bmi.bmiHeader.biBitCount=24;
					bmi.bmiHeader.biCompression=BI_RGB;

					bmi.bmiColors[0].rgbBlue = 0;
					bmi.bmiColors[0].rgbGreen = 0;
					bmi.bmiColors[0].rgbRed = 0;
					bmi.bmiColors[0].rgbReserved = 0;

					::StretchDIBits(mdc.m_hDC,0,0,bmi.bmiHeader.biWidth,bmi.bmiHeader.biHeight,0,bmi.bmiHeader.biHeight,bmi.bmiHeader.biWidth,-bmi.bmiHeader.biHeight,
						(void *)m_splashOut->getBitmap()->getDataPtr(),&bmi,DIB_RGB_COLORS,SRCCOPY);
					
					mdc.SelectObject(pOld);
					mdc.DeleteDC();
					//********END DIB

					m_Bitmap->SetBitmapDimension(m_splashOut->getBitmap()->getWidth(),m_splashOut->getBitmap()->getHeight());

					int box_left, box_top, box_right, box_bottom;
					m_splashOut->getModRegion(&box_left, &box_top, &box_right, &box_bottom);
					m_bbox = CRect(box_left, box_top, box_right, box_bottom);

					clientDC.Detach();
				} 
				//prerender next page
				if (m_CurrentPage+1 <= m_PDFDoc->getNumPages())
				{   
						m_PageToRenderByThread = m_CurrentPage+1;
						m_renderingThread = AfxBeginThread((AFX_THREADPROC)AFPDFDoc::RenderingThread,(LPVOID) this,THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
						m_renderingThread->m_bAutoDelete=FALSE;
						m_renderingThread->ResumeThread();
				}

				m_LastRenderDPI = m_renderDPI;
				m_LastPageRendered=m_CurrentPage;

			} catch(void *e){
				return errOutOfMemory;
			}
		}
		return 0;
	}

	
	UINT AFPDFDoc::RenderingThread( LPVOID param )
	{
		AFPDFDoc *pdfDoc =(AFPDFDoc *)param;
		long a =pdfDoc->m_PageHeight;
		double renderDPI =pdfDoc->m_renderDPI;
		int page = pdfDoc->m_PageToRenderByThread;
		//Verificar los valores de variables
		if (renderDPI==0) renderDPI=72;
		if(page==0) page=1;
		pdfDoc->m_LastPageRenderedByThread=page;

		pdfDoc->m_PDFDoc->displayPage(pdfDoc->m_splashOut,page, 
									renderDPI, renderDPI, pdfDoc->m_Rotation, 
									gFalse, gTrue, gFalse,0,0);
		
		return TRUE;
	}

	long AFPDFDoc::GetCurrentPage(void)
	{
		
		return m_CurrentPage;
	}

	void AFPDFDoc::SetCurrentPage(long newVal)
	{
		m_CurrentPage = newVal;
	}

	long AFPDFDoc::GetCurrentX(void)
	{
		
		return m_ViewOffsetX;
	}

	void AFPDFDoc::SetCurrentX(long newVal)
	{
		
		m_ViewOffsetX = newVal;
	}

	long AFPDFDoc::GetCurrentY(void)
	{
		
		return m_ViewOffsetY;
	}

	void AFPDFDoc::SetCurrentY(long newVal)
	{
		
		m_ViewOffsetY = newVal;
	}


	
	long AFPDFDoc::RenderHDC(long lHdc)
	{
		CDC dc;
		CDC mdc;
		if (m_Bitmap != NULL) 
		{
			dc.Attach((HDC)lHdc);

			// Draw the rendered document
			mdc.CreateCompatibleDC(&dc);
			CBitmap* pOld = mdc.SelectObject(m_Bitmap);

			long clientWidth = this->m_PageWidth;
			long clientHeight = this->m_PageHeight;

			RECT clientArea;
			clientArea.left =0;					clientArea.top=0;
			clientArea.right=clientWidth;		clientArea.bottom =clientHeight;
			
			int bmWidth = m_Bitmap->GetBitmapDimension().cx;
			int bmHeight = m_Bitmap->GetBitmapDimension().cy;
			
			int targetOffsetX=0;			int targetOffsetY=0;
			int viewOffsetX=m_ViewOffsetX;	int viewOffsetY=m_ViewOffsetY;

			BOOL bRet = dc.BitBlt(targetOffsetX,targetOffsetY,clientWidth,clientHeight,&mdc,viewOffsetX,viewOffsetY,SRCCOPY);

			mdc.SelectObject(pOld);
			mdc.DeleteDC();

			//Draw background if neccessary
			CBrush background;
			background.CreateStockObject(GRAY_BRUSH);
			if (viewOffsetX<0)
				dc.FillRect(CRect(clientArea.left,clientArea.top,-viewOffsetX,clientArea.bottom),&background);
			if (viewOffsetY<0)
				dc.FillRect(CRect(clientArea.left,clientArea.top,clientArea.right,-viewOffsetY),&background);
			if (bmWidth-m_ViewOffsetX < clientWidth )
				dc.FillRect(CRect(bmWidth-m_ViewOffsetX,clientArea.top,clientArea.right,clientArea.bottom),&background);
			if (bmHeight-m_ViewOffsetY < clientHeight)
				dc.FillRect(CRect(clientArea.left,bmHeight-m_ViewOffsetY,clientArea.right,clientArea.bottom),&background);

			// draw selection 
			if (!m_HideMarks && m_CurrentPage == m_SearchPage) {
				const double mul = m_renderDPI / FIND_DPI;

				CPen draw_pen, * old_pen;
				CGdiObject * old_brush;

				draw_pen.CreatePen(PS_SOLID, 0, RGB(127, 127, 255));
				old_pen = dc.SelectObject(&draw_pen);
				old_brush = dc.SelectStockObject(NULL_BRUSH);

				for(int j = 0; j < m_Selection.GetCount(); j++) {
					 // transform selection into current DPI and
					 // offset it by current upper/left corner.
					 // Provided m_renderDPI was integer we could
					 // use CRect::MulDiv.
					if (((CPDFSearchResult)m_Selection[j]).PageFound == m_CurrentPage){
						CRect nsel =(CRect)m_Selection[j];
						CRect r;

						nsel.left   = int(nsel.left   * m_renderDPI / FIND_DPI);
						nsel.top    = int(nsel.top    * m_renderDPI / FIND_DPI);
						nsel.right  = int(nsel.right  * m_renderDPI / FIND_DPI);
						nsel.bottom = int(nsel.bottom * m_renderDPI / FIND_DPI);

						// enlarge right/bottom 
						nsel.right++;
						nsel.bottom++;

						// rotate using predefined angles only 
						switch(m_Rotation) {
						case 0:
							r = nsel;
							break;

						case 90:
							r = CRect(
								m_PageWidth - nsel.bottom,
								nsel.left,
								m_PageWidth - nsel.top,
								nsel.right);
							break;

						case 180:
							r = CRect(
								m_PageWidth - nsel.right,
								m_PageHeight - nsel.bottom,
								m_PageWidth - nsel.left,
								m_PageHeight - nsel.top);
							break;

						case 270:
							r = CRect(
								nsel.top,
								m_PageHeight - nsel.right,
								nsel.bottom,
								m_PageHeight - nsel.left);
							break;
						}

						r.OffsetRect(-m_ViewOffsetX, -m_ViewOffsetY);

						dc.Rectangle(r);
						r.DeflateRect(1,1);
						dc.InvertRect(r);
					}
				}

				
				dc.SelectObject(old_pen);
				dc.SelectObject(old_brush);
			}	

			dc.Detach();
		}
		return 0;
	}


	long AFPDFDoc::RenderBitmap(long lhWnd)
	{
		
		CDC dc;
		dc.Attach(::GetWindowDC((HWND)lhWnd));
		
		if (m_Bitmap != NULL) {
			
			// Draw the rendered document
			CDC mdc;
			mdc.CreateCompatibleDC(&dc);
			CBitmap* pOld = mdc.SelectObject(m_Bitmap);

			RECT clientArea;
			::GetClientRect((HWND)lhWnd,&clientArea);
			
			int bmWidth = m_Bitmap->GetBitmapDimension().cx;
			int bmHeight = m_Bitmap->GetBitmapDimension().cy;
			int clientWidth = clientArea.right-clientArea.left;
			int clientHeight = clientArea.bottom-clientArea.top;
			
			int targetOffsetX=0;
			int targetOffsetY=0;
			
			int viewOffsetX=m_ViewOffsetX;
			int viewOffsetY=m_ViewOffsetY;

			BOOL bRet = dc.BitBlt(0,0,clientWidth,clientHeight,&mdc,viewOffsetX,viewOffsetY,SRCCOPY);
			mdc.SelectObject(pOld);

			//Draw background if neccessary
			CBrush background;
			background.CreateStockObject(GRAY_BRUSH);
			if (viewOffsetX<0)
				dc.FillRect(CRect(clientArea.left,clientArea.top,-viewOffsetX,clientArea.bottom),&background);
			if (viewOffsetY<0)
				dc.FillRect(CRect(clientArea.left,clientArea.top,clientArea.right,-viewOffsetY),&background);
			if (bmWidth-m_ViewOffsetX < clientWidth )
				dc.FillRect(CRect(bmWidth-m_ViewOffsetX,clientArea.top,clientArea.right,clientArea.bottom),&background);
			if (bmHeight-m_ViewOffsetY < clientHeight)
				dc.FillRect(CRect(clientArea.left,bmHeight-m_ViewOffsetY,clientArea.right,clientArea.bottom),&background);

			/* draw selection */

			if (!m_HideMarks && m_CurrentPage == m_SearchPage) {
				const double mul = m_renderDPI / FIND_DPI;

				CPen draw_pen, * old_pen;
				CGdiObject * old_brush;

				draw_pen.CreatePen(PS_SOLID, 0, RGB(127, 127, 255));
				old_pen = dc.SelectObject(&draw_pen);
				old_brush = dc.SelectStockObject(NULL_BRUSH);

				for(int j = 0; j < m_Selection.GetCount(); j++) {
					 // transform selection into current DPI and
					 // offset it by current upper/left corner.
					 // Provided m_renderDPI was integer we could
					 // use CRect::MulDiv.
					if (((CPDFSearchResult)m_Selection[j]).PageFound == m_CurrentPage){
						CRect nsel =(CRect)m_Selection[j];
						CRect r;

						nsel.left   = int(nsel.left   * m_renderDPI / FIND_DPI);
						nsel.top    = int(nsel.top    * m_renderDPI / FIND_DPI);
						nsel.right  = int(nsel.right  * m_renderDPI / FIND_DPI);
						nsel.bottom = int(nsel.bottom * m_renderDPI / FIND_DPI);

						// enlarge right/bottom 
						nsel.right++;
						nsel.bottom++;

						// rotate using predefined angles only 
						switch(m_Rotation) {
						case 0:
							r = nsel;
							break;

						case 90:
							r = CRect(
								m_PageWidth - nsel.bottom,
								nsel.left,
								m_PageWidth - nsel.top,
								nsel.right);
							break;

						case 180:
							r = CRect(
								m_PageWidth - nsel.right,
								m_PageHeight - nsel.bottom,
								m_PageWidth - nsel.left,
								m_PageHeight - nsel.top);
							break;

						case 270:
							r = CRect(
								nsel.top,
								m_PageHeight - nsel.right,
								nsel.bottom,
								m_PageHeight - nsel.left);
							break;
						}

						r.OffsetRect(-m_ViewOffsetX, -m_ViewOffsetY);

						dc.Rectangle(r);
						r.DeflateRect(1,1);
						dc.InvertRect(r);
					}
				}

				dc.SelectObject(old_pen);
				dc.SelectObject(old_brush);
			}
		}
		return -2;
	}

	long AFPDFDoc::LoadFromFile2(char * FileName)
	{
		
		return LoadFromFile(FileName);
	}

	long AFPDFDoc::GetPageCount(void)
	{
		
		if(m_PDFDoc){
			if(m_PDFDoc->isOk()){
				return m_PDFDoc->getNumPages();
			}
		}
		return 0;
	}


	long AFPDFDoc::NextPage(void)
	{
		
		if(m_PDFDoc){
			long pageno = m_CurrentPage + 1;
			if(pageno>=1 && pageno<=m_PDFDoc->getNumPages()){
				m_CurrentPage = pageno;
			}
		}
		return m_CurrentPage;
	}

	long AFPDFDoc::PreviousPage(void)
	{
		
		if(m_PDFDoc){
			long pageno = m_CurrentPage - 1;
			if(pageno>=1 && pageno<=m_PDFDoc->getNumPages()){
				m_CurrentPage = pageno;
			}
		}
		return m_CurrentPage;
	}

	long AFPDFDoc::GotoPage(long PageNumber)
	{
		
		if(m_PDFDoc){
			if(PageNumber>=1 && PageNumber<=m_PDFDoc->getNumPages()){
				m_CurrentPage = PageNumber;
			}
		}
		return m_CurrentPage;
	}

	long AFPDFDoc::ZoomIN(void)
	{
		
		m_renderDPI =m_renderDPI * 4 / 3;
		return m_renderDPI;
	}

	long AFPDFDoc::ZoomOut(void)
	{
		
		m_renderDPI =m_renderDPI *  3/4;
		return m_renderDPI;
	}

	long AFPDFDoc::FitScreenWidth(long lhWnd)
	{
		
		if (m_PDFDoc!=NULL)
		{
			RECT clientArea;

			//We start with invisible scrollbars. If needed, we enable SB_VERT later.
//			SetScrollRange(SB_VERT,0,0);
//			SetScrollRange(SB_HORZ,0,0);

			::GetClientRect((HWND)lhWnd, &clientArea);
			double height = clientArea.bottom-clientArea.top;
			double width = clientArea.right-clientArea.left;
			double contentHeight = 0;
			if (m_Rotation==0||m_Rotation==180){
				contentHeight = m_PDFDoc->getPageCropHeight(m_CurrentPage);
			} else {
				contentHeight = m_PDFDoc->getPageCropWidth(m_CurrentPage);
			}
			double contentWidth = 0;
			if (m_Rotation==0||m_Rotation==180){
				contentWidth = m_PDFDoc->getPageCropWidth(m_CurrentPage);
			} else {
				contentWidth = m_PDFDoc->getPageCropHeight(m_CurrentPage);
			}

			m_renderDPI = 72.0/contentWidth*width;
			//Now we know the required DPI if there weren't any scrollbars

	//		if (!m_ConfigDialog.m_HideScrollbars)
			{
				double targetHeight = (m_renderDPI*contentHeight)/72.0;
				if (targetHeight>height)
				{
					width -= GetSystemMetrics(SM_CXVSCROLL);
	/*
					SetScrollRange(SB_VERT,0,100);
					GetClientRect(&clientArea);
					height = clientArea.bottom-clientArea.top;
					width = clientArea.right-clientArea.left;
	*/
				} 
				m_renderDPI = 72.0/contentWidth*width;
				//Now the result takes the potential scrollbar into account
			}
		}
		return m_renderDPI;
	}

	long AFPDFDoc::FitScreenHeight(long lhWnd)
	{
		

			if (m_PDFDoc!=NULL){
			RECT clientArea;

			//We start with invisible scrollbars. If needed, we enable SB_HORZ later.
//			SetScrollRange(SB_VERT,0,0);
//			SetScrollRange(SB_HORZ,0,0);

			::GetClientRect((HWND)lhWnd,&clientArea);
			double height = clientArea.bottom-clientArea.top;
			double width = clientArea.right-clientArea.left;
			double contentHeight = 0;
			if (m_Rotation==0||m_Rotation==180){
				contentHeight = m_PDFDoc->getPageCropHeight(m_CurrentPage);
			} else {
				contentHeight = m_PDFDoc->getPageCropWidth(m_CurrentPage);
			}
			double contentWidth = 0;
			if (m_Rotation==0||m_Rotation==180){
				contentWidth = m_PDFDoc->getPageCropWidth(m_CurrentPage);
			} else {
				contentWidth = m_PDFDoc->getPageCropHeight(m_CurrentPage);
			}

			m_renderDPI = 72.0/contentHeight*height;
			//Now we know the required DPI if there weren't any scrollbars
	//		if (!m_ConfigDialog.m_HideScrollbars)
			{
				double targetWidth = (m_renderDPI*contentWidth)/72.0;
				if (targetWidth > width)
				{
					height -= GetSystemMetrics(SM_CYHSCROLL);
	/*				SetScrollRange(SB_VERT,0,targetWidth-1);
					GetClientRect(&clientArea);
					height = clientArea.bottom-clientArea.top;
					width = clientArea.right-clientArea.left;
	*/
				} 
				m_renderDPI = 72.0/contentHeight*height;
				//Now the result takes the potential scrollbar into account
			}
		}
		return m_renderDPI;
	}

	long AFPDFDoc::GetPageWidth(void)
	{
		
		return m_PageWidth;
	}

	long AFPDFDoc::GetPageHeight(void)
	{
		
		return m_PageHeight;
	}


	OutlineItemA* AFPDFDoc::GetOutline(long iOutline)
	{
		
		
		/*if(m_Outline){
			if(m_Outline->getItems()){
				if (iOutline > 0 && iOutline <= m_Outline->getItems()->getLength()){
					OutlineItemA *it=new OutlineItemA; //(m_Outline.items->get(iOutline));		
					OutlineItem *ol;
					ol =(OutlineItem *) m_Outline->getItems()->get(iOutline-1);
					//((IOutlineItemA *)it)->SetOutlineItemXPDF((long)ol);
					it->IOutlineItemA->SetOutlineItemXPDF((long)ol);
					//((IOutlineItemA *)it)
					return (IDispatch *)it;
				}
			}
		}*/
		return NULL;
	}

	long AFPDFDoc::GetOutlineCount(void)
	{
		
		if(m_Outline){
			if (m_Outline->getItems())
				return m_Outline->getItems()->getLength();
			else
				return 0;
		}
	}


	long AFPDFDoc::GetOutlinePtr(long iOutline)
	{
		
		if(m_Outline){
			if(m_Outline->getItems()){
				if (iOutline > 0 && iOutline <= m_Outline->getItems()->getLength()){
					OutlineItem *ol;
					ol =(OutlineItem *) m_Outline->getItems()->get(iOutline-1);
					return (long)ol;
				}
			}
		}

		return 0;
	}

	long AFPDFDoc::ProcessLinkAction(long lPtrLinkAction)
	{
		
		LinkAction * link = (LinkAction *)lPtrLinkAction;
		if (link == NULL)
			return 0;
		
		if (link->getKind() == actionGoTo) {
			LinkGoTo * p = (LinkGoTo *)link;
			LinkDest * dest;
			GString  * namedDest;
			int pageno;
			int x = m_ViewOffsetX, y = m_ViewOffsetY;
			int dx, dy;
			dest = p->getDest();

			if (dest == NULL) {
				if ((namedDest = p->getNamedDest()) == NULL)
					return 0;

				dest = m_PDFDoc->findDest(namedDest);
			}

			if (dest == NULL || !dest->isOk())
				return 0;

			if (dest->isPageRef()) {
				Ref ref = dest->getPageRef();
				pageno = m_PDFDoc->getCatalog()->findPage(ref.num, ref.gen);
			} else
				pageno = dest->getPageNum();

			if (pageno <= 0 || pageno >= m_PDFDoc->getNumPages())
				return 0;

			// go to the page 
			if(pageno>=1 && pageno<=m_PDFDoc->getNumPages()){
				m_CurrentPage = pageno;
			}

			switch (dest->getKind()) {
			case destXYZ:
				{
					m_splashOut->cvtUserToDev(dest->getLeft(),
						dest->getTop(), &dx, &dy);

					if (dest->getChangeLeft()) x = dx;
					if (dest->getChangeTop())  y = dy;
					// ignore zoom 
				}
				break;
				// preserve user defined zoom factor, so do not fit 
			case destFit:
			case destFitB:
				x = y = 0;
				break;
			case destFitH:
			case destFitBH:
				//~ do fit
				m_splashOut->cvtUserToDev(0, dest->getTop(), &dx, &dy);
				x = 0;
				y = dy;
				break;
			case destFitV:
			case destFitBV:
				//~ do fit
				m_splashOut->cvtUserToDev(dest->getLeft(), 0, &dx, &dy);
				x = dx;
				y = 0;
				break;
			case destFitR:
				//~ do fit
				m_splashOut->cvtUserToDev(dest->getLeft(), dest->getTop(), &dx, &dy);
				x = dx;
				y = dy;
				break;
			}
			m_splashOut->cvtUserToDev(0, dest->getTop(), &dx, &dy);
			return (long)dy;
		}
		return 0;
	}

	float AFPDFDoc::GetRenderDPI(void)
	{		
		return m_renderDPI;
	}

	void AFPDFDoc::SetRenderDPI(FLOAT newVal)
	{
		
		m_renderDPI = newVal;
	}

	wchar_t *AFPDFDoc::GetPDFVersion(void)
	{
		

		CString strResult;
		char version[32];
		if(m_PDFDoc){
			sprintf(version,"%f",m_PDFDoc->getPDFVersion());
			strResult=version;
		}
		return strResult.AllocSysString();
	}

	long AFPDFDoc::FindText(LPCTSTR sText, long iPage, long SearchOrder, bool bCaseSensitive, bool bBackward, bool bMarkAll, bool bWholeDoc)
	{
		

		double x0, y0, x1, y1;
		long iFirstPageFound=0;
		long searchPage=0;
		GBool rc, startAtTop, startAtLast, backward;
		Unicode * ucstring;
		TextOutputDev FindPage(NULL, gTrue, gFalse, gFalse);

		CString theString(sText);
		int length = theString.GetLength();
		
		//Tratar de reservar la cadena
		ucstring = GetUnicodeString(theString, length);
		if (ucstring == NULL) {
			AfxMessageBox("Out of memory");
			return FALSE;
		}

		m_SearchStarted=false;
		startAtTop = gTrue;
		startAtLast = gFalse;
		backward = bBackward; 
		m_SearchStarted = FALSE;
		m_Selection.RemoveAll();

		//Si se desea buscar desde el principio
		if(SearchOrder==0)
			searchPage=1;
		else
			searchPage=iPage;

		/* use fixed DPI and rotation, this would lead to somehwere
		 * wrong positioning displaying more than 300 DPI document
		 * don't think it's a problem thought
		 */
		m_PDFDoc->displayPage(&FindPage, searchPage,
			FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);

		if (!FindPage.isOk()) {
			AfxMessageBox("Error al mostrar la pagina");
			delete [] ucstring;
			return -1;
		}

		//Mientras haya que hacer
		while(true) {
			//Buscar el texto
			rc = FindPage.findText(ucstring, length,
				startAtTop, gTrue, startAtLast, gFalse,
				bCaseSensitive, backward,
				&x0, &y0, &x1, &y1);
			//Si existen resultados, agregamos esta coincidencia a la lista
			if (rc) {
				m_Selection.Add(CPDFSearchResult(CRect((int)x0, (int)y0, (int)x1, (int)y1),searchPage));
				startAtTop = gFalse;
				startAtLast = gTrue;
				if(iFirstPageFound==0) //La primer pagina en donde se encontro
					iFirstPageFound =searchPage;
				continue;
			}

			if(!bWholeDoc)	//Si no se desea buscar en todo el documento, salimos
				if (m_Selection.GetCount() > 0)
					break;

			// Ir a la siguiente pagina
			startAtTop = gTrue;
			startAtLast = gFalse;

			// Si se desea buscar en todo el documento
			if(bWholeDoc)
				if (backward)	//Buscar hacia atras
					searchPage--;
				else
					searchPage++;
			else
				break;	// Salir si no se deseaba buscar desde el primer registro

			//Si ya no hay paginas para buscar
			if (searchPage < 1 || searchPage > m_PDFDoc->getNumPages()) {
				delete [] ucstring;
				m_SearchPage=iFirstPageFound;
				return m_Selection.GetCount();
			}
			//Dibujar la siguiente pagina
			m_PDFDoc->displayPage(&FindPage, searchPage,
				FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);

			if (!FindPage.isOk()) {
				AfxMessageBox("Error al mostrar la pagina");
				delete [] ucstring;
				return -1;	//No hay resultados
			}
		}
		//Establecer la pagina de busqueda en la primer pagina
		m_SearchPage=iFirstPageFound;
		delete [] ucstring;
		return m_Selection.GetCount();
	}
	double __x0=0;
	double __y0=0;
	long AFPDFDoc::FindNext(LPCTSTR sText)
	{
		

		if (!m_SearchStarted)
			return 0;

		//Comenzar desde donde se quedo la ultima búsqueda
		double x0=__x0, y0=__y0, x1, y1;
		long iFirstPageFound=0, searchPage=0;
		searchPage=m_SearchPage;
		GBool rc, startAtTop, startAtLast;
		Unicode * ucstring;
		TextOutputDev FindPage(NULL, gTrue, gFalse, gFalse);

		CString theString(sText);
		int length = theString.GetLength();
		
		//Tratar de reservar la cadena
		ucstring = GetUnicodeString(theString, length);
		if (ucstring == NULL) {
			AfxMessageBox("Out of memory");
			return FALSE;
		}
		//Comenzar en xMin, yMin
		startAtTop = gFalse;
		startAtLast = gFalse;
		m_SearchStarted = FALSE;
		m_Selection.RemoveAll();
		
		/* use fixed DPI and rotation, this would lead to somehwere
		 * wrong positioning displaying more than 300 DPI document
		 * don't think it's a problem thought
		 */
		m_PDFDoc->displayPage(&FindPage, searchPage,
			FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);

		if (!FindPage.isOk()) {
			AfxMessageBox("Error al mostrar la pagina");
			delete [] ucstring;
			return -1;
		}

		//Mientras haya que hacer
		while(true) {
			//Buscar el texto
			rc = FindPage.findText(ucstring, length,
				startAtTop, gTrue, startAtLast, gFalse,
				m_bCaseSensitive, gFalse,
				&x0, &y0, &x1, &y1);

			//Si existen resultados, agregamos esta coincidencia a la lista
			if (rc) {
				m_Selection.Add(CPDFSearchResult(CRect((int)x0, (int)y0, (int)x1, (int)y1),searchPage));
				//La primer pagina en donde se encontro
				__x0=x0;
				__y0=y0;
				iFirstPageFound =searchPage;
				break;
			}else{
				// Ir a la pagina sigueinte
				startAtTop = gTrue;
				startAtLast = gFalse;
				searchPage++;	
				//Si ya no hay paginas para buscar
				if (searchPage < 1 || searchPage > m_PDFDoc->getNumPages()) {
					delete [] ucstring;
					m_SearchPage=iFirstPageFound;
					return m_Selection.GetCount();
				}
				//Dibujar la siguiente pagina
				m_PDFDoc->displayPage(&FindPage, searchPage,
					FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);
				//Si no se mostro correctamente, salir
				if (!FindPage.isOk()) {
					AfxMessageBox("Error al mostrar la pagina");
					delete [] ucstring;
					return -1;	//No hay resultados
				}
			}
		}
		
		
		//Establecer la pagina de busqueda en la primer ocurrencia
		m_SearchPage=iFirstPageFound;
		m_SearchStarted = true;
		delete [] ucstring;
		return m_Selection.GetCount();
	}

	long AFPDFDoc::FindPrior(LPCTSTR sText)
	{
		

		if (!m_SearchStarted)
			return 0;

		//Comenzar desde donde se quedo la ultima búsqueda
		double x0=__x0, y0=__y0, x1, y1;
		long iFirstPageFound=0, searchPage=0;
		searchPage=m_SearchPage;
		GBool rc, startAtTop, startAtLast;
		Unicode * ucstring;
		TextOutputDev FindPage(NULL, gTrue, gFalse, gFalse);

		CString theString(sText);
		int length = theString.GetLength();
		
		//Tratar de reservar la cadena
		ucstring = GetUnicodeString(theString, length);
		if (ucstring == NULL) {
			AfxMessageBox("Out of memory");
			return FALSE;
		}
		//Comenzar en xMin, yMin
		startAtTop = gFalse;
		startAtLast = gFalse;
		m_SearchStarted = FALSE;
		m_Selection.RemoveAll();
		
		/* use fixed DPI and rotation, this would lead to somehwere
		 * wrong positioning displaying more than 300 DPI document
		 * don't think it's a problem thought
		 */
		m_PDFDoc->displayPage(&FindPage, searchPage,
			FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);

		if (!FindPage.isOk()) {
			AfxMessageBox("Error al mostrar la pagina");
			delete [] ucstring;
			return -1;
		}

		//Mientras haya que hacer
		while(true) {
			//Buscar el texto
			rc = FindPage.findText(ucstring, length,
				startAtTop, gTrue, startAtLast, gFalse,
				m_bCaseSensitive,gTrue,
				&x0, &y0, &x1, &y1);

			//Si existen resultados, agregamos esta coincidencia a la lista
			if (rc) {
				m_Selection.Add(CPDFSearchResult(CRect((int)x0, (int)y0, (int)x1, (int)y1),searchPage));
				//La primer pagina en donde se encontro
				__x0=x0;
				__y0=y0;
				iFirstPageFound =searchPage;
				break;
			}else{
				// Ir a la pagina sigueinte
				startAtTop = gTrue;
				startAtLast = gFalse;
				searchPage--;	
				//Si ya no hay paginas para buscar
				if (searchPage < 1 || searchPage > m_PDFDoc->getNumPages()) {
					delete [] ucstring;
					m_SearchPage=iFirstPageFound;
					return m_Selection.GetCount();
				}
				//Dibujar la siguiente pagina
				m_PDFDoc->displayPage(&FindPage, searchPage,
					FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);
				//Si no se mostro correctamente, salir
				if (!FindPage.isOk()) {
					AfxMessageBox("Error al mostrar la pagina");
					delete [] ucstring;
					return -1;	//No hay resultados
				}
			}
		}
		
		
		//Establecer la pagina de busqueda en la primer ocurrencia
		m_SearchPage=iFirstPageFound;
		m_SearchStarted = true;
		delete [] ucstring;
		return m_Selection.GetCount();
	}

	CPDFSearchResult *AFPDFDoc::GetSearchResults(){
		return m_Selection.ToArray();
	}

	long AFPDFDoc::GetSearchCount(){
		return m_Selection.GetCount();
	}

	long AFPDFDoc::PrintToFile(LPCTSTR fileName, int fromPage, int toPage){
		if(!m_PDFDoc->okToPrint())
			return -1;
				
		PSOutputDev *psOut =new PSOutputDev((char *)fileName,m_PDFDoc->getXRef(),m_PDFDoc->getCatalog(),fromPage,toPage,psModePS);
		if(!psOut->isOk()){
			delete psOut;
			return -2;
		}
		m_PDFDoc->displayPages(psOut,fromPage,toPage,PRINT_DPI,PRINT_DPI,0,gTrue,globalParams->getPSCrop(),gTrue);

		delete psOut;
		return 0;
	}

	long AFPDFDoc::FindFirst(LPCTSTR sText, long SearchOrder, bool Backward)
	{
		

		double x0, y0, x1, y1;
		long iFirstPageFound=0;
		long searchPage=0;
		GBool rc, startAtTop, startAtLast, backward;
		Unicode * ucstring;
		TextOutputDev FindPage(NULL, gTrue, gFalse, gFalse);

		CString theString(sText);
		int length = theString.GetLength();
		
		//Tratar de reservar la cadena
		ucstring = GetUnicodeString(theString, length);
		if (ucstring == NULL) {
			AfxMessageBox("Out of memory");
			return FALSE;
		}

		startAtTop = gTrue;
		startAtLast = gFalse;
		backward = Backward; 
		m_SearchStarted = FALSE;
		m_Selection.RemoveAll();

		//Si se desea buscar desde el principio
		if(SearchOrder==0)
			searchPage=1;
		else
			searchPage=m_CurrentPage;

		/* use fixed DPI and rotation, this would lead to somehwere
		 * wrong positioning displaying more than 300 DPI document
		 * don't think it's a problem thought
		 */
		m_PDFDoc->displayPage(&FindPage, searchPage,
			FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);

		if (!FindPage.isOk()) {
			AfxMessageBox("Error al mostrar la pagina");
			delete [] ucstring;
			return -1;
		}

		//Mientras haya que hacer
		while(true) {
			//Buscar el texto
			rc = FindPage.findText(ucstring, length,
				startAtTop, gTrue, startAtLast, gFalse,
				m_bCaseSensitive, backward,
				&x0, &y0, &x1, &y1);
			//Si existen resultados, agregamos esta coincidencia a la lista
			if (rc) {
				m_Selection.Add(CPDFSearchResult(CRect((int)x0, (int)y0, (int)x1, (int)y1),searchPage));
				__x0=x0;
				__y0=y0;
				iFirstPageFound =searchPage;
				break;
			}

			// Ir a la siguiente pagina
			startAtTop = gTrue;
			startAtLast = gFalse;

			if (backward)	//Buscar hacia atras
				searchPage--;
			else
				searchPage++;

			//Si ya no hay paginas para buscar
			if (searchPage < 1 || searchPage > m_PDFDoc->getNumPages()) {
				/*delete [] ucstring;
				m_SearchPage=iFirstPageFound;
				return m_Selection.GetCount();
				*/
				break;
			}
			//Dibujar la siguiente pagina
			m_PDFDoc->displayPage(&FindPage, searchPage,
				FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);

			if (!FindPage.isOk()) {
				AfxMessageBox("Error al mostrar la pagina");
				delete [] ucstring;
				return -1;	//No hay resultados
			}
		}
		if(iFirstPageFound>0)
			m_SearchStarted=true;
		
		//Establecer la pagina de busqueda en la primer pagina
		m_SearchPage=iFirstPageFound;
		delete [] ucstring;
		return m_Selection.GetCount();
	}

	long AFPDFDoc::GetSearchPage(void)
	{
		
		return m_SearchPage;
	}

	long AFPDFDoc::SetSearchPage(long lNewValue)
	{
		
		m_SearchPage=lNewValue;
		return 0;
	}
	bool AFPDFDoc::GetSearchCaseSensitive(void)
	{
		
		return m_bCaseSensitive;
	}

	void AFPDFDoc::SetSearchCaseSensitive(bool newVal)
	{
		
		m_bCaseSensitive=newVal;
	}
	
	char * AFPDFDoc::getTitle(){
		Object info;
		UnicodeMap *uMap;
		if (!(uMap = globalParams->getTextEncoding())) {
//			error(-1, "Couldn't get text encoding");
			return EmptyChar;			
		}

		m_PDFDoc->getDocInfo(&info);
		if (info.isDict()) {
			Dict *infoDict = info.getDict();
			return getDicString(infoDict,"Title",uMap);
		}
		return EmptyChar;
	}

	char * AFPDFDoc::getAuthor(){
		Object info;
		UnicodeMap *uMap;
		if (!(uMap = globalParams->getTextEncoding())) {
//			error(-1, "Couldn't get text encoding");
			return EmptyChar;			
		}

		m_PDFDoc->getDocInfo(&info);
		if (info.isDict()) {
			Dict *infoDict = info.getDict();
			return getDicString(infoDict,"Author",uMap);
		}
		return EmptyChar;
	}
		/*
	char * getSubject();
	char * getKeywords();
	char * getCreator();
	char * getProducer();
	char * getCreationDate();
	char * getLastModifiedDate();
*/

	Links *AFPDFDoc::GetLinksPage(long iPage)
	{
		Links *l=m_PDFDoc->getLinks(iPage);
		return l;
	}

	void AFPDFDoc::cvtUserToDev(double ux, double uy, int *dx, int *dy){
		m_splashOut->cvtUserToDev(ux,uy,dx,dy);	
	}

	void AFPDFDoc::cvtDevToUser(double ux, double uy, double *dx, double *dy){
		m_splashOut->cvtDevToUser(ux,uy,dx,dy);
	}