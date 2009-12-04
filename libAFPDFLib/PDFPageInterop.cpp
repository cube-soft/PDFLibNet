#include "PDFPageInterop.h"
#include "AFPDFDoc.h"
#include "AFPDFDocInterop.h"
#include "DynArray.h"
#include "ImagesMemoryOutputDev.h"
#include "CRect.h"

static wchar_t		EmptyChar[1]						={'\0'};


//Try to decode UTF-8
wchar_t* UTF8translate(char artist[])
{
	int nNeeded = MultiByteToWideChar( CP_UTF8, NULL, artist, -1, 0, 0 );
	if( !nNeeded )
	{
		return NULL;
	}
 
	wchar_t* pWideChar = new wchar_t[nNeeded + 1];
	if( !MultiByteToWideChar( CP_UTF8, NULL, artist, -1, pWideChar, nNeeded ))
	{
		delete[] pWideChar;
		return NULL;
	}
	
	return pWideChar;
}


class pdfPageSelection 
	: public CRect{
public:
	wchar_t *text;
	double dpi;
	pdfPageSelection(wchar_t *text, double DPI, int x, int y, int w, int h)
		: CRect(x,y,x+w,y+h)
		, text(text)
		, dpi(DPI)
	{
	}
};

PDFPageInterop::PDFPageInterop(int page, void *lptr,void *pdfDoc)
: _page(page)
, _pdfPage(lptr)
, _pdfDoc(pdfDoc)
, _selectionArray(new DynArray<pdfPageSelection>())
, _text(0)
, dpi(0)
, _images(0)
, _ctmdpi(0)
{
}
PDFPageInterop::~PDFPageInterop(void)
{
	if(_text)
		delete _text;
	if(_images)
		delete _images;
}

int PDFPageInterop::getImagesCount(){
	if(_images)
		return ((ImagesMemoryOutputDev *)_images)->getImageCount();
	return 0;
}
unsigned char *PDFPageInterop::getImageBytes(int index){
	if(_images)
		return ((ImagesMemoryOutputDev *)_images)->getImageBytes(index);
	return 0;
}
int PDFPageInterop::getImageSize(int index){
	if(_images)
		return ((ImagesMemoryOutputDev *)_images)->getImageSize(index);
	return 0;
}
int PDFPageInterop::getImageType(int index){
	if(_images)
		return ((ImagesMemoryOutputDev *)_images)->getImageType(index);
	return 0;
}
int PDFPageInterop::getImageWidth(int index){
	if(_images)
		return ((ImagesMemoryOutputDev *)_images)->getImageWidth(index);
	return 0;
}

int PDFPageInterop::getImageNumComps(int index){
	if(_images)
		return ((ImagesMemoryOutputDev *)_images)->getImageNumComps(index);
	return 0;
}

int PDFPageInterop::getImageHeight(int index){
	if(_images)
		return ((ImagesMemoryOutputDev *)_images)->getImageHeight(index);
	return 0;
}


PageLinksInterop *PDFPageInterop::getLinks()
{
	AFPDFDocInterop *doc = (AFPDFDocInterop *)_pdfDoc;
	if(_page>=0 && _page<doc->GetPageCount()){
		PageLinksInterop *pl = doc->getPageLinksInterop(_page);
		return pl;
	}
	return 0;
}
int PDFPageInterop::getPage()
{
	return _page;
}
//Extract selected text
wchar_t *PDFPageInterop::extractText()
{
	CString str;
	DynArray<pdfPageSelection> *arr =(DynArray<pdfPageSelection> *)this->_selectionArray;
	char *buf;
	size_t bytes_c;
	int bufSize=4096;
	for(int i=0; i<arr->GetSize();++i){
		str.AppendChar('\n');
		wchar_t *txt =((pdfPageSelection)arr->operator [](i)).text;
		bufSize = wcslen(txt);
		buf = (char *)malloc(bufSize+2);
		wcstombs_s(&bytes_c,buf,bufSize,txt,bufSize);
		
		if(buf){
			str.Append(buf);
			free(buf);
		}
	}
	return str.AllocSysString();
}
wchar_t *PDFPageInterop::extractText(int xMin, int yMin, int xMax, int yMax) //Extract text from coordinates
{
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
	int x0, x1,y0,y1;
	takeText();
	TextPage *txt =(TextPage *)_text;
	this->CvtUserToDev(xMin,yMin,&x0,&y0);
	this->CvtUserToDev(xMax,yMax,&x1,&y1);

	GString *s1 = txt->getText(x0,y0,x1,y1);
	GBool isUnicode=gFalse;
	
	if(s1 && s1->getLength()>0){
		int size=s1->getLength();
		wchar_t *ret = NULL;
			
		GBool isUnicode=gFalse;
		Unicode u;
		int i;
		if ((s1->getChar(0) & 0xff) == 0xfe && (s1->getChar(1) & 0xff) == 0xff) {
			isUnicode = gTrue;
			i = 2;
		} else {
			isUnicode = gFalse;
			i = 0;
		}
		//Unicode Support
		if(!isUnicode)
			ret = UTF8translate(s1->getCString());
		if(ret==NULL)
		{
			ret =new wchar_t[size+1];
			int j=0;

			while (i < s1->getLength()) {
				  if (isUnicode) {
						u = ((s1->getChar(i) & 0xff) << 8) | (s1->getChar(i+1) & 0xff);
						i += 2;
				  } else {
						u = s1->getChar(i) & 0xff;
						++i;
				  }
				  ret[j] = u;
				  j++;
			}

			ret[j]='\0';
			
			delete s1;
			delete txt;	
		}
	}
	delete s1;
	return EmptyChar;
}
bool PDFPageInterop::hasText(int x, int y)	//Return true if the point is in text bounds
{
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
	int x0, x1,y0,y1;
	takeText();
	TextPage *txt =(TextPage *)_text;
	CvtUserToDev(x,y,&x0,&y0);
	
	if(txt){
		if(txt->hasText(x0,y0))
			return true;
	}
	return false;
}

