#pragma once
#define			MAX_BITMAP_CACHE	16


#include "stdafx.h"
#include "globals.h"
#include "OutlineItemA.h"
#include "PageMemory.h"
#include "queue.h"
#include "CRect.h"

void InitGlobalParams(char *configFile);
typedef int (__stdcall *NOTIFYHANDLE)();
typedef int (__stdcall *PAGERENDERNOTIFY)(int,bool);
typedef int (__stdcall *PROGRESSHANDLE)(int, int);
typedef void (__stdcall *OUTPUTFUNCTIONB)(unsigned char *,int);
typedef void (__stdcall *OUTPUTFUNCTION)(wchar_t *,int);

void OutputToDelegate(void *stream, char *str, int len);

class CPDFSearchResult 
	: public CRect
{
public:
	CPDFSearchResult();
	CPDFSearchResult(CRect r,long lPage);
	long PageFound;
};

// AFPDFDoc command target
 class AFPDFDoc 
{
private:
	PageMemory	*_bitmapCache[MAX_BITMAP_CACHE+1];
	int			 _pageCached[MAX_BITMAP_CACHE+1];
	int			 _countCached;
	bool		m_searchWholeWord;
	bool		_needNonText;
	PageMemory *GetBitmapCache(int page);
	void InvalidateBitmapCache();
	void RemoveFromCache(int page);
	void AddBitmapCache(PageMemory *bmp, int page);
	int RenderThreadFinished(SplashOutputDev *out,int page, bool enablePreRender);
protected:
	long m_PageToRenderByThread;
	long m_LastPageRenderedByThread;
	volatile LONG g_lLocker;
	static UINT RenderingThread( LPVOID param );
	static UINT RenderingThreadThumb( LPVOID param );
	static GBool callbackAbortDisplay(void *data);
	static UINT ExportingJpgThread( LPVOID param );
	bool m_PageRenderedByThread;
	bool getNeedNonText();
	void setNeedNonText(bool needs);
	HANDLE hExportJpgCancel;
	HANDLE hRenderFinished;
	HANDLE hExportJpgCancelled;
	HANDLE hExportJpgFinished;
	CRITICAL_SECTION hgMutex;
	PDFDoc *createDoc(char *FileName);
public:
	PROGRESSHANDLE m_ExportProgressHandle;
	NOTIFYHANDLE m_ExportFinishHandle;
	NOTIFYHANDLE m_RenderFinishHandle;
	PAGERENDERNOTIFY m_RenderNotifyFinishHandle;
private:
	GString m_LastOpenedFile;
	BaseStream *m_LastOpenedStream;
	HANDLE m_renderingThread;
	HANDLE m_exportJpgThread;
	HANDLE m_renderThumbs;
	Queue m_QueuedThumbs;
	DynArray<CPDFSearchResult> m_Selection;
	PDFDoc *m_PDFDoc;
	SplashOutputDev *m_thumbOut;
	Links *_pageLinks;

	SplashOutputDev	*m_splashOut;
	//SplashOutputDev	*m_splashOutThread;
	Outline *m_Outline;
	PageMemory *m_Bitmap;
	CString m_OwnerPassword;
	CString m_UserPassword;
	CRect m_bbox;
	CRect m_sliceBox;
	double __x0;
	double __y0;

	//Client Bounds
	long m_ViewWidth;
	long m_ViewHeight;
	long m_ViewX;
	long m_ViewY;
	//Offset view
	long m_ViewOffsetX;
	long m_ViewOffsetY;

	long m_CurrentPage;
	long m_LastPageRendered;
	long m_SearchPage;
	long m_PageHeight;
	long m_PageWidth;
	int m_Rotation;
	double m_LastRenderDPI;
	double m_renderDPI;
	bool m_HideMarks;
	bool m_bCaseSensitive;
	bool m_SearchStarted;

public:
	AFPDFDoc(char *configFile);
	virtual ~AFPDFDoc();
	PDFDoc *getDoc(){
		return m_PDFDoc;
	}
	long DrawPage(int page,long hdc, int width, int height, double dpi,bool bThread, void *callback, bool bAntialising);
	bool ThumbInQueue(int page);
	long LoadFromStream(void *callback,long fullLenght, char *user_password, char *owner_password);
	long LoadFromFile(char *FileName, char *user_password, char *owner_password);
	long LoadFromFile(char *FileName, char *user_password);
	long LoadFromFile(char *sFileName);
	void SetUserPassword(char *user_password);
	void SetOwnerPassword(char *owner_password);
	void SetSliceBox(int x, int y, int w, int h);
	long RenderPage(long lhWnd);
	long RenderPage(long lhWnd, bool bForce);
	long RenderPage(long lhWnd, bool bForce, bool enableThread);
	long RenderPageThread(long lhWnd, bool bForce);
	long GetCurrentPage(void);
	void SetCurrentPage(long newVal);
	long GetCurrentX(void);
	void SetCurrentX(long newVal);
	long GetCurrentY(void);
	void SetCurrentY(long newVal);
	
