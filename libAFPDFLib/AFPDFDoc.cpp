// AFPDFDoc.cpp : implementation file
#include "AFPDFDoc.h"
#include "jpeg.h"
#include "error.h"
#include "GMutex.h"
#include "StreamCallback.h"

	//------DECLARATIONS	
	#define			FIND_DPI			72
	#define			PRINT_DPI			150
	#define			SPACE_X				16
	#define			SPACE_Y				16
	#define			MAX(a,b)			a>b?a:b			
	#define			IFZERO(a,b)			a==0?b:a;
	
	static wchar_t		EmptyChar[1]						={'\0'};
	

	struct threadParam {
		threadParam(AFPDFDoc *pdfDoc,int page){
			doc=pdfDoc;
			enablePreRender=true;
			pageToRender=page;
		}
		AFPDFDoc *doc;	
		SplashOutputDev *out;
		bool enablePreRender;
		int pageToRender;
	};


	void OutputToDelegate(void *stream, char *str, int len){

		GBool isUnicode;
		Unicode u;
		int i;
		int j=0;
		GString s;
		
		if ((str[0] & 0xff) == 0xfe && (str[1] & 0xff) == 0xff) {
			isUnicode = gTrue;
			i = 2;
		} else {
			isUnicode = gFalse;
			i = 0;
		}
		
		wchar_t *ret =new wchar_t[len+1];
		
		while (i < len) {
			  if (isUnicode) {
					u = ((str[i] & 0xff) << 8) |  (str[i+1] & 0xff);
					i += 2;
			  } else {
					u = str[i] & 0xff;
					++i;
			  }
			  ret[j] = u;
			  j++;
		}
		ret[j]='\0';

		if(stream)
			((OUTPUTFUNCTION)stream)(ret,len);

		delete ret;
	}

	void logInfo(char *string){
		//if(globalParams->getErrQuiet()) return;

		time_t ltime;
		 char tmpbuf[128], timebuf[26], ampm[] = "AM";

		time( &ltime );
		//printf( "Time in seconds since UTC 1/1/70:\t%ld\n", ltime );
		int err = ctime_s(timebuf, 26, &ltime);
		
//		printf( "UNIX time and date:\t\t\t%s", timebuf );


		fprintf(stderr,"%s\tINFO: %s\n",timebuf,string);
		fflush(stderr);
	}
	//------PRINT INFORMATION
	void InitGlobalParams(char *configFile){
		if(globalParams==0){
			TCHAR szExe[MAX_PATH];
			int size = ::GetModuleFileName(NULL, szExe, MAX_PATH); 
			TCHAR *pLastSlash = _tcsrchr(szExe, ('\\'));
			
			if (pLastSlash){
				// Truncate at slash to get app dir
				*pLastSlash=L'\0';
			}

			char *baseDir = new char[wcslen((const wchar_t *)szExe)+1];
			sprintf(baseDir,"%S",szExe);

			globalParams = new GlobalParams(configFile);
			//Initialize default settings
			globalParams->setupBaseFonts(baseDir);
			globalParams->setErrQuiet(gFalse);
			globalParams->setEnableT1lib("no");
			globalParams->setEnableFreeType("yes");
			globalParams->setPSEmbedCIDPostScript(1);
			globalParams->setPSEmbedCIDTrueType(1);
			globalParams->setPSEmbedTrueType(1);
			globalParams->setPSEmbedType1(1);
			globalParams->setAntialias("yes");
			globalParams->setVectorAntialias("yes");
			globalParams->setTextEncoding("UTF-8");
			delete baseDir;
		}
	}

	static GString* getInfoString(Dict *infoDict, char *key) {
	  Object obj;
	  GString *s1 = NULL;

	  if (infoDict->lookup(key, &obj)->isString()) {
		s1 = new GString(obj.getString());
	  }
	  obj.free();
	  return s1;
	}

	static GString* getInfoDate(Dict *infoDict, char *key) {
	  Object obj;
	  char *s;
	  int year, mon, day, hour, min, sec;
	  struct tm tmStruct;
	  GString *result = NULL;
	  char buf[256];

	  if (infoDict->lookup(key, &obj)->isString()) {
		s = obj.getString()->getCString();
		if (s[0] == 'D' && s[1] == ':') {
		  s += 2;
		}
		if (sscanf(s, "%4d%2d%2d%2d%2d%2d",
				   &year, &mon, &day, &hour, &min, &sec) == 6) {
		  tmStruct.tm_year = year - 1900;
		  tmStruct.tm_mon = mon - 1;
		  tmStruct.tm_mday = day;
		  tmStruct.tm_hour = hour;
		  tmStruct.tm_min = min;
		  tmStruct.tm_sec = sec;
		  tmStruct.tm_wday = -1;
		  tmStruct.tm_yday = -1;
		  tmStruct.tm_isdst = -1;
		  mktime(&tmStruct); // compute the tm_wday and tm_yday fields
		  if (strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S+00:00", &tmStruct)) {
		result = new GString(buf);
		  } else {
			result = new GString(s);
		  }
		} else {
		  result = new GString(s);
		}
	  }
	  obj.free();
	  return result;
	}



	static void printInfoString		(FILE *f,Dict *infoDict, char *key, char *text,  UnicodeMap *uMap)
	{
	  Object obj;
	  GString *s1;
	  GBool isUnicode;
	  Unicode u;
	  char buf[8];
	  int i, n;

	  if (infoDict->lookup(key, &obj)->isString()) {
		fputs(text, stdout);
		s1 = obj.getString();
		if ((s1->getChar(0) & 0xff) == 0xfe &&
		(s1->getChar(1) & 0xff) == 0xff) {
		  isUnicode = gTrue;
		  i = 2;
		} else {
		  isUnicode = gFalse;
		  i = 0;
		}
		while (i < obj.getString()->getLength()) {
		  if (isUnicode) {
		u = ((s1->getChar(i) & 0xff) << 8) |
			(s1->getChar(i+1) & 0xff);
		i += 2;
		  } else {
		u = pdfDocEncoding[s1->getChar(i) & 0xff];
		++i;
		  }
		  n = uMap->mapUnicode(u, buf, sizeof(buf));
		  fwrite(buf, 1, n, stdout);
		}
		fputc('\n', stdout);
	  }
	  obj.free();
	}

	static void parseDateTime(char *datetime, const char *s){
		if (s[0] == 'D' && s[1] == ':') {
		  s += 2;
		}
		int year, mon, day, hour, min, sec, n;
		struct tm tmStruct;
		char buf[256];
		if ((n = sscanf(s, "%4d%2d%2d%2d%2d%2d",
				&year, &mon, &day, &hour, &min, &sec)) >= 1) {
		  switch (n) {
		  case 1: mon = 1;
		  case 2: day = 1;
		  case 3: hour = 0;
		  case 4: min = 0;
		  case 5: sec = 0;
		  }
		  tmStruct.tm_year = year - 1900;
		  tmStruct.tm_mon = mon - 1;
		  tmStruct.tm_mday = day;
		  tmStruct.tm_hour = hour;
		  tmStruct.tm_min = min;
		  tmStruct.tm_sec = sec;
		  tmStruct.tm_wday = -1;
		  tmStruct.tm_yday = -1;
		  tmStruct.tm_isdst = -1;
		  // compute the tm_wday and tm_yday fields
		  if (mktime(&tmStruct) != (time_t)-1 &&  strftime(buf, sizeof(buf), "%c", &tmStruct)) {
			strcpy(datetime,buf);
		  } else {
			  strcpy(datetime,s);
		  }
		} else {
		  strcpy(datetime,s);
		}
	}
	static void sprintInfoDate		(char *datetime,Dict *infoDict, char *key, char *text) {
	  Object obj;
	  char *s;

	  if (infoDict->lookup(key, &obj)->isString()) {
		fputs(text, stdout);
		s = obj.getString()->getCString();
		parseDateTime(datetime,s);
	  }
	  obj.free();
	}
	static void printInfoDate		(Dict *infoDict, char *key, char *text) {
	  char buf[256];
	  sprintInfoDate(buf,infoDict,key,text);
	  fputs(buf,stdout);
	  fputc('\n',stdout);
	}

	static void printBox			(char *text, PDFRectangle *box) { printf("%s%8.2f %8.2f %8.2f %8.2f\n",
		 text, box->x1, box->y1, box->x2, box->y2);
	}

	static void printInfoString		(FILE *f, Dict *infoDict, char *key, char *text1, char *text2, UnicodeMap *uMap) 
	{
	  Object obj;
	  GString *s1;
	  GBool isUnicode;
	  Unicode u;
	  char buf[8];
	  int i, n;

	  if (infoDict->lookup(key, &obj)->isString()) {
		fputs(text1, f);
		s1 = obj.getString();
		if ((s1->getChar(0) & 0xff) == 0xfe &&
		(s1->getChar(1) & 0xff) == 0xff) {
		  isUnicode = gTrue;
		  i = 2;
		} else {
		  isUnicode = gFalse;
		  i = 0;
		}
		while (i < obj.getString()->getLength()) {
		  if (isUnicode) {
		u = ((s1->getChar(i) & 0xff) << 8) |
			(s1->getChar(i+1) & 0xff);
		i += 2;
		  } else {
		u = s1->getChar(i) & 0xff;
		++i;
		  }
		  n = uMap->mapUnicode(u, buf, sizeof(buf));
		  fwrite(buf, 1, n, f);
		}
		fputs(text2, f);
	  }
	  obj.free();
	}

	static void printInfoDate		(FILE *f, Dict *infoDict, char *key, char *fmt) {
	  Object obj;
	  char *s;

	  if (infoDict->lookup(key, &obj)->isString()) {
		s = obj.getString()->getCString();
		if (s[0] == 'D' && s[1] == ':') {
		  s += 2;
		}
		fprintf(f, fmt, s);
	  }
	  obj.free();
	}

	wchar_t * GetTitle(UnicodeMap *uMap, OutlineItem *item)
	{
		
		wchar_t *ret;
		int j;
		//char buf[8];
		//GString *title=new GString();
//		char *s;
		if(item /*&& uMap!=NULL*/){
			ret =new wchar_t[item->getTitleLength()+1];

			//12/July/2009 - Allow windows to map unicode characters, 
			for (j = 0; j < item->getTitleLength(); ++j) {
			  //n = uMap->mapUnicode(m_Item->getTitle()[j], buf, sizeof(buf));
			  //title->append(buf, n);
			  ret[j]=(wchar_t)item->getTitle()[j];
			}
			ret[j]='\0';
			//s = title->getCString();
			
		}else{
			return L"\0";
		}
		//USES_CONVERSION;
		//ret =A2W(s);
		return ret;
	}

	
	
	//------DICTIONARY STRING
	Unicode			*	GetUnicodeString(const wchar_t*str, int length)
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
	static wchar_t	*	getDicString(Dict *infoDict,char *key,UnicodeMap *uMap)
	{
		Object obj;
		GString *s1;
		GBool isUnicode;
		Unicode u;
//		char buf[8];
		int i;

	
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
			
			wchar_t *ret =new wchar_t[s1->getLength()];
			int j=0;
			i=0;
			while (i < s1->getLength()) {
				  if (isUnicode) {
						u = ((s1->getChar(i) & 0xff) << 8) |  (s1->getChar(i+1) & 0xff);
						i += 2;
				  } else {
						u = s1->getChar(i) & 0xff;
						++i;
				  }
				  ret[j] = u;
				  j++;
			}
			ret[j]='\0';
			
			if(s1->getLength()>0)
				return ret;

		}
		return EmptyChar;
		
	}


	static wchar_t	*	getDocInfo(char *key,PDFDoc *doc){
		Object info;
		UnicodeMap *uMap;
		if (!(uMap = globalParams->getTextEncoding())) {
			return (EmptyChar);			
		}

		doc->getDocInfo(&info);
		if (info.isDict()) {
			Dict *infoDict = info.getDict();
			return getDicString(infoDict,key,uMap);
		}
		return (EmptyChar);
	}



	//------PDF SEARCH RESULT
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




	
	//------BITMAP CACHE
	PageMemory *AFPDFDoc::GetBitmapCache(int page){
		for(int i=0;i<MAX_BITMAP_CACHE;i++){
			if(_pageCached[i]==page){
				return _bitmapCache[i];
			}
		}
		return 0;
	}

	void		AFPDFDoc::InvalidateBitmapCache(){
		
			for(int i=0;i<MAX_BITMAP_CACHE;i++){
				if(_bitmapCache[i]!=0){
					_bitmapCache[i]->Dispose();
					delete _bitmapCache[i];
				}
				_bitmapCache[i]=0;
				_pageCached[i]=0;
			}
		}

	void		AFPDFDoc::RemoveFromCache(int page){
		for(int i=0;i<MAX_BITMAP_CACHE;i++){
			if(_pageCached[i]==page){
				_bitmapCache[i]->Dispose();
				delete _bitmapCache[i];
				_bitmapCache[i]=0;
				return;
			}
		}
	}

	void		AFPDFDoc::AddBitmapCache(PageMemory *bmp, int page){
		//if exists and is not equal, delete
		for(int i=0;i<MAX_BITMAP_CACHE;i++){
			if(_pageCached[i]==page){
				if( _bitmapCache[i]!=0 && _bitmapCache[i]!=bmp){
					_bitmapCache[i]->Dispose();
					delete _bitmapCache[i];
					_bitmapCache[i]=bmp;
					return;
				}else{
					return;
				}
			}
		}
		//Add in an empty bin
		for(int i=0;i<MAX_BITMAP_CACHE;i++){
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
			_bitmapCache[_countCached]->Dispose();
			delete _bitmapCache[_countCached];
			_bitmapCache[_countCached]=0;
			_pageCached[_countCached]=0;
		}
		//Save in cache
		_bitmapCache[_countCached]=bmp;
		_pageCached[_countCached]=page;
	}


	
	//------AFPDFDoc
	AFPDFDoc::AFPDFDoc(char *configFile)
	: m_PDFDoc(NULL)
	, m_splashOut(NULL)
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
	, m_ViewX(0)
	, m_ViewY(0)
	, m_ViewWidth(0)
	, m_ViewHeight(0)
	, __x0(0)
	, __y0(0)
	, m_ExportProgressHandle(0)
	, m_exportJpgThread(0)
	, hExportJpgCancel(0)
	, _countCached(-1)
	, m_RenderFinishHandle(0)
	, m_PageRenderedByThread(0)
	, m_sliceBox(0,0,0,0)
	, m_LastOpenedStream(0)
	{
		
		// GMutex m;
		gInitMutex(&hgMutex);

		//hgMutex = CreateMutex(NULL,FALSE,NULL);
		hRenderFinished = CreateEvent(NULL,TRUE,FALSE,TEXT("CancellEvent"));

		for(int i=0;i<MAX_BITMAP_CACHE;i++){
			_bitmapCache[i]=0;
			_pageCached[i]=0;
		}

		InitGlobalParams(configFile);
		
		m_Bitmap=0;
		m_PDFDoc=0;
		m_splashOut=0;
		m_Outline=0;
		//Redirect
#ifdef  _DEBUG
		freopen( "C:\\stderr.log", "w", stderr );
#endif
	}

	AFPDFDoc::~AFPDFDoc()
	{
		
		this->Dispose();
		gDestroyMutex(&this->hgMutex);
		CloseHandle(hRenderFinished);
	}
	
	void AFPDFDoc::Dispose(){

		
		if(m_renderingThread){
			DWORD exitcode=0;
			GetExitCodeThread(m_renderingThread,&exitcode);
			if(exitcode==STILL_ACTIVE){
				::InterlockedExchange(&g_lLocker,1);
				::WaitForSingleObject(hRenderFinished,INFINITE);
				//TerminateThread(m_renderingThread,exitcode);
			}
			CloseHandle(m_renderingThread);
			m_renderingThread=0;
		}
		if(m_exportJpgThread){
			DWORD exitcode=0;
			GetExitCodeThread(m_renderingThread,&exitcode);
			if(exitcode==STILL_ACTIVE){
				::SetEvent(hExportJpgCancel);
				::WaitForSingleObject(hExportJpgFinished,INFINITE);
				//TerminateThread(m_exportJpgThread,exitcode);
			}
			CloseHandle(m_exportJpgThread);
			m_exportJpgThread=0;
		}

		if(m_LastOpenedStream)
		{
			m_LastOpenedStream->close();
			//Let PDFDoc to delete the stream
			//delete m_LastOpenedStream;
			m_LastOpenedStream=0;
		}
		
		InvalidateBitmapCache();
		m_Bitmap=0;

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

	
	PDFDoc *AFPDFDoc::createDoc(char *FileName){
		PDFDoc *pdfDoc;
		if(FileName==NULL)
			FileName=m_LastOpenedFile.getCString();
		if(m_LastOpenedFile.getLength()==0 && this->m_LastOpenedStream!=0)
			pdfDoc = new PDFDoc(m_LastOpenedStream);
		else
			pdfDoc = new PDFDoc(new GString(FileName), NULL,NULL);
		//Intentamos abrir el documento sin clave
		
		
		
		//Esperamos a que se carge correctamente, o que ocurra un error
		while (!pdfDoc->isOk()) 
		{
			//En caso de que este encriptado con clave
			if (pdfDoc->getErrorCode() == errEncrypted)
			{
				//Si no se especifico clave salimos
				if(m_OwnerPassword.GetLength()<=0 && m_UserPassword.GetLength()<=0) {
					delete pdfDoc;
					pdfDoc=NULL;
				}else{
					//Si no se especifico una de las claves, usamos la misma para ambos
					if(m_UserPassword.GetLength()<=0){
						m_UserPassword=m_OwnerPassword;
					}else if(m_OwnerPassword.GetLength()<=0){
						m_OwnerPassword = m_UserPassword;
					}				
					//Intentamos abrir con clave
					pdfDoc = new PDFDoc(new GString(FileName), 
							new GString(m_OwnerPassword.GetBuffer()), 
							new GString(m_UserPassword.GetBuffer()));
					m_OwnerPassword.ReleaseBuffer();
					m_UserPassword.ReleaseBuffer();

				}
			} 
			if (!pdfDoc->isOk())
			{
				//En caso de error, regresamos NULL
				delete pdfDoc;
				pdfDoc=NULL;
				//return PDFDoc->getErrorCode();
			}
			
		}

		return pdfDoc;
	}

	long AFPDFDoc::LoadFromStream(void *callback,long fullLenght, char *user_password, char *owner_password)
	{
		
		//Wait for previous threads and delete them
		if (m_renderingThread!=0)
		{					
			DWORD exitcode=0;
			GetExitCodeThread(m_renderingThread,&exitcode);
			if(exitcode==STILL_ACTIVE){
				::InterlockedExchange(&this->g_lLocker,1);
				WaitForSingleObject(this->hRenderFinished,INFINITE);
			}
			CloseHandle(m_renderingThread);
			m_renderingThread=NULL;
		}

		InvalidateBitmapCache();
		if(user_password!=NULL)
			m_UserPassword = user_password;
		if(owner_password!=NULL)
			m_OwnerPassword = owner_password;
		//Si la existia lo eliminamos
		if (m_splashOut!=NULL){
			delete m_splashOut;
			m_splashOut=0;
		}
		//Close the stream
		if(m_LastOpenedStream)
		{
			m_LastOpenedStream->close();
			m_LastOpenedStream=0;
		}
		//Si ya existia la eliminamos
		if (m_PDFDoc!=0){
			delete m_PDFDoc;
			m_PDFDoc=0;
		}

		//Intentamos abrir el documento sin clave		
		Object obj;
		obj.initNull();
		StreamCallback *str = new StreamCallback((READFROMSTREAM)callback,fullLenght,0,gFalse,0,&obj);
		m_PDFDoc = new PDFDoc(str); //,new GString(user_password),new GString(owner_password));
		m_LastOpenedStream=str;
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
					m_PDFDoc = new PDFDoc(str, 
							new GString(m_OwnerPassword.GetBuffer()), 
							new GString(m_UserPassword.GetBuffer()));
					m_OwnerPassword.ReleaseBuffer();
					m_UserPassword.ReleaseBuffer();
				}
			} 
			if (!m_PDFDoc->isOk())
			{
				//En caso de error, regresamos el codigo de error
				int errCode = m_PDFDoc->getErrorCode();
				fprintf(stderr,"Error File (%d)",errCode);
				delete m_PDFDoc;
				m_PDFDoc=NULL;
				return errCode;
			}
			
		}
		m_LastOpenedFile.clear(); //No se especifico nombre dearchivo, habra que usar el metodo MakeSubstream del stream para el momento de exportar jpgs