void PDFPageInterop::extractImages()
{
	if(_images==0)
	{
		AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
		_images =(void *)new ImagesMemoryOutputDev();
		ImagesMemoryOutputDev *img=(ImagesMemoryOutputDev *)_images;
		if(doc->IsBusy())
			while(doc->IsBusy()) Sleep(50);
		doc->getDoc()->displayPage(img,this->_page,72,72,0,gTrue,gFalse,gFalse,0,0);
	}
}

/*bool PDFPageInterop::hasImage(int x, int y) //Return true if the point is in image bounds
{
	return false;
}
void PDFPageInterop::getImageBox(int x, int y, int *x1,int *y1, int *x2, int *y2) //Return the image bounds
{
}
void PDFPageInterop::exportImage(wchar_t *fileName,int quality,int x1, int y1, int x2, int y2)
{
}*/

void PDFPageInterop::getTextCharBox(int x, int y, int *x1,int *y1, int *x2, int *y2)
{
}
void PDFPageInterop::getTextWordBox(int x, int y, int *x1,int *y1, int *x2, int *y2)
{
}
void PDFPageInterop::addSelection(double dpi, int x1, int y1, int x2, int y2)
{
	DynArray<pdfPageSelection> *arr= (DynArray<pdfPageSelection> *)this->_selectionArray;
	arr->Add(*(new pdfPageSelection(NULL,dpi,x1,y1,x2-x1,y2-y1)));
}
void PDFPageInterop::clearSelection()
{
	DynArray<pdfPageSelection> *arr= (DynArray<pdfPageSelection> *)this->_selectionArray;
	arr->Clear();
}
void PDFPageInterop::takeText()
{
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
	//Extract text
	if(_text==0 || doc->GetRenderDPI()!=dpi){
		if(_text)	delete _text;
		TextOutputDev *txt = new TextOutputDev(NULL,gTrue,gFalse,gFalse);
		if(doc->IsBusy())
			while(doc->IsBusy()) Sleep(50);
		dpi = doc->GetRenderDPI();
		doc->getDoc()->displayPage(txt,_page,dpi,dpi,0,gFalse,gFalse,gFalse);
		((Page *)_pdfPage)->getDefaultCTM(ctm, dpi, dpi, 0,  gFalse, gFalse);
		_ctmdpi=dpi;
		_text = txt->takeText();
		delete txt;
	}
}
void PDFPageInterop::CvtUserToDevDpi(double dpi,int xu,int yu,int *xd,int *yd)
{
	if(_ctmdpi!=dpi){
		double lctm[6];
		((Page *)_pdfPage)->getDefaultCTM(lctm, dpi, dpi, 0,  gFalse, gFalse);
		*xd = (int)(lctm[0] * xu + lctm[2] * yu + lctm[4] + 0.5);
		*yd = (int)(lctm[1] * xu + lctm[3] * yu + lctm[5] + 0.5);
	}else{
		*xd = (int)(ctm[0] * xu + ctm[2] * yu + ctm[4] + 0.5);
		*yd = (int)(ctm[1] * xu + ctm[3] * yu + ctm[5] + 0.5);
	}
}
void PDFPageInterop::CvtUserToDev(int xu, int yu,int *xd,int *yd){
    *xd = (int)(ctm[0] * xu + ctm[2] * yu + ctm[4] + 0.5);
    *yd = (int)(ctm[1] * xu + ctm[3] * yu + ctm[5] + 0.5);
}

static void WriteToGString(void *stream,char *str, int len)
{
	GString *gstr =(GString *)stream;
	gstr->append(str,len);
}
static void WriteToCString(void *stream,char *str, int len)
{
	CString *gstr =(CString *)stream;
	gstr->Append(str);
}
	
