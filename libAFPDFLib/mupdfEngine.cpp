#ifdef _MUPDF

#include "AuxOutputDev.h"
#include "mupdfEngine.h"
#include "error.h"


struct stuff
{
	fz_obj *resources;
	fz_obj *mediabox;
	fz_obj *cropbox;
	fz_obj *rotate;
};


static fz_error
getpagecount(pdf_xref *xref, fz_obj *node, int *pagesp)
{
	fz_error error;
	fz_obj *type;
	fz_obj *kids;
	fz_obj *count;
	char *typestr;
	int pages = 0;
	int i;

	if (!fz_isdict(node))
	{
		fz_warn("pagetree node is missing, igoring missing pages...");
		return fz_okay;
	}

	type = fz_dictgets(node, "Type");
	kids = fz_dictgets(node, "Kids");
	count = fz_dictgets(node, "Count");

	if (fz_isname(type))
		typestr = fz_toname(type);
	else
	{
		fz_warn("pagetree node (%d %d R) lacks required type", fz_tonum(node), fz_togen(node));

		kids = fz_dictgets(node, "Kids");
		if (kids)
		{
			fz_warn("guessing it may be a pagetree node, continuing...");
			typestr = "Pages";
		}
		else
		{
			fz_warn("guessing it may be a page, continuing...");
			typestr = "Page";
		}
	}

	if (!strcmp(typestr, "Page"))
		(*pagesp)++;

	else if (!strcmp(typestr, "Pages"))
	{
		if (!fz_isarray(kids))
			fz_warn("page tree node contains no pages");

		pdf_logpage("subtree (%d %d R) {\n", fz_tonum(node), fz_togen(node));

		for (i = 0; i < fz_arraylen(kids); i++)
		{
			fz_obj *obj = fz_arrayget(kids, i);

			/* prevent infinite recursion possible in maliciously crafted PDFs */
			if (obj == node)
				return fz_throw("corrupted pdf file");

			error = getpagecount(xref, obj, &pages);
			if (error)
				return fz_rethrow(error, "cannot load pagesubtree (%d %d R)", fz_tonum(obj), fz_togen(obj));
		}

		if (pages != fz_toint(count))
		{
			fz_warn("page tree node contains incorrect number of pages, continuing...");

			error = fz_newint(&count, pages);
			if (!error)
			{
				error = fz_dictputs(node, "Count", count);
				fz_dropobj(count);
			}
			if (error)
				return fz_rethrow(error, "cannot correct wrong page count");
		}

		pdf_logpage("%d pages\n", pages);

		(*pagesp) += pages;

		pdf_logpage("}\n");
	}

	return fz_okay;
}


fz_error
pdf_getpagecount(pdf_xref *xref, int *pagesp)
{
	fz_error error;
	fz_obj *ref;
	fz_obj *catalog;
	fz_obj *pages;

	ref = fz_dictgets(xref->trailer, "Root");
	catalog = fz_resolveindirect(ref);

	pages = fz_dictgets(catalog, "Pages");
	pdf_logpage("determining page count (%d %d R) {\n", fz_tonum(pages), fz_togen(pages));

	*pagesp = 0;
	error = getpagecount(xref, pages, pagesp);
	if (error)
		return fz_rethrow(error, "cannot determine page count");

	pdf_logpage("}\n");

	return fz_okay;
}

