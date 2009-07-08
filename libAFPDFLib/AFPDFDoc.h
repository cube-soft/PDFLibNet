#pragma once
#include "stdafx.h"
#include "OutlineItemA.h"

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
	CBitmap *m_Bitmap;
	CString m_OwnerPassword;
	CString m_UserPassword;
	CRect m_bbox;
	
	long m_CurrentPage;
	long m_LastPageRendered;
	long m_SearchPage;
	long m_PageHeight;
	long m_PageWidth;
	int m_Rotation;
	double m_LastRenderDPI;
	double m_renderDPI;

	long m_ViewOffsetX;
	long m_ViewOffsetY;

	bool m_HideMarks;
	bool m_bCaseSensitive;
	bool m_SearchStarted;

	void *m_bitmapBytes;
	
public:
	AFPDFDoc();
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
	long RenderBitmap(long lhWnd);
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
	/*char * getSubject();
	char * getKeywords();
	char * getCreator();
	char * getProducer();
	char * getCreationDate();
	char * getLastModifiedDate();*/

};