	long GetViewX(){ return m_ViewX; }
	void SetViewX(long newVal){ m_ViewX =newVal; }
	long GetViewY(){ return m_ViewY; }
	void SetViewY(long newVal){ m_ViewY =newVal; }

	long GetViewWidth() { return m_ViewWidth; }
	void SetViewWidth(long newVal) { m_ViewWidth =newVal; }
	long GetViewHeight() { return m_ViewHeight; }
	void SetViewHeight(long newVal) { m_ViewHeight=newVal; }

	int SaveJpg(char *fileName,float renderDPI,int fromPage, int toPage, int quality, int waitProc);
	int SaveTxt(char *fileName,int firstPage, int lastPage, bool htmlMeta,bool physLayout, bool rawOrder);
	//int SaveHtml(char *outFileName, int firstPage, int lastPage, bool noFrames, bool nomerge, bool complexmode);

	void CancelJpgSave();
	//Returns true if exists a background thread of jpg export is running
	bool JpgIsBusy();

	//Returns true if a background thread is rendering next page
	bool IsBusy();

	//void WaitForFinish();
	bool IsEncrypted(){
		return m_PDFDoc->isEncrypted()?true:false;
	}
	
	long LoadFromFile2(char * FileName);
	long GetPageCount(void);
	long NextPage(void);
	long PreviousPage(void);
	long GotoPage(long PageNumber);
	long ZoomIN(void);
	long ZoomOut(void);
	long FitScreenWidth(long lhWnd);
	long FitScreenHeight(long lhWnd);
	long GetPageWidth(void);
	long GetPageHeight(void);
	OutlineItemA* GetOutline(long iOutline);
	long GetOutlineCount(void);
	long GetOutlinePtr(long iOutline);
	long ProcessLinkAction(long lPtrLinkAction);
	float GetRenderDPI(void);
	void SetRenderDPI(FLOAT newVal);
	wchar_t *GetPDFVersion(void);
	long FindText(const wchar_t *sText, long iPage, long SearchOrder, bool bCaseSensitive, bool bBackward, bool bMarkAll, bool bWholeDoc, bool bWholeWord, bool stopOnFirstPageResults);
	long FindNext(const wchar_t *sText);
	long FindPrior(const wchar_t *sText);
	long FindFirst(const wchar_t *sText, long SearchOrder, bool Backward, bool WholeWord);
	long RenderHDC(long lHdc);
	Links *GetLinksPage(long iPage);
	LinkDest *findDest(char *destName);
	void cvtUserToDev(double ux, double uy, int *dx, int *dy);
	void cvtDevToUser(double ux, double uy, double *dx, double *dy);
	long GetSearchPage(void);
	long SetSearchPage(long lNewValue);
	bool GetSearchCaseSensitive(void);
	void SetSearchCaseSensitive(bool newVal);
	long PrintToFile(LPCTSTR fileName, int fromPage, int toPage);
	CPDFSearchResult *GetSearchResults();
	long GetSearchCount();
	int findpage(int num, int gen){ return m_PDFDoc->findPage(num,gen);	}
	void Dispose();

	wchar_t * getTitle();
	wchar_t * getAuthor();
	wchar_t * getSubject();
	wchar_t * getKeywords();
	wchar_t * getCreator();
	wchar_t * getProducer();
	char * getCreationDate();
	char * getLastModifiedDate();
};

struct ExportParams{
	AFPDFDoc * _this;
	char *fileName;
	int fromPage;
	int toPage;
	int quality;
	float renderDPI;
	int rotation;
	int WaitTime;
};