void PDFPageInterop::getTextStream(void *stream){
	TextOutputFunc fn=(TextOutputFunc)&::OutputToDelegate;
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
	
	TextOutputDev *txt= new TextOutputDev((TextOutputFunc)fn,(void *)stream,gTrue,gFalse);
	doc->getDoc()->displayPage(txt,_page,72,72,0,gFalse,gFalse,gFalse);
	delete txt;

}

wchar_t *PDFPageInterop::getText(){
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
	GString *s1 = new GString();
	CString *s2 = new CString();

	globalParams->setTextEncoding("UTF-8");
	UnicodeMap *map=globalParams->getTextEncoding();
	
	
	TextOutputDev *txt= new TextOutputDev((TextOutputFunc)&WriteToGString,(void *)s1,gTrue,gFalse);
	if(doc->IsBusy())
		while(doc->IsBusy()) 
			Sleep(50);
	doc->getDoc()->displayPage(txt,_page,72,72,0,gFalse,gTrue,gFalse);
	
	int size = s1->getLength();
	if(size>0){
		wchar_t *ret = NULL;
			
		GBool isUnicode=gFalse;
		Unicode u;
		int i;
		if ((s1->getChar(0) & 0xff) == 0xfe && (s1->getChar(1) & 0xff) == 0xff) {
			isUnicode = gTrue;
			i = 2;
		} else {
			isUnicode = gFalse;
			i = 0;
		}
		//Unicode Support
		//if(!isUnicode)
		ret = UTF8translate(s1->getCString());
		if(ret==NULL)
		{
			ret =new wchar_t[size+1];
			int j=0;

			while (i < s1->getLength()) {
				  if (isUnicode) {
						u = ((s1->getChar(i) & 0xff) << 8) | (s1->getChar(i+1) & 0xff);
						i += 2;
				  } else {
						u = s1->getChar(i) & 0xff;
						++i;
				  }
				  ret[j] = u;
				  j++;
			}

			ret[j]='\0';
			
			delete s1;
			delete txt;	
		}

		return ret;	
	}
	delete s1;
	delete txt;
	return EmptyChar;
}

double PDFPageInterop::getPageWidth(){
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
	double w = doc->getDoc()->getPageCropWidth(_page);
  
	return 254*w/72;
}

char *PDFPageInterop::getISOANum(){
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
	double wISO, hISO;
	double w = doc->getDoc()->getPageCropWidth(_page);
    double h = doc->getDoc()->getPageCropHeight(_page);
    char *iso=new char[64];

	if ((fabs(w - 612) < 0.1 && fabs(h - 792) < 0.1) ||	(fabs(w - 792) < 0.1 && fabs(h - 612) < 0.1)) {
		//Letter page Size
		delete iso;
		return "letter";
	} else {
		//ISO A page Size
		hISO = sqrt(sqrt(2.0)) * 7200 / 2.54;
		wISO = hISO / sqrt(2.0);
		for (int i = 0; i <= 6; ++i) {
			if ((fabs(w - wISO) < 1 && fabs(h - hISO) < 1) || (fabs(w - hISO) < 1 && fabs(h - wISO) < 1)) {
				sprintf(iso,"A%d",i);
				return iso;
			}
			hISO = wISO;
			wISO /= sqrt(2.0);
		}
	}
	delete iso;
	//Unknow ISO Name Size
	return "unknown";
}

int PDFPageInterop::getPageRotate()
{
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
    int r = doc->getDoc()->getPageRotate(_page);
	return r;
}
double PDFPageInterop::getPageHeight(){
	AFPDFDoc *doc = (AFPDFDoc *)_pdfDoc;
    double h = doc->getDoc()->getPageCropHeight(_page);
    
	return 254*h/72;
}

void PDFPageInterop::RenderSelection(double dpi, long hdc,unsigned long color,unsigned long linecolor,int linewidth)
{
	HDC dc =(HDC)hdc;
	DynArray<pdfPageSelection> *arr= (DynArray<pdfPageSelection> *)this->_selectionArray;
	HGDIOBJ draw_pen, old_pen;
	HGDIOBJ old_brush;

	draw_pen = CreatePen(PS_SOLID, 0,(COLORREF)linecolor);
	old_pen = SelectObject(dc,draw_pen);
	//old_brush = SelectStockObject(dc,NULL_BRUSH);
	old_brush = SelectObject(dc,GetStockObject(NULL_BRUSH));

	for(int i=0;i<arr->GetCount();i++){
		const double mul = dpi / arr->operator [](i).dpi;
		CRect nsel =(CRect)arr->operator [](i);

		nsel.left   = int(nsel.left   * mul);
		nsel.top    = int(nsel.top    * mul);
		nsel.right  = int(nsel.right  * mul);
		nsel.bottom = int(nsel.bottom * mul);

		// enlarge right/bottom 
		nsel.right++;
		nsel.bottom++;

		::Rectangle(dc,nsel.left,nsel.top,nsel.right,nsel.bottom);
		::InvertRect(dc,&nsel);
	}
	SelectObject(dc,old_pen);
	SelectObject(dc,old_brush);
}