static fz_error
getpageobject(pdf_xref *xref, struct stuff inherit, fz_obj *node, int *pagesp, int pageno, fz_obj **pagep)
{
	fz_error error;
	char *typestr;
	fz_obj *type;
	fz_obj *kids;
	fz_obj *count;
	fz_obj *inh;
	int i;

	if (!fz_isdict(node))
	{
		fz_warn("pagetree node is missing, ignoring missing pages...");
		*pagep = NULL;
		return fz_okay;
	}

	type = fz_dictgets(node, "Type");
	kids = fz_dictgets(node, "Kids");
	count = fz_dictgets(node, "Count");

	if (fz_isname(type))
		typestr = fz_toname(type);
	else
	{
		fz_warn("pagetree node (%d %d R) lacks required type", fz_tonum(node), fz_togen(node));

		kids = fz_dictgets(node, "Kids");
		if (kids)
		{
			fz_warn("guessing it may be a pagetree node, continuing...");
			typestr = "Pages";
		}
		else
		{
			fz_warn("guessing it may be a page, continuing...");
			typestr = "Page";
		}
	}

	if (!strcmp(typestr, "Page"))
	{
		(*pagesp)++;
		if (*pagesp == pageno)
		{
			pdf_logpage("page %d (%d %d R)\n", *pagesp, fz_tonum(node), fz_togen(node));

			if (inherit.resources && !fz_dictgets(node, "Resources"))
			{
				pdf_logpage("inherited resources\n");
				error = fz_dictputs(node, "Resources", inherit.resources);
				if (error)
					return fz_rethrow(error, "cannot inherit page tree resources");
			}

			if (inherit.mediabox && !fz_dictgets(node, "MediaBox"))
			{
				pdf_logpage("inherit mediabox\n");
				error = fz_dictputs(node, "MediaBox", inherit.mediabox);
				if (error)
					return fz_rethrow(error, "cannot inherit page tree mediabox");
			}

			if (inherit.cropbox && !fz_dictgets(node, "CropBox"))
			{
				pdf_logpage("inherit cropbox\n");
				error = fz_dictputs(node, "CropBox", inherit.cropbox);
				if (error)
					return fz_rethrow(error, "cannot inherit page tree cropbox");
			}

			if (inherit.rotate && !fz_dictgets(node, "Rotate"))
			{
				pdf_logpage("inherit rotate\n");
				error = fz_dictputs(node, "Rotate", inherit.rotate);
				if (error)
					return fz_rethrow(error, "cannot inherit page tree rotate");
			}

			*pagep = node;
		}
	}

	else if (!strcmp(typestr, "Pages"))
	{
		if (!fz_isarray(kids))
			fz_warn("page tree node contains no pages");

		if (*pagesp + fz_toint(count) < pageno)
		{
			(*pagesp) += fz_toint(count);
			return fz_okay;
		}

		inh = fz_dictgets(node, "Resources");
		if (inh) inherit.resources = inh;

		inh = fz_dictgets(node, "MediaBox");
		if (inh) inherit.mediabox = inh;

		inh = fz_dictgets(node, "CropBox");
		if (inh) inherit.cropbox = inh;

		inh = fz_dictgets(node, "Rotate");
		if (inh) inherit.rotate = inh;

		pdf_logpage("subtree (%d %d R) {\n", fz_tonum(node), fz_togen(node));

		for (i = 0; !(*pagep) && i < fz_arraylen(kids); i++)
		{
			fz_obj *obj = fz_arrayget(kids, i);

			/* prevent infinite recursion possible in maliciously crafted PDFs */
			if (obj == node)
				return fz_throw("corrupted pdf file");

			error = getpageobject(xref, inherit, obj, pagesp, pageno, pagep);
			if (error)
				return fz_rethrow(error, "cannot load pagesubtree (%d %d R)", fz_tonum(obj), fz_togen(obj));
		}

		pdf_logpage("}\n");
	}

	return fz_okay;
}

fz_error
pdf_getpageobject(pdf_xref *xref, int pageno, fz_obj **pagep)
{
	fz_error error;
	struct stuff inherit;
	fz_obj *ref;
	fz_obj *catalog;
	fz_obj *pages;
	int count;

	inherit.resources = NULL;
	inherit.mediabox = NULL;
	inherit.cropbox = NULL;
	inherit.rotate = NULL;

	ref = fz_dictgets(xref->trailer, "Root");
	catalog = fz_resolveindirect(ref);

	pages = fz_dictgets(catalog, "Pages");
	pdf_logpage("get page %d (%d %d R) {\n", pageno, fz_tonum(pages), fz_togen(pages));

	*pagep = NULL;
	count = 0;
	error = getpageobject(xref, inherit, pages, &count, pageno, pagep);
	if (error)
		return fz_rethrow(error, "cannot find page %d", pageno);

	pdf_logpage("}\n");

	return fz_okay;
}

static fz_error
findpageobject(pdf_xref *xref, fz_obj *node, fz_obj *page, int *pagenop, int *foundp)
{
	fz_error error;
	char *typestr;
	fz_obj *type;
	fz_obj *kids;
	int i;

	if (!fz_isdict(node))
		return fz_throw("pagetree node is missing");

	type = fz_dictgets(node, "Type");
	kids = fz_dictgets(node, "Kids");

	if (fz_isname(type))
		typestr = fz_toname(type);
	else
	{
		fz_warn("pagetree node (%d %d R) lacks required type", fz_tonum(node), fz_togen(node));

		kids = fz_dictgets(node, "Kids");
		if (kids)
		{
			fz_warn("guessing it may be a pagetree node, continuing...");
			typestr = "Pages";
		}
		else
		{
			fz_warn("guessing it may be a page, continuing...");
			typestr = "Page";
		}
	}

	if (!strcmp(typestr, "Page"))
	{
		(*pagenop)++;
		if (fz_tonum(node) == fz_tonum(page))
		{
			pdf_logpage("page %d (%d %d R)\n", *pagenop, fz_tonum(node), fz_togen(node));
			*foundp = 1;
		}
	}

	else if (!strcmp(typestr, "Pages"))
	{
		if (!fz_isarray(kids))
			fz_warn("page tree node contains no pages");

		pdf_logpage("subtree (%d %d R) {\n", fz_tonum(node), fz_togen(node));

		for (i = 0; !(*foundp) && i < fz_arraylen(kids); i++)
		{
			fz_obj *obj = fz_arrayget(kids, i);

			/* prevent infinite recursion possible in maliciously crafted PDFs */
			if (obj == node)
				return fz_throw("corrupted pdf file");

			error = findpageobject(xref, obj, page, pagenop, foundp);
			if (error)
				return fz_rethrow(error, "cannot load pagesubtree (%d %d R)", fz_tonum(obj), fz_togen(obj));
		}

		pdf_logpage("}\n");
	}

	return fz_okay;
}

fz_error
pdf_findpageobject(pdf_xref *xref, fz_obj *page, int *pagenop)
{
	fz_error error;
	fz_obj *ref;
	fz_obj *catalog;
	fz_obj *pages;
	int found;

	ref = fz_dictgets(xref->trailer, "Root");
	catalog = fz_resolveindirect(ref);

	pages = fz_dictgets(catalog, "Pages");
	pdf_logpage("find page object (%d %d R) (%d %d R) {\n", fz_tonum(page), fz_togen(page), fz_tonum(pages), fz_togen(pages));

	*pagenop = 0;
	found = 0;
	error = findpageobject(xref, pages, page, pagenop, &found);
	if (error)
		return fz_rethrow(error, "cannot find page object (%d %d R)", fz_tonum(page), fz_togen(page));

	pdf_logpage("}\n");

	if (!found)
		return fz_throw("cannot find page object (%d %d R)", fz_tonum(page), fz_togen(page));

	return fz_okay;
}


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


	if (_xref->crypt){
		if(pdf_setpassword(_xref->crypt,""))
			return 0;
		if(own_pwd || usr_pwd)
			if(pdf_setpassword(_xref->crypt, own_pwd) || pdf_setpassword(_xref->crypt,usr_pwd))
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
	if(fz_rendertree(&image, _rast, page->tree, ctm, fz_roundrect(bbox), 1)){
		error(0,"Can not render page, unknown problem with mupdf");
		return NULL;
	}
	out->setSize(image->w,image->h);
	out->SetDataPtr(image->samples);
	double dctm[6] = {ctm.a,ctm.b,ctm.c,ctm.d,ctm.e,ctm.f};
	out->setDefCTM(dctm);
	out->setDefICTM(dctm);
    ConvertPixmapForWindows(image);
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