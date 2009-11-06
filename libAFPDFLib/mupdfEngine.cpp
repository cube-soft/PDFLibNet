#ifdef _MUPDF

#include "AuxOutputDev.h"
#include "mupdfEngine.h"
#include "error.h"


static void ConvertPixmapForWindows(fz_pixmap *image)
{
   int bmpstride = ((image->w * 3 + 3) / 4) * 4;
   int imageh = image->h;
   int imagew = image->w;
   unsigned char *bmpdata = (unsigned char*)fz_malloc(image->h * bmpstride);
   if (!bmpdata)
       return;

   unsigned char *p = bmpdata;
   unsigned char *s = image->samples;
   for (int y = 0; y < imageh; y++)
   {
       unsigned char *pl = p;
       unsigned char *sl = s;
       for (int x = 0; x < imagew; x++)
       {
           pl[0] = sl[3];
           pl[1] = sl[2];
           pl[2] = sl[1];
           pl += 3;
           sl += 4;
       }
       p += bmpstride;
       s += imagew * 4;
   }
   fz_free(image->samples);
   image->samples = bmpdata;
}

mupdfEngine::mupdfEngine()
: _pages(NULL)
, _rast(NULL)
, _xref(NULL)
, _page(NULL)
{
}

mupdfEngine::~mupdfEngine()
{
#ifdef CACHE_MUPDF_PAGES
	  if (_pages) {
        for (int i=0; i < _pageCount; i++) {
            if (_pages[i])
                pdf_droppage(_pages[i]);
        }
        free(_pages);
    }
#endif

    if (_xref) {
        if (_xref->store)
            pdf_dropstore(_xref->store);
        _xref->store = 0;
        pdf_closexref(_xref);
    }

    if (_rast)
        fz_droprenderer(_rast);
}

int mupdfEngine::LoadFile(char *fileName,char *own_pwd,char *usr_pwd){

	//start xref
	if(_xref==NULL){
		_xref=new pdf_xref();
		if (pdf_newxref(&_xref)){
			error(0,"XRef Read error, invalid file");
			return 1;
		}
	}

	if (pdf_loadxref(_xref, (TCHAR *)fileName)) {
		if (pdf_repairxref(_xref, (TCHAR *)fileName)){
			error(0,"XRef Read error, corrupt data. Can not repair");
			return 2;
		}
	}

	if (pdf_decryptxref(_xref)){
		error(0,"XRef Decrypt error");
		return 3;
	}

	if (pdf_needspassword(_xref)) {
        
		if (pdf_authenticatepassword(_xref, ""))
			return 0;
        if(pdf_authenticatepassword(_xref, own_pwd) || pdf_authenticatepassword(_xref, usr_pwd))
				return 0;
        error(0,"File needs password, invalid password");
        return 4;
    }

	return 0;
}

fz_pixmap* mupdfEngine::display(AuxOutputDev *out,int pageNo, int rotate, double zoom, GBool (*abortCheckCbk)(void *), void *abortCheckCbkData)
{
    fz_matrix ctm;
    fz_rect bbox;
    fz_pixmap* image = NULL;
	pdf_page* page = GetPage(pageNo);
	fz_rect *pageRect = &page->mediabox;

    if (!_rast) 
        fz_newrenderer(&_rast, pdf_devicergb, 0, 1024 * 512);
	if (!page){
        error(0,"Can not create mupdf renderer");
		return NULL;
	}

    ctm = viewctm(page, zoom, rotate);
    bbox = fz_transformaabb(ctm, *pageRect);
	//ctm = fz_invertmatrix(ctm);
	if(fz_rendertree(&image, _rast, page->tree, ctm, fz_roundrect(bbox), 1)){
		error(0,"Can not render page, unknown problem with mupdf");
		return NULL;
	}
	out->setSize(image->w,image->h);
	out->SetDataPtr(image->samples);

	/*double dctm[6] = {ctm.a,ctm.b,ctm.c,ctm.d,ctm.e,ctm.f};
	out->setDefCTM(dctm);
	out->setDefICTM(dctm);*/

    ConvertPixmapForWindows(image);

#ifndef CACHE_MUPDF_PAGES
	pdf_droppage(page);
#endif
	return image;
}

pdf_page *mupdfEngine::GetPage(int pageNo)
{
	pdf_page* page=NULL;
	fz_obj * obj;

#ifdef CACHE_MUPDF_PAGES
	if (!_pages){
		int _pageCount;
		pdf_getpagecount(_xref,&_pageCount);
        _pages = (pdf_page**)malloc(sizeof(pdf_page*) * _pageCount);
		for (int i = 0; i < _pageCount; i++)
			_pages[i] = NULL;
	}
    page = _pages[pageNo-1];
    if (page) 
        return page;
#endif   
    
	if(!pdf_getpageobject(_xref, pageNo,&obj)){
        if(pdf_loadpage(&page, _xref, obj))
			return NULL;
	}
#ifdef CACHE_MUPDF_PAGES
	_pages[pageNo-1]=page;
#endif
    return page;
}

fz_matrix mupdfEngine::viewctm(pdf_page *page, float zoom, int rotate)
{
    fz_matrix ctm;
    ctm = fz_identity();
    //ctm = fz_concat(ctm, fz_translate(0, -page->mediabox.y1));
    ctm = fz_concat(ctm, fz_translate(-page->mediabox.x0, -page->mediabox.y1));
    ctm = fz_concat(ctm, fz_scale(zoom, -zoom));
    rotate += page->rotate;
    if (rotate != 0)
        ctm = fz_concat(ctm, fz_rotate(rotate));
    return ctm;
}

#endif