//		m_LastOpenedFile.insert((int)0,FileName,strlen(FileName));
		//El archivo se cargo correctamente
		m_Outline = m_PDFDoc->getOutline();

		m_LastPageRendered = -1;
		m_CurrentPage = 0;
		m_SearchPage = 0;
		
		return S_OK;
	}
	long AFPDFDoc::LoadFromFile(char *FileName, char *user_password, char *owner_password)
	{		
		
		//Wait for previous threads and delete them
		if (m_renderingThread!=0)
		{					
			DWORD exitcode=0;
			GetExitCodeThread(m_renderingThread,&exitcode);
			if(exitcode==STILL_ACTIVE){
				::InterlockedExchange(&this->g_lLocker,1);
				WaitForSingleObject(this->hRenderFinished,INFINITE);
			}
			CloseHandle(m_renderingThread);
			m_renderingThread=NULL;
		}

		logInfo("InvalidateBitmapCache");
		InvalidateBitmapCache();

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
		
		//Establecemos el color del papel
		/*SplashColor paperColor;
		paperColor[0] = 0xff;
		paperColor[1] = 0xff;
		paperColor[2] = 0xff;*/
		
		//logInfo("Create SplashOutputDev");
		//Note: the alignment is given by GDI requirements: bitmaps have to be 16-bit aligned.
		//m_splashOut = new SplashOutputDev(splashModeBGR8, 4, gFalse, paperColor,gTrue,globalParams->getAntialias());
		//m_splashOut->setVectorAntialias(globalParams->getVectorAntialias());
		
		logInfo("Create PDFDoc");
		//Intentamos abrir el documento sin clave
		m_PDFDoc = new PDFDoc(new GString(FileName), NULL,NULL);		
		//Esperamos a que se carge correctamente, o que ocurra un error
		while (!m_PDFDoc->isOk()) 
		{
			logInfo("m_PDFDoc->isOk");
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
					m_OwnerPassword.ReleaseBuffer();
					m_UserPassword.ReleaseBuffer();
				}
			} 
			if (!m_PDFDoc->isOk())
			{
				//En caso de error, regresamos el codigo de error
				int errCode = m_PDFDoc->getErrorCode();
				fprintf(stderr,"Error File (%d)",errCode);
				delete m_PDFDoc;
				m_PDFDoc=NULL;
				return errCode;
			}
			
		}
		if(m_LastOpenedStream){
			m_LastOpenedStream->close();
			delete m_LastOpenedStream;
			m_LastOpenedStream = 0;
		}
		m_LastOpenedFile.clear();
		m_LastOpenedFile.insert((int)0,FileName,strlen(FileName));
		//El archivo se cargo correctamente
		m_Outline = m_PDFDoc->getOutline();

		m_LastPageRendered = -1;
		m_CurrentPage = 0;
		m_SearchPage = 0;
		
		return S_OK;
	}


	long AFPDFDoc::RenderPageThread(long lhWnd, bool bForce)
	{
		if (m_PDFDoc != NULL) {

			if(m_splashOut!=0){
				//Heuristically check if we have enough memory ;-)
				double newbytes = (m_renderDPI/72.0)*(m_renderDPI/72.0) * m_PDFDoc->getPageCropWidth(m_CurrentPage)* m_PDFDoc->getPageCropHeight(m_CurrentPage); //new approx. number of pixels
				newbytes = newbytes * 3 + newbytes*2; //24-bit splashbitmap, 16-bit gdi-bitmap
				newbytes=(newbytes*1.2); //Safety area;
				newbytes -= m_splashOut->getBitmap()->getWidth()*m_splashOut->getBitmap()->getHeight()*3; //substract old 24-bit splash bitmap size
				void* testAllocation = malloc((int)2.0*newbytes);
				if (testAllocation==0 && newbytes>0){
					//We better dont zoom this far in!
					return errOutOfMemory+1;
				} else {
					free (testAllocation);
				}
			}
			try{
				//Wait for previous threads and delete them
				if (m_renderingThread!=0)
				{					
					//ReleaseMutex(hgMutex);
					DWORD exitcode;
					::GetExitCodeThread(m_renderingThread,&exitcode);
					if(exitcode==STILL_ACTIVE){
						::InterlockedExchange(&this->g_lLocker,1);
						WaitForSingleObject(this->hRenderFinished,INFINITE);
					}
					//TerminateThread(m_renderingThread,0);
					CloseHandle(m_renderingThread);
					SetEvent(this->hRenderFinished);
					m_renderingThread=NULL;
				}
				m_PageRenderedByThread=true;
				//Establecemos el color del papel
				SplashColor paperColor;
				paperColor[0] = 0xff;
				paperColor[1] = 0xff;
				paperColor[2] = 0xff;
				
				m_PageHeight =(long)( m_PageHeight * m_renderDPI/m_LastRenderDPI);
				m_PageWidth  =(long)( m_PageWidth * m_renderDPI/m_LastRenderDPI);
				m_Bitmap->Resize(m_PageWidth,m_PageHeight,m_renderDPI);

				//Run thread
				if (m_LastPageRenderedByThread != m_CurrentPage 
					|| m_LastRenderDPI!=m_renderDPI)
				{
					m_PageRenderedByThread=true;
					m_PageToRenderByThread = m_CurrentPage;
					
					if(m_splashOut>0){
						delete m_splashOut;
						m_splashOut=0;
					}
					threadParam *tp =new threadParam(this,m_PageToRenderByThread);
					//Note: the alignment is given by GDI requirements: bitmaps have to be 16-bit aligned.
					tp->out= new SplashOutputDev(splashModeBGR8, 4, gFalse, paperColor,gTrue,globalParams->getAntialias());
					tp->out->startDoc(m_PDFDoc->getXRef());
					tp->out->clearModRegion();
					tp->enablePreRender=true;
					
					m_splashOut=tp->out;
					ResetEvent(this->hRenderFinished);
					m_renderingThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AFPDFDoc::RenderingThread,(LPVOID)tp,CREATE_SUSPENDED,0);
					ResumeThread(m_renderingThread);
				}
			} catch(void *e){
				return errOutOfMemory;
			}
		}
		return 0;
	}

	int AFPDFDoc::RenderThreadFinished(SplashOutputDev *out, int page, bool enablePreRender)
	{
		//The process was cancelled!
		if(::InterlockedExchange(&this->g_lLocker,0)!=0)
		{
			SetEvent(this->hRenderFinished);
			return 0;
		}
		bool byThread=m_PageRenderedByThread;
		m_PageRenderedByThread=false;

		//Rendered bitmap by xpdf
		SplashBitmap * bitmap = out->getBitmap();
		int bmWidth = bitmap->getWidth();
		int bmHeight = bitmap->getHeight();
		
		BITMAPINFO bmi;
		ZeroMemory(&bmi,sizeof(bmi));
		bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth=bmWidth;
		//By default bitmaps are bottom up images which means 1st scanline is bottom when and last is top one.
		bmi.bmiHeader.biHeight=-bmHeight;
		bmi.bmiHeader.biPlanes=1;
		bmi.bmiHeader.biBitCount=24;
		bmi.bmiHeader.biCompression=BI_RGB;

		bmi.bmiColors[0].rgbBlue = 0;
		bmi.bmiColors[0].rgbGreen = 0;
		bmi.bmiColors[0].rgbRed = 0;
		bmi.bmiColors[0].rgbReserved = 0;

		HDC clientDC;
		clientDC = GetWindowDC(NULL);

		//Get from cache
		PageMemory *bmpMem = this->GetBitmapCache(page);
		//Check if valid, if not create and add to cache
		if(!bmpMem || (bmpMem && ( bmpMem->Width != bmWidth || bmpMem->Height != bmHeight)))
		{
			bmpMem = new PageMemory();
			bmpMem->Create(clientDC,bmWidth,bmHeight,m_renderDPI, out->getDefCTM(),out->getDefICTM());	
		}
		//********START DIB
		bmpMem->SetDimensions(bmWidth,bmHeight,m_renderDPI);
		bmpMem->SetDIBits(clientDC,(void *)out->getBitmap()->getDataPtr());
		//********END DIB
		AddBitmapCache(bmpMem,page);
		//If was called by render current, then update the current Bitmap, if not just save in cache	
		if(byThread){
			m_PageHeight = bmHeight;
			m_PageWidth  = bmWidth;
			//Update current bitmap
			m_Bitmap =bmpMem;

			//Update page box
			int box_left, box_top, box_right, box_bottom;
			out->getModRegion(&box_left, &box_top, &box_right, &box_bottom);
			m_bbox = CRect(box_left, box_top, box_right, box_bottom);

			m_LastRenderDPI = m_renderDPI;
			m_LastPageRendered=page;
		}
		bool newThreadRunned=false;
		//prerender next page
		if (page+1 <= m_PDFDoc->getNumPages() && enablePreRender && byThread && 0)
		{   
			m_PageToRenderByThread = page+1;
			
			//Establecemos el color del papel
			SplashColor paperColor;
			paperColor[0] = 0xff;
			paperColor[1] = 0xff;
			paperColor[2] = 0xff;

			//Delete splash
			if(m_splashOut>0){
				delete m_splashOut;
				m_splashOut=0;
			}
			
			threadParam *tp = new threadParam(this,m_PageToRenderByThread);
			tp->out= new SplashOutputDev(splashModeBGR8, 4, gFalse, paperColor,gTrue,globalParams->getAntialias());
			tp->out->startDoc(m_PDFDoc->getXRef());
			tp->out->clearModRegion();
			tp->pageToRender=m_PageToRenderByThread;
			tp->enablePreRender=false;//Disable Prerender next page
			newThreadRunned=true;

			m_splashOut=tp->out;
			ResetEvent(this->hRenderFinished);
			
			InterlockedExchange(&this->g_lLocker,0);
			m_renderingThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AFPDFDoc::RenderingThread,(LPVOID)tp,CREATE_SUSPENDED,0);
			SetThreadPriority(m_renderingThread,THREAD_PRIORITY_BELOW_NORMAL);
			ResumeThread(m_renderingThread);	
		}else{
			m_renderingThread=0;
		}
		
		if(this->m_RenderFinishHandle!=0 && byThread)
			this->m_RenderFinishHandle();
		
		/*if(this->m_RenderNotifyFinishHandle)
			this->m_RenderNotifyFinishHandle(page,byThread);*/
		
		if(!newThreadRunned)
			SetEvent(this->hRenderFinished);
		
		return 0;
	}
	long AFPDFDoc::RenderPage(long lhWnd)
	{
		return RenderPage(lhWnd,false);
	}
	long AFPDFDoc::RenderPage(long lhWnd, bool bForce)
	{
		return RenderPage(lhWnd,bForce,true);
	}

	long AFPDFDoc::RenderPage(long lhWnd, bool bForce, bool enableThread)
	{

		if (m_PDFDoc != NULL) {

			m_PageRenderedByThread=false;
			int bmWidth, bmHeight;
			
			//Establecemos el color del papel
			SplashColor paperColor;
			paperColor[0] = 0xff;
			paperColor[1] = 0xff;
			paperColor[2] = 0xff;

			if(m_splashOut!=0){
				//Heuristically check if we have enough memory ;-)
				double newbytes = (m_renderDPI/72.0)*(m_renderDPI/72.0) * m_PDFDoc->getPageCropWidth(m_CurrentPage)* m_PDFDoc->getPageCropHeight(m_CurrentPage); //new approx. number of pixels
				newbytes = newbytes * 3 + newbytes*2; //24-bit splashbitmap, 16-bit gdi-bitmap
				newbytes=(newbytes*1.2); //Safety area;
				newbytes -= m_splashOut->getBitmap()->getWidth()*m_splashOut->getBitmap()->getHeight()*3; //substract old 24-bit splash bitmap size
				void* testAllocation = malloc((size_t)(2*newbytes));
				if (testAllocation==0 && newbytes>0){
					//We better dont zoom this far in!
					return errOutOfMemory+1;
				} else {
					free (testAllocation);
				}
			}
			try{
				//Wait for previous threads and delete them
				if (m_renderingThread!=0)
				{					
					DWORD exitcode=0;
					logInfo("Running out last thread");
					GetExitCodeThread(m_renderingThread,&exitcode);
					if(exitcode==STILL_ACTIVE) //hurry up!
						SetThreadPriority(m_renderingThread,THREAD_PRIORITY_ABOVE_NORMAL);
					//Wait to finish
					logInfo("Wait to signaled procedure");
					WaitForSingleObject(this->hRenderFinished,INFINITE);
					logInfo("Closing Handler");
					CloseHandle(m_renderingThread);
					m_renderingThread=NULL;
				}
				logInfo("GetBitmapCache");
				//Read from cache
				m_Bitmap = GetBitmapCache(m_CurrentPage);
				//Check if the last bitmap is valid
				if ( bForce || (m_Bitmap && m_Bitmap->getRenderDPI() != m_renderDPI) ){
					m_Bitmap=0;
				}	

				if(m_Bitmap==0){	
					logInfo("Run thread");
					m_PageToRenderByThread = m_CurrentPage;

					if(m_splashOut>0){
						delete m_splashOut;
						m_splashOut=0;
					}
					threadParam *tp=new threadParam(this,m_PageToRenderByThread);
					tp->out= new SplashOutputDev(splashModeBGR8, 4, gFalse, paperColor,gTrue,globalParams->getAntialias());
					tp->out->setVectorAntialias(globalParams->getVectorAntialias());
					tp->out->startDoc(m_PDFDoc->getXRef());
					tp->out->clearModRegion();
					tp->enablePreRender=false;
					m_splashOut=tp->out;

					InterlockedExchange(&this->g_lLocker,0);
					//Render the page syncronized
					AFPDFDoc::RenderingThread((LPVOID)tp);
					//At this point the m_Bitmap now contains the full page
					delete tp;
					m_Bitmap=GetBitmapCache(m_CurrentPage);
					//Replaced by RenderThreadFinished
					
					/*
					logInfo("getBitmap()");
					//Rendered bitmap by xpdf
					SplashBitmap * bitmap = m_splashOut->getBitmap();
					bmWidth = bitmap->getWidth();
					bmHeight = bitmap->getHeight();
					m_PageHeight = bmHeight;
					m_PageWidth  = bmWidth;


					BITMAPINFO bmi;
					ZeroMemory(&bmi,sizeof(bmi));
					bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
					bmi.bmiHeader.biWidth=bmWidth;
					//By default bitmaps are bottom up images which means 1st scanline is bottom when and last is top one.
					bmi.bmiHeader.biHeight=-bmHeight;
					bmi.bmiHeader.biPlanes=1;
					bmi.bmiHeader.biBitCount=24;
					bmi.bmiHeader.biCompression=BI_RGB;

					bmi.bmiColors[0].rgbBlue = 0;
					bmi.bmiColors[0].rgbGreen = 0;
					bmi.bmiColors[0].rgbRed = 0;
					bmi.bmiColors[0].rgbReserved = 0;

					HDC clientDC;
					//clientDC.Attach(::GetWindowDC((HWND)lhWnd));
					clientDC = GetWindowDC((HWND)lhWnd);
					//Bitmap Rebuild
					logInfo("Bitmap Rebuild\n");
					if(!m_Bitmap || (m_Bitmap && ( m_Bitmap->Width != bmWidth || m_Bitmap->Height != bmHeight)))
					{
						if(m_Bitmap==0){
							logInfo("Add Bitmap cache\n");
							m_Bitmap = new PageMemory();
							AddBitmapCache(m_Bitmap,m_CurrentPage);
						}
						logInfo("Create Bitmap \n");
						m_Bitmap->Create(clientDC,bmWidth,bmHeight,m_renderDPI);		
					}
					//********START DIB
					logInfo("Set DIBits\n");
					m_Bitmap->SetDIBits(clientDC,(void *)m_splashOut->getBitmap()->getDataPtr());
					//********END DIB
					//clientDC.Detach();

					int box_left, box_top, box_right, box_bottom;
					m_splashOut->getModRegion(&box_left, &box_top, &box_right, &box_bottom);
					m_bbox = CRect(box_left, box_top, box_right, box_bottom);
					*/
				} 
				//Update Size
				if(m_Bitmap){
					m_PageWidth = m_Bitmap->Width;
					m_PageHeight = m_Bitmap->Height;
				}

				
				//prerender next page
				logInfo("prerender next page\n");
				if (enableThread && m_CurrentPage+1 <= m_PDFDoc->getNumPages())
				{
					m_PageToRenderByThread = m_CurrentPage+1;

					if(m_splashOut>0){
						delete m_splashOut;
						m_splashOut=0;
					}
					threadParam *tp = new threadParam(this, m_PageToRenderByThread);
					tp->out= new SplashOutputDev(splashModeBGR8, 4, gFalse, paperColor,gTrue,globalParams->getAntialias());
					tp->out->startDoc(m_PDFDoc->getXRef());
					tp->out->clearModRegion();
					tp->enablePreRender=false;
					m_splashOut=tp->out;
					ResetEvent(this->hRenderFinished);

					InterlockedExchange(&this->g_lLocker,0);
					m_renderingThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AFPDFDoc::RenderingThread,(LPVOID)tp,CREATE_SUSPENDED,0);
					SetThreadPriority(m_renderingThread,THREAD_PRIORITY_BELOW_NORMAL);
					logInfo("resume prerender next page thread\n");
					ResumeThread(m_renderingThread);
					
				}else
					m_renderingThread=0;
				
				m_LastRenderDPI = m_renderDPI;
				m_LastPageRendered=m_CurrentPage;

			} catch(void *e){
				return errOutOfMemory;
			}
		}
		return 0;
	}

	GBool AFPDFDoc::callbackAbortDisplay(void *data)
	{
		AFPDFDoc *doc =(AFPDFDoc *)data;
		
		if(::InterlockedExchange(&doc->g_lLocker,0)!=0){
			fprintf(stderr,"======================ABORTED--------------------\n");	
			::InterlockedExchange(&doc->g_lLocker,1);
			return gTrue;
		}
		return gFalse;
	}
	//Rendering thread
	UINT AFPDFDoc::RenderingThread( LPVOID value )
	{
			threadParam *param =(threadParam *)value;
			AFPDFDoc *pdfDoc =(AFPDFDoc *)param->doc;

			//Add a mutex?
			::gLockMutex(&pdfDoc->hgMutex);
			
			__try {
				double renderDPI =pdfDoc->m_renderDPI;
				int page = pdfDoc->m_PageToRenderByThread;
			
				//Verificar los valores de variables
				renderDPI=IFZERO(renderDPI,72);
				page=MAX(1,page);
				globalParams->setPrintCommands(gTrue);
				pdfDoc->m_LastPageRenderedByThread=page;
				if(pdfDoc->m_sliceBox.NotEmpty()){
					logInfo("displayPageSlice");
					pdfDoc->m_PDFDoc->displayPageSlice(param->out,page,
												renderDPI,renderDPI, pdfDoc->m_Rotation,
												gFalse,gTrue,gFalse,
												pdfDoc->m_sliceBox.left ,pdfDoc->m_sliceBox.top,
												pdfDoc->m_sliceBox.width,pdfDoc->m_sliceBox.height,callbackAbortDisplay,pdfDoc);					
				}else{
					if(pdfDoc->m_PDFDoc->getCatalog() && pdfDoc->m_PDFDoc->getCatalog()->isOk()){
						Page *p = pdfDoc->m_PDFDoc->getCatalog()->getPage(page);
						if(p && p->isOk()){
							logInfo("displayPage");
							fprintf(stderr,"IS OK %d,%d,%d",pdfDoc->m_PDFDoc?1:0,param->out?1:0,page);
							fflush(stderr);
							pdfDoc->m_PDFDoc->displayPage(param->out,page,
												renderDPI, renderDPI, pdfDoc->m_Rotation, 
												gFalse, gTrue, gFalse,callbackAbortDisplay,pdfDoc);
						}
					}
				}
			
			}__finally{
				pdfDoc->RenderThreadFinished(param->out,param->pageToRender, param->enablePreRender);
				::gUnlockMutex(&pdfDoc->hgMutex);
				return true;
			}
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


	//Render from x to x+w, y to y+h
	long AFPDFDoc::RenderHDC(long lHdc)
	{
		HDC dc=(HDC)lHdc;
		if (m_Bitmap != NULL) 
		{
			

			// Draw the rendered document
			m_Bitmap->Draw(
				(HDC)lHdc,
				m_ViewOffsetX,		//SrcX
				m_ViewOffsetY,		//SrcY
				m_ViewWidth,		//Width
				m_ViewHeight,		//Height
				m_ViewX,			//DestX
				m_ViewY);			//DestY
	
			/*
			Links *l =m_PDFDoc->getLinks(m_CurrentPage);

			if(l->getNumLinks()>0){
				HGDIOBJ draw_pen, old_pen;
				HGDIOBJ old_brush;

				draw_pen = CreatePen(PS_SOLID, 0, RGB(127, 127, 255));
				old_pen = SelectObject(dc,draw_pen);
				old_brush = SelectObject(dc,GetStockObject(NULL_BRUSH));

				double x1,y1,x2,y2;
				int left,top,right,bottom;
				for(int j=0; j < l->getNumLinks(); j++){
					Link *link = l->getLink(j);
					link->getRect(&x1,&y1,&x2,&y2);
					this->cvtUserToDev(x1,y1,&left,&top);
					this->cvtUserToDev(x2,y2,&right,&bottom);
					CRect r(left,top,right,bottom);
					r.OffsetRect(-m_ViewOffsetX, -m_ViewOffsetY);
					r.OffsetRect(m_ViewX, m_ViewY);
					Rectangle(dc,r.left,r.top,r.right,r.bottom);
				}
				SelectObject(dc,old_pen);
				SelectObject(dc,old_brush);
				DeleteObject(draw_pen);
			}
			delete l;*/

			// draw selection 
			if (!m_HideMarks && m_CurrentPage == m_SearchPage) {
				//dc.Attach((HDC)lHdc);

				const double mul = m_renderDPI / FIND_DPI;

				HGDIOBJ draw_pen, old_pen;
				HGDIOBJ old_brush;
				
				draw_pen = CreatePen(PS_SOLID, 0, RGB(127, 127, 255));
				old_pen = SelectObject(dc,draw_pen);
//				old_brush = SelectStockObject(dc,NULL_BRUSH);
				old_brush = SelectObject(dc,GetStockObject(NULL_BRUSH));
				
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
						r.OffsetRect(m_ViewX, m_ViewY);
						r.InflateRect(1,0);
						::Rectangle(dc,r.left,r.top,r.right,r.bottom);
						//Rectangle(r);
						r.DeflateRect(1,1);
						::InvertRect(dc,&r);
						//dc.InvertRect(r);
					}
				}

				SelectObject(dc,old_pen);
				SelectObject(dc,old_brush);
				//dc.Detach();
			}	
		}
		return 0;
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
		return 0;
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
/*
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
			}*/
			m_splashOut->cvtUserToDev(0, dest->getTop(), &dx, &dy);
			return (long)dy;
		}
		return 0;
	}

	float AFPDFDoc::GetRenderDPI(void)
	{		
		return (float)m_renderDPI;
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

	long AFPDFDoc::FindText(const wchar_t *sText, long iPage, long SearchOrder, bool bCaseSensitive, bool bBackward, bool bMarkAll, bool bWholeDoc)
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
		ucstring = GetUnicodeString(sText, length);
		if (ucstring == NULL) {
//			AfxMessageBox("Out of memory");
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
//			AfxMessageBox("Error al mostrar la pagina");
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
//				AfxMessageBox("Error al mostrar la pagina");
				delete [] ucstring;
				return -1;	//No hay resultados
			}
		}
		//Establecer la pagina de busqueda en la primer pagina
		m_SearchPage=iFirstPageFound;
		delete [] ucstring;
		return m_Selection.GetCount();
	}
	
	long AFPDFDoc::FindNext(const wchar_t *sText)
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
		ucstring = GetUnicodeString(sText, length);
		if (ucstring == NULL) {
//			AfxMessageBox("Out of memory");
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
//			AfxMessageBox("Error al mostrar la pagina");
			delete [] ucstring;
			return -1;
		}

		//Mientras haya que hacer
		while(true) {
			if(m_searchWholeWord){
					//Buscar el texto
				rc = FindPage.findTextWholeWord(ucstring, length,
					startAtTop, gTrue, startAtLast, gFalse,
					m_bCaseSensitive, gFalse,
					&x0, &y0, &x1, &y1);
			}else{

				//Buscar el texto
				rc = FindPage.findText(ucstring, length,
					startAtTop, gTrue, startAtLast, gFalse,
					m_bCaseSensitive, gFalse,
					&x0, &y0, &x1, &y1);
			}

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
//					AfxMessageBox("Error al mostrar la pagina");
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

	long AFPDFDoc::FindPrior(const  wchar_t *sText)
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
		ucstring = GetUnicodeString(sText, length);
		if (ucstring == NULL) {
//			AfxMessageBox("Out of memory");
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
//			AfxMessageBox("Error al mostrar la pagina");
			delete [] ucstring;
			return -1;
		}

		//Mientras haya que hacer
		while(true) {
			if(m_searchWholeWord){
				//Buscar el texto
				rc = FindPage.findTextWholeWord(ucstring, length,
					startAtTop, gTrue, startAtLast, gFalse,
					m_bCaseSensitive,gTrue,
					&x0, &y0, &x1, &y1);
			}else{
				//Buscar el texto
				rc = FindPage.findText(ucstring, length,
					startAtTop, gTrue, startAtLast, gFalse,
					m_bCaseSensitive,gTrue,
					&x0, &y0, &x1, &y1);
			}

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
//					AfxMessageBox("Error al mostrar la pagina");
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

	long AFPDFDoc::FindFirst(const wchar_t *sText, long SearchOrder, bool Backward, bool WholeWord)
	{
		

		double x0, y0, x1, y1;
		x0=y0=x1=y1=0;
		long iFirstPageFound=0;
		long searchPage=0;
		GBool rc, startAtTop, startAtLast, backward;
		Unicode * ucstring;
		TextOutputDev FindPage(NULL, gTrue, gFalse, gFalse);

		CString theString(sText);
		int length = theString.GetLength();
		
		//Tratar de reservar la cadena
		ucstring = GetUnicodeString(sText, length);
		if (ucstring == NULL) {
//			AfxMessageBox("Out of memory");
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

		if(searchPage>this->GetPageCount())
			searchPage = this->GetPageCount();
		if(searchPage<1)
			searchPage=1;

		/* use fixed DPI and rotation, this would lead to somehwere
		 * wrong positioning displaying more than 300 DPI document
		 * don't think it's a problem thought
		 */
		m_PDFDoc->displayPage(&FindPage, searchPage,
			FIND_DPI, FIND_DPI, 0, gFalse, gTrue, gFalse);

		if (!FindPage.isOk()) {
//			AfxMessageBox("Error al mostrar la pagina");
			delete [] ucstring;
			return -1;
		}
		m_searchWholeWord = WholeWord;
		//Mientras haya que hacer
		while(true) {
			
			if(WholeWord){
				//Make a whole word search!
				rc = FindPage.findTextWholeWord(ucstring, length,
					startAtTop, gTrue, startAtLast, gFalse,
					(m_bCaseSensitive?gTrue:gFalse), backward,
					&x0, &y0, &x1, &y1);
			}else{
				//Buscar el texto
				rc = FindPage.findText(ucstring, length,
					startAtTop, gTrue, startAtLast, gFalse,
					(m_bCaseSensitive?gTrue:gFalse), backward,
					&x0, &y0, &x1, &y1);
			}
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
//				AfxMessageBox("Error al mostrar la pagina");
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
	
	wchar_t * AFPDFDoc::getTitle(){
		return ::getDocInfo("Title",m_PDFDoc);
	}

	wchar_t * AFPDFDoc::getAuthor(){
		return ::getDocInfo("Author",m_PDFDoc);
	}
		
	wchar_t * AFPDFDoc::getSubject(){
		return ::getDocInfo("Subject",m_PDFDoc);
	}
	wchar_t * AFPDFDoc::getKeywords(){
		return ::getDocInfo("Keywords",m_PDFDoc);
	}
	wchar_t * AFPDFDoc::getCreator(){
		return ::getDocInfo("Creator",m_PDFDoc);
	}
	wchar_t * AFPDFDoc::getProducer(){
		return ::getDocInfo("Producer",m_PDFDoc);
	}
	char * AFPDFDoc::getCreationDate(){
		wchar_t * s = getDocInfo("CreationDate",m_PDFDoc);
		char *datetime = new char[256];
		USES_CONVERSION;
		parseDateTime(datetime,W2A(s));
		return datetime;
	}
	char * AFPDFDoc::getLastModifiedDate(){
		wchar_t *s = getDocInfo("LastModifiedDate",m_PDFDoc);
		char *datetime=new char[256];
		USES_CONVERSION;
		parseDateTime(datetime,W2A(s));
		return datetime;
	}

	Links *AFPDFDoc::GetLinksPage(long iPage)
	{
		Catalog *c = m_PDFDoc->getCatalog();
		if(c){
			Links *l=m_PDFDoc->getLinks(iPage);
			return l;
		}
		return 0;
	}

	void AFPDFDoc::cvtUserToDev(double ux, double uy, int *dx, int *dy){
		if(m_Bitmap)
			m_Bitmap->cvtUserToDev(ux,uy,dx,dy);
		else if(m_splashOut)
			m_splashOut->cvtUserToDev(ux,uy,dx,dy);	
		else{
			*dx=ux;
			*dy=uy;
		}
	}

	void AFPDFDoc::cvtDevToUser(double ux, double uy, double *dx, double *dy){
		if(m_Bitmap)
			m_Bitmap->cvtDevTouser(ux,uy,dx,dy);
		else if(m_splashOut)
			m_splashOut->cvtDevToUser(ux,uy,dx,dy);
		else{
			*dx=ux;
			*dy=uy;
		}

	}

	int AFPDFDoc::SaveJpg(char *fileName,float renderDPI,int fromPage, int toPage, int quality, int waitProc)
	{
		CString errmsg;
		char filenameBuff[1024];
		DWORD waitRes=0;
		//If the current rendered page is the same requested
		if(renderDPI ==  this->m_renderDPI
			&& fromPage == this->m_LastPageRenderedByThread
			&& toPage == fromPage){

			if (m_renderingThread!=0)
			{
				DWORD exitcode=0;
				//hurry up!
				
				GetExitCodeThread(m_renderingThread,&exitcode);
				if(exitcode==STILL_ACTIVE)
					SetThreadPriority(m_renderingThread,THREAD_PRIORITY_ABOVE_NORMAL);
				while (exitcode==STILL_ACTIVE){
					
					GetExitCodeThread(m_renderingThread,&exitcode);
					Sleep(50);
				}
				CloseHandle(m_renderingThread);
				m_renderingThread=NULL;
			}
			SplashColor paperColor;
			paperColor[0] = 0xff;
			paperColor[1] = 0xff;
			paperColor[2] = 0xff;

			BITMAPINFO bmi;
			ZeroMemory(&bmi,sizeof(bmi));
			bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = m_splashOut->getBitmap()->getWidth();
			bmi.bmiHeader.biHeight = m_splashOut->getBitmap()->getHeight();
			bmi.bmiHeader.biPlanes=1;
			bmi.bmiHeader.biBitCount=24;
			bmi.bmiHeader.biCompression=BI_RGB;

			bmi.bmiColors[0].rgbBlue = 0;
			bmi.bmiColors[0].rgbGreen = 0;
			bmi.bmiColors[0].rgbRed = 0;
			bmi.bmiColors[0].rgbReserved = 0;
			
			int ret = JpegFromDib((HANDLE)m_splashOut->getBitmap()->getDataPtr(),&bmi,quality,CString(fileName),&errmsg);
			if(this->m_ExportProgressHandle != 0){
				if(this->m_ExportProgressHandle(1,1) !=0){
				}
			}

			return ret;
			
		}else{
			if(m_exportJpgThread!=0){
				DWORD exitcode=0;
				GetExitCodeThread(m_exportJpgThread,&exitcode);
				if (exitcode==STILL_ACTIVE){	
					//Cancel event
					SetEvent(this->hExportJpgCancel);
					//Wait for finished
					WaitForSingleObject(this->hExportJpgFinished,INFINITE);
				}
				CloseHandle(m_exportJpgThread);
				m_exportJpgThread=0;

			}
			
			if(this->hExportJpgCancel ==0){
				this->hExportJpgCancel = CreateEvent(NULL,TRUE,FALSE,TEXT("CancellEvent"));
				this->hExportJpgCancelled = CreateEvent(NULL,TRUE,FALSE,TEXT("CancelledEvent"));
				this->hExportJpgFinished = CreateEvent(NULL,TRUE,FALSE,TEXT("FinishedEvent"));
			}
			ResetEvent(this->hExportJpgCancel);
			ResetEvent(this->hExportJpgCancelled);
			ResetEvent(this->hExportJpgFinished);
			ExportParams *p=new ExportParams;
			p->_this = this;
			p->fileName =new char[strlen(fileName)+1];
			strcpy(p->fileName,fileName);
			p->rotation=m_Rotation;
			p->renderDPI = renderDPI;
			p->fromPage = fromPage;
			p->toPage = toPage;
			p->quality =quality;
			p->WaitTime = waitProc;
			DWORD dThread;
			m_exportJpgThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AFPDFDoc::ExportingJpgThread,(LPVOID)p,THREAD_PRIORITY_NORMAL,&dThread);

//			m_exportJpgThread->m_bAutoDelete=false;
//			m_exportJpgThread->ResumeThread();
			if(waitProc==-1){
				::WaitForSingleObject(this->hExportJpgFinished,INFINITE);
			}else if(waitProc>0){
				::WaitForSingleObject(this->hExportJpgFinished,waitProc);
			}

			return waitRes;
		}
	}

	void AFPDFDoc::CancelJpgSave(){
		SetEvent(this->hExportJpgCancel);
	}
	UINT AFPDFDoc::ExportingJpgThread( LPVOID param )
	{
		ExportParams *exp = (ExportParams *)param;
		PDFDoc *doc;
		
		doc = exp->_this->createDoc(NULL);
		
		SplashOutputDev	*splashOut=NULL;
		int eError=0;
		CString errMessage;
		CString fileName(exp->fileName);
		char *filenameBuffer[1024];

		if(doc!=NULL){
			SplashColor paperColor;
			paperColor[0] = 0xff;
			paperColor[1] = 0xff;
			paperColor[2] = 0xff;

			BITMAPINFO bmi;
			ZeroMemory(&bmi,sizeof(bmi));
			bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			
			bmi.bmiHeader.biPlanes=1;
			bmi.bmiHeader.biBitCount=24;
			bmi.bmiHeader.biCompression=BI_RGB;

			bmi.bmiColors[0].rgbBlue = 0;
			bmi.bmiColors[0].rgbGreen = 0;
			bmi.bmiColors[0].rgbRed = 0;
			bmi.bmiColors[0].rgbReserved = 0;
			

			splashOut = new SplashOutputDev(splashModeBGR8, 4, gFalse, paperColor,gTrue,globalParams->getAntialias());
			splashOut->setVectorAntialias(globalParams->getVectorAntialias());
			splashOut->startDoc(doc->getXRef());
		
			for(int page=exp->fromPage;page<=exp->toPage;page++){
				DWORD waitRes = WaitForSingleObject(exp->_this->hExportJpgCancel,0);
				if(waitRes ==  WAIT_OBJECT_0){
					SetEvent(exp->_this->hExportJpgCancelled);
					eError=1;
					break;//Process canceled
				}else{
					if(exp->_this->m_ExportProgressHandle != 0){
						if(!exp->_this->m_ExportProgressHandle(exp->toPage-exp->fromPage+1,page-exp->fromPage+1) !=0){
							SetEvent(exp->_this->hExportJpgCancelled);
							eError=4;
							break;
						}
					}

					doc->displayPage(splashOut,page, 
							exp->renderDPI, exp->renderDPI, exp->rotation, 
							gFalse, gTrue, gFalse,0,0);
			
					bmi.bmiHeader.biWidth = splashOut->getBitmap()->getWidth();
					bmi.bmiHeader.biHeight = splashOut->getBitmap()->getHeight();

					sprintf((char *)filenameBuffer,exp->fileName,page);
					if(JpegFromDib((HANDLE)splashOut->getBitmap()->getDataPtr(),&bmi,exp->quality,CString((char *)filenameBuffer),&errMessage)!=0){
						eError=3;
						break;
					}
					
				}
			}
		}else{
			eError=2;
		}
		if(doc!=NULL)
			delete doc;
		if(splashOut!=NULL)
			delete splashOut;
		SetEvent(exp->_this->hExportJpgFinished); 
		if(exp->_this->m_ExportFinishHandle !=0)
			exp->_this->m_ExportFinishHandle();
		return eError;
	}

	/*int AFPDFDoc::SaveHtml(char *outFileName, int firstPage, int lastPage, bool noFrames, bool nomerge, bool complexmode)
	{
		GString *docTitle = NULL;
		GString *author = NULL, *keywords = NULL, *subject = NULL, *date = NULL;
		GString *htmlFileName = NULL;
		GString *psFileName = NULL;
		HtmlOutputDev *htmlOut = NULL;
		PSOutputDev *psOut = NULL;
		GBool ok;
		char *p;
		char extension[16] = "png";
		Object info;
		char * extsList[] = {"png", "jpeg", "bmp", "pcx", "tiff", "pbm", NULL};
		char gsDevice[33] = "png16m";

		GBool stout;
		GBool xml=gFalse;
		GBool noframes = noFrames?gTrue:gFalse;
		GBool complexMode = complexmode?gTrue:gFalse;
		GBool noMerge=nomerge?gTrue:gFalse;
		GBool rawOrder;
		int scale=this->m_renderDPI/72;
		scale=1.5;
		
		// check for copy permission
		if (!m_PDFDoc->okToCopy()) {
			error(-1, "Copying of text from this document is not allowed.");
			goto error;
		}

		// construct text file name
		
		GString* tmp = new GString(outFileName);
		p=tmp->getCString()+tmp->getLength()-5;
		if (!xml)
			if (!strcmp(p, ".html") || !strcmp(p, ".HTML"))
				htmlFileName = new GString(tmp->getCString(),tmp->getLength() - 5);
			else 
				htmlFileName =new GString(tmp);
		else   
			if (!strcmp(p, ".xml") || !strcmp(p, ".XML"))
				htmlFileName = new GString(tmp->getCString(), tmp->getLength() - 5);
			else 
				htmlFileName =new GString(tmp);

		delete tmp;

		if (scale>3.0) scale=3.0;
		if (scale<0.5) scale=0.5;


		if (complexMode) {
			//noframes=gFalse;
			stout=gFalse;
		} 

		if (stout) {
			noframes=gTrue;
			complexMode=gFalse;
		}

		if (xml)
		{ 
			complexMode = gTrue;
			noframes = gTrue;
			noMerge = gTrue;
		}

		// get page range
		if (firstPage < 1)
			firstPage = 1;
		if (lastPage < 1 || lastPage > m_PDFDoc->getNumPages())
			lastPage = m_PDFDoc->getNumPages();

		m_PDFDoc->getDocInfo(&info);
		if (info.isDict()) {
			docTitle = getInfoString(info.getDict(), "Title");
			author = getInfoString(info.getDict(), "Author");
			keywords = getInfoString(info.getDict(), "Keywords");
			subject = getInfoString(info.getDict(), "Subject");
			date = getInfoDate(info.getDict(), "ModDate");
			if( !date )
				date = getInfoDate(info.getDict(), "CreationDate");
		}
		info.free();
		if( !docTitle || docTitle->getLength()==0 ) 
			docTitle = new GString(htmlFileName);
		
		// determine extensions of output background images 
		{int i;
		for(i = 0; extsList[i]; i++)
		{
			if( strstr(gsDevice, extsList[i]) != (char *) NULL )
			{
				strncpy(extension, extsList[i], sizeof(extension));
				break;
			}
		}}

		rawOrder = complexMode; // todo: figure out what exactly rawOrder do :)
		
		// write text file
		htmlOut = new HtmlOutputDev(htmlFileName->getCString(), 
			docTitle->getCString(), 
			author ? author->getCString() : NULL,
			keywords ? keywords->getCString() : NULL, 
			subject ? subject->getCString() : NULL, 
			date ? date->getCString() : NULL,
			extension,
			rawOrder, 
			firstPage,
			m_PDFDoc->getCatalog()->getOutline()->isDict());
		
		
		if( author )
		{   
			delete author;
		}
		if( keywords )
		{
			delete keywords;
		}
		if( subject )
		{
			delete subject;
		}
		if( date )
		{
			delete date;
		}
		if (htmlOut->isOk())
		{
			m_PDFDoc->displayPages(htmlOut, firstPage, lastPage, static_cast<int>(72*scale), static_cast<int>(72*scale), 0, gFalse, gTrue,gFalse);
			return 0;
			if (!xml)
			{
				htmlOut->dumpDocOutline(m_PDFDoc->getCatalog());
			}
		}

	  
		delete htmlOut;

		// clean up
		error:
		
		if(htmlFileName) 
			delete htmlFileName;
		HtmlFont::clear();

		// check for memory leaks
		Object::memCheck(stderr);
		gMemReport(stderr);

		return 0;
	}*/
	int AFPDFDoc::SaveTxt(char *fileName,int firstPage, int lastPage, bool htmlMeta, bool physLayout, bool rawOrder){
		  TextOutputDev *textOut;
		  FILE *f;
		  Object info;
//		  GBool ok;
		  GString *textFileName = new GString(fileName);
		 // check for copy permission
		  if (!m_PDFDoc->okToCopy()) {
//			error(-1, "Copying of text from this document is not allowed.");
			return 10003;
		  }
			UnicodeMap *uMap;
			if (!(uMap = globalParams->getTextEncoding())) {
				return 10004;
			}

		  // write HTML header
		  if (htmlMeta) {
			if (!textFileName->cmp("-")) {
			  f = stdout;
			} else {
			  if (!(f = fopen(textFileName->getCString(), "wb"))) {
//			error(-1, "Couldn't open text file '%s'", textFileName->getCString());
			return 20002;
			  }
			}
			fputs("<html>\n", f);
			fputs("<head>\n", f);
			m_PDFDoc->getDocInfo(&info);
			if (info.isDict()) {
			  printInfoString(f, info.getDict(), "Title", "<title>", "</title>\n",
					  uMap);
			  printInfoString(f, info.getDict(), "Subject",
					  "<meta name=\"Subject\" content=\"", "\">\n", uMap);
			  printInfoString(f, info.getDict(), "Keywords",
					  "<meta name=\"Keywords\" content=\"", "\">\n", uMap);
			  printInfoString(f, info.getDict(), "Author",
					  "<meta name=\"Author\" content=\"", "\">\n", uMap);
			  printInfoString(f, info.getDict(), "Creator",
					  "<meta name=\"Creator\" content=\"", "\">\n", uMap);
			  printInfoString(f, info.getDict(), "Producer",
					  "<meta name=\"Producer\" content=\"", "\">\n", uMap);
			  printInfoDate(f, info.getDict(), "CreationDate",
					"<meta name=\"CreationDate\" content=\"%s\">\n");
			  printInfoDate(f, info.getDict(), "LastModifiedDate",
					"<meta name=\"ModDate\" content=\"%s\">\n");
			}
			info.free();
/*
			fputs("<ul>\n",f);

			USES_CONVERSION;
			for(int itOutline=0;itOutline <  m_PDFDoc->getOutline()->getItems()->getLength();itOutline++){
				OutlineItem *ol = (OutlineItem *) m_PDFDoc->getOutline()->getItems()->get(itOutline);
				wchar_t *title =GetTitle(uMap,ol);
				fprintf(f,"<li>%s</li>\n",W2A(title));
			}
			fputs("<\ul>\n",f);
*/
			fputs("</head>\n", f);
			fputs("<body>\n", f);
			fputs("<pre>\n", f);
			if (f != stdout) {
			  fclose(f);
			}
		  }else{
				/*if (!(f = fopen(textFileName->getCString(), "wb"))) {
					return 20002;
				}
				
				USES_CONVERSION;
				for(int itOutline=0;itOutline <  m_PDFDoc->getOutline()->getItems()->getLength();itOutline++)
				{
					OutlineItem *ol = (OutlineItem *) m_PDFDoc->getOutline()->getItems()->get(itOutline);
					wchar_t *title =GetTitle(uMap,ol);
					fprintf(f,"%s\n",W2A(title));					
				}
				
				fclose(f);
*/
		  }

		  // write text file
		  textOut = new TextOutputDev(textFileName->getCString(),
			  physLayout?1:0, rawOrder?1:0, htmlMeta?1:0);
		  if (textOut->isOk()) {
			m_PDFDoc->displayPages(textOut, firstPage, lastPage, 72, 72, 0,
					  gFalse, gTrue, gFalse);
		  } else {
			delete textOut;
			return 2;
		  }
		  delete textOut;

		  // write end of HTML file
		  if (htmlMeta) {
			if (!textFileName->cmp("-")) {
			  f = stdout;
			} else {
			  if (!(f = fopen(textFileName->getCString(), "ab"))) {
			//error(-1, "Couldn't open text file '%s'", textFileName->getCString());
			return 20002;
			  }
			}
			fputs("</pre>\n", f);
			fputs("</body>\n", f);
			fputs("</html>\n", f);
			if (f != stdout) {
			  fclose(f);
			}
		  }

		  return 0;
	}

	LinkDest *AFPDFDoc::findDest(char *destName){
		GString s(destName);
		return this->m_PDFDoc->getCatalog()->findDest(&s);
	}


	//Returns true if exists a background thread of jpg export is running
	bool AFPDFDoc::JpgIsBusy(){
		if(m_exportJpgThread!=0){
			DWORD exitcode=0;
			GetExitCodeThread(m_exportJpgThread,&exitcode);
			if (exitcode==STILL_ACTIVE){	
				return true;
			}
		}
		return false;
	}

	//Returns true if a background thread is rendering next page
	bool AFPDFDoc::IsBusy(){
		if(this->m_renderingThread!=0){
			DWORD exitcode=0;
			GetExitCodeThread(m_renderingThread,&exitcode);
			if (exitcode==STILL_ACTIVE){	
				return true;
			}
		}
		return false;
	}

	void AFPDFDoc::SetSliceBox(int x, int y, int w, int h){
		m_sliceBox.left=x;
		m_sliceBox.right = x+w;
		m_sliceBox.bottom = y+h;
		m_sliceBox.top=x;
		m_sliceBox.width=w;
		m_sliceBox.height=h;
	}