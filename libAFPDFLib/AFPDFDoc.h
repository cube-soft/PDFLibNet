#pragma once
#include "stdafx.h"
#include "OutlineItemA.h"
#include "PageMemory.h";

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
protected:
	long m_PageToRenderByThread;
	long m_LastPageRenderedByThread;
	static UINT RenderingThread( LPVOID param );
private:
	CWinThread *m_renderingThread;
	DynArray<CPDFSearchResult> m_Selection;
	PDFDoc *m_PDFDoc;
	SplashOutputDev	*m_splashOut;
	Outline *m_Outline;
	PageMemory *m_Bitmap;
	CString m_OwnerPassword;
	CString m_UserPassword;
	CRect m_bbox;
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

	void *m_bitmapBytes;
	
public:
	AFPDFDoc(char *configFile);
	virtual ~AFPDFDoc();
	long LoadFromFile(char *FileName, char *user_password, char *owner_password);
	long LoadFromFile(char *FileName, char *user_password);
	long LoadFromFile(char *sFileName);
	void SetUserPassword(char *user_password);
	void SetOwnerPassword(char *owner_password);
	
	long RenderPage(long lhWnd);
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

	int SaveJpg(char *fileName,int quality);
	int SaveTxt(char *fileName,int firstPage, int lastPage, bool htmlMeta,bool physLayout, bool rawOrder);

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
	long FindText(LPCTSTR sText, long iPage, long SearchOrder, bool bCaseSensitive, bool bBackward, bool bMarkAll, bool bWholeDoc);
	long FindNext(LPCTSTR sText);
	long FindPrior(LPCTSTR sText);
	long FindFirst(LPCTSTR sText, long SearchOrder, bool Backward);
	long RenderHDC(long lHdc);
	Links *GetLinksPage(long iPage);
	void cvtUserToDev(double ux, double uy, int *dx, int *dy);
	void cvtDevToUser(double ux, double uy, double *dx, double *dy);
	long GetSearchPage(void);
	long SetSearchPage(long lNewValue);
	bool GetSearchCaseSensitive(void);
	void SetSearchCaseSensitive(bool newVal);
	long PrintToFile(LPCTSTR fileName, int fromPage, int toPage);
	CPDFSearchResult *GetSearchResults();
	long GetSearchCount();
	int findpage(int num, int gen){
		return m_PDFDoc->findPage(num,gen);
	}
	void Dispose();

	char * getTitle();
	char * getAuthor();
	char * getSubject();
	char * getKeywords();
	char * getCreator();
	char * getProducer();
	char * getCreationDate();
	char * getLastModifiedDate();


	void static setAntialias(bool antialias){
		globalParams->setAntialias(antialias?"yes":"no");
	}
	void static setVectorAntialias(bool antialias){
		globalParams->setVectorAntialias(antialias?"yes":"no");
	}
	void static setPrintError(bool printError){
		globalParams->setErrQuiet(printError?0:1);
	}
	void static setPrintErrorFile(char *fileName){
		freopen( fileName, "w", stderr );
	}
	void static setEncoding(char *encoding){
		globalParams->setTextEncoding(encoding);
	}
	void static setTextPageBreaks(bool textpagebreak){
		globalParams->setTextPageBreaks(textpagebreak?1:0);
	}
	void static setTextEOL(bool textEol){
		globalParams->setTextEOL(textEol?"yes":"no");
	}
	void static setEnableFreeType(bool enableFreeType){
		globalParams->setEnableFreeType(enableFreeType?"yes":"no");
	}
/*
	globalParams->setPSPaperHeight(0);
	globalParams->setPSPaperWidth(0);
	globalParams->setPSPaperSize("");
	globalParams->setPSEmbedCIDPostScript(true);
	globalParams->setPSEmbedCIDTrueType(true);
	globalParams->setPSEmbedTrueType(true);
	globalParams->setPSEmbedType1(true);
	globalParams->setTextPageBreaks(true);
	globalParams->setTextEOL("yes");
	globalParams->setEnableFreeType("yes");
	globalParams->setEnableT1lib("no");*/

};


