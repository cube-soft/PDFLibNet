// pdfutils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\..\libAFPDFLib\jpeg.h"
#include "outline.h"

volatile LONG interlock=0;
#pragma data_seg(".USRMUTEX_SHARED")
volatile DWORD g_dwDSMutexOwner = 0;
volatile LONG  g_lLocker = 0;
#pragma data_seg()

#pragma comment(linker, "/section:.USRMUTEX_SHARED,rws")

int pdfinfo(PDFDoc *doc, UnicodeMap *uMap);
int pdftotext(PDFDoc *doc, UnicodeMap *uMap);
int pdftojpg(PDFDoc *doc,UnicodeMap *uMap);

GBool abortCallback(void *data);
static void printInfoString(Dict *infoDict, char *key, char *text,   UnicodeMap *uMap);
static void printInfoDate(Dict *infoDict, char *key, char *text);
static void printBox(char *text, PDFRectangle *box);
static void printInfoString(FILE *f, Dict *infoDict, char *key,char *text1, char *text2, UnicodeMap *uMap);
static void printInfoDate(FILE *f, Dict *infoDict, char *key, char *fmt);

//for all
static int firstPage = 1;
static int lastPage = 0;
static GBool printBoxes = gFalse;
static GBool printMetadata = gFalse;
static char textEncName[128] = "";
static char ownerPassword[33] = "\001";
static char userPassword[33] = "\001";
static char cfgFileName[256] = {"xpdfrc"};
static char utilType[7] ="info";
static GBool printVersion = gFalse;
static GBool printHelp = gFalse;
static GString *fileName;
GBool multiPage;

//totext:
static GBool physLayout = gFalse;
static GBool rawOrder = gFalse;
static GBool htmlMeta = gFalse;
static char textEOL[16] = "";
static GBool noPageBreaks = gFalse;
static GBool quiet = gFalse;
static GString *textFileName;

//tojpg
static char enableT1libStr[16] = "";
static char enableFreeTypeStr[16] = "yes";
static char antialiasStr[16] = "yes";
static char vectorAntialiasStr[16] = "yes";
static int resolution = 150;




static ArgDesc argDesc[] = {
  {"-op",		argString,	 utilType,			sizeof(utilType),		"Util type (info, totext, images, toppm, topps, fonts, tojpg)"},
  {"-f",		argInt,      &firstPage,        0,						"first page to convert"},
  {"-l",		argInt,      &lastPage,         0,						"last page to convert"},
  {"-box",		argFlag,     &printBoxes,       0,						"print the page bounding boxes"},
  {"-meta",		argFlag,     &printMetadata,    0,						"print the document metadata (XML)"},
  {"-enc",		argString,   textEncName,		sizeof(textEncName),	"output text encoding name"},
  {"-opw",		argString,   ownerPassword,		sizeof(ownerPassword),  "owner password (for encrypted files)"},
  {"-upw",		argString,   userPassword,		sizeof(userPassword),   "user password (for encrypted files)"},
  {"-cfg",      argString,   cfgFileName,		sizeof(cfgFileName),	"configuration file to use in place of .xpdfrc"},
  {"-v",		argFlag,     &printVersion,		0,   "print copyright and version info"},
  {"-h",		argFlag,     &printHelp,		0,   "print usage information"},
  {"-help",		argFlag,     &printHelp,		0,   "print usage information"},
  {"--help",	argFlag,     &printHelp,		0,   "print usage information"},
  {"-?",		argFlag,     &printHelp,		0,   "print usage information"},

  //params for pdftotext
  {"-layout",	argFlag,     &physLayout,		0,						"totext: maintain original physical layout"},
  {"-raw",		argFlag,     &rawOrder,			0,						"totext: keep strings in content stream order"},
  {"-htmlmeta", argFlag,	 &htmlMeta,			0,						"totext: generate a simple HTML file, including the meta information"},
  {"-enc",		argString,   textEncName,		sizeof(textEncName),	"totext: output text encoding name"},
  {"-eol",		argString,   textEOL,			sizeof(textEOL),		"totext: output end-of-line convention (unix, dos, or mac)"},
  {"-nopgbrk",	argFlag,     &noPageBreaks,		0,						"totext: don't insert page breaks between pages"},
  {"-q",		argFlag,     &quiet,			0,						"don't print any messages or errors"},

  //params for tojpg
#if HAVE_T1LIB_H
  {"-t1lib",    argString,      enableT1libStr, sizeof(enableT1libStr),		"enable t1lib font rasterizer: yes, no"},
#endif
  {"-freetype", argString,      enableFreeTypeStr, sizeof(enableFreeTypeStr),   "enable FreeType font rasterizer: yes, no"},
  {"-aa",       argString,      antialiasStr,   sizeof(antialiasStr),   "enable font anti-aliasing: yes, no"},
  {"-aaVector", argString,      vectorAntialiasStr, sizeof(vectorAntialiasStr),   "enable vector anti-aliasing: yes, no"},
  {"-r",      argInt,      &resolution,    0,	"resolution, in DPI (default is 150)"},
  {NULL}
};


int _tmain(int argc, char *argv[])
{
  PDFDoc *doc;
  GString *fileName;
  GString *ownerPW, *userPW;
  UnicodeMap *uMap;
  GBool ok;
  int exitCode;
  char *p;
  
  exitCode = 99;

  // parse args
  ok = parseArgs(argDesc, &argc, argv);
  if (!ok || argc < 2 || argc > 3 || printVersion || printHelp) {
    fprintf(stderr, "pdfutils version %s\n", xpdfVersion);
    fprintf(stderr, "%s\n", xpdfCopyright);
    if (!printVersion) {
      printUsage("pdfutils", "<PDF-file> [<text-file>|<directory>]", argDesc);
    }
    goto err0;
  }
  fileName = new GString(argv[1]);
  freopen( "C:\\stderr.log", "w", stderr );

  // read config file
  globalParams = new GlobalParams(cfgFileName);
  if (textEncName[0]) {
    globalParams->setTextEncoding(textEncName);
  }
  if (textEOL[0]) {
    if (!globalParams->setTextEOL(textEOL)) {
      error(-1, "Bad '-eol' value on command line\n");
    }
  }
  if (noPageBreaks) {
    globalParams->setTextPageBreaks(gFalse);
  }
  if (quiet) {
    globalParams->setErrQuiet(quiet);
  }
  if (enableT1libStr[0]) {
	if (!globalParams->setEnableT1lib(enableT1libStr)) {
	  error(-1, "Bad '-t1lib' value on command line\n");
	}
  }
  if (enableFreeTypeStr[0]) {
	if (!globalParams->setEnableFreeType(enableFreeTypeStr)) {
	  error(-1, "Bad '-freetype' value on command line\n");
	}
  }
  if (antialiasStr[0]) {
	if (!globalParams->setAntialias(antialiasStr)) {
	  error(-1, "Bad '-aa' value on command line\n");
	}
  }
  if (vectorAntialiasStr[0]) {
	if (!globalParams->setVectorAntialias(vectorAntialiasStr)) {
	  error(-1, "Bad '-aaVector' value on command line\n");
	}
  }

    // construct text file name
  if (argc == 3) {
    textFileName = new GString(argv[2]);
  } else {
    p = fileName->getCString() + fileName->getLength() - 4;
    if (!strcmp(p, ".pdf") || !strcmp(p, ".PDF")) {
      textFileName = new GString(fileName->getCString(),
				 fileName->getLength() - 4);
    } else {
      textFileName = fileName->copy();
    }
    textFileName->append(htmlMeta ? ".html" : ".txt");
  }


  // get mapping to output encoding
  if (!(uMap = globalParams->getTextEncoding())) {
    error(-1, "Couldn't get text encoding");
    delete fileName;
    goto err1;
  }

  // open PDF file
  if (ownerPassword[0] != '\001') {
    ownerPW = new GString(ownerPassword);
  } else {
    ownerPW = NULL;
  }
  if (userPassword[0] != '\001') {
    userPW = new GString(userPassword);
  } else {
    userPW = NULL;
  }
  doc = new PDFDoc(fileName, ownerPW, userPW);
  if (userPW) {
    delete userPW;
  }
  if (ownerPW) {
    delete ownerPW;
  }
  if (!doc->isOk()) {
    exitCode = 1;
    goto err2;
  }
	GString *meta = doc->readMetadata();
  // get page range
  if (firstPage < 1) {
    firstPage = 1;
  }
  if (lastPage == 0) {
    multiPage = gFalse;
    lastPage = 1;
  } else {
    multiPage = gTrue;
  }
  if (lastPage < 1 || lastPage > doc->getNumPages()) {
    lastPage = doc->getNumPages();
  }

  if(!strcmp(utilType,"info")){
	  exitCode = pdfinfo(doc,uMap);
  }else if(!strcmp(utilType,"totext")){
	  exitCode = pdftotext(doc,uMap);
  }else if(!strcmp(utilType,"tojpg")){
	exitCode =pdftojpg(doc,uMap);
  }

  // clean up
 err2:
  uMap->decRefCnt();
  delete doc;
 err1:
  delete globalParams;
 err0:

  // check for memory leaks
  Object::memCheck(stderr);
  gMemReport(stderr);

  return exitCode;
}

#include "GList.h"
::HANDLE hRenderFinished;
int pdftojpg(PDFDoc *doc,UnicodeMap *uMap)
{
	hRenderFinished = CreateEvent(NULL,TRUE,FALSE,TEXT("RenderFinishedEvent"));
	SplashOutputDev *splashOut;
	char ppmFile[512];
	int pg;

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
	
	
	//Note: the alignment is given by GDI requirements: bitmaps have to be 16-bit aligned.
	splashOut = new SplashOutputDev(splashModeBGR8, 4, gFalse, paperColor);
	splashOut->startDoc(doc->getXRef());
	CString errmsg;
	::InterlockedExchange(&g_lLocker, 0);
	for (pg = firstPage; pg <= lastPage; ++pg) {
		GBool (*fp)(void *);
		fp=abortCallback;
		doc->displayPage(splashOut, pg, resolution, resolution, 0,gFalse, gTrue, gFalse,fp,(void *)&interlock);

		SplashBitmap * bitmap = splashOut->getBitmap();
		int bmWidth = bitmap->getWidth();
		int bmHeight = bitmap->getHeight();
		bmi.bmiHeader.biWidth=bmWidth;
		bmi.bmiHeader.biHeight=bmHeight;

		sprintf(ppmFile, "%.*s-%06d.%s",(int)sizeof(ppmFile) - 32, textFileName->getCString(), pg,	"jpg");
		if(JpegFromDib((HANDLE)splashOut->getBitmap()->getDataPtr(),&bmi,70,CString(ppmFile),&errmsg)){
			USES_CONVERSION;
			error(-1,TEXT(errmsg.GetBuffer()));
			errmsg.ReleaseBuffer();
		}
	}
	delete splashOut;
	return 0;
}



int pdfinfo(PDFDoc *doc, UnicodeMap *uMap){
Object info;	
char buf[256];
double w, h, wISO, hISO;
FILE *f;
GString *metadata;
int pg, i;
Page *page;

  // print doc info
  doc->getDocInfo(&info);
  if (info.isDict()) {
    printInfoString(info.getDict(), "Title",        "Title:          ", uMap);
    printInfoString(info.getDict(), "Subject",      "Subject:        ", uMap);
    printInfoString(info.getDict(), "Keywords",     "Keywords:       ", uMap);
    printInfoString(info.getDict(), "Author",       "Author:         ", uMap);
    printInfoString(info.getDict(), "Creator",      "Creator:        ", uMap);
    printInfoString(info.getDict(), "Producer",     "Producer:       ", uMap);
    printInfoDate(info.getDict(),   "CreationDate", "CreationDate:   ");
    printInfoDate(info.getDict(),   "ModDate",      "ModDate:        ");
  }
  info.free();
  
  // print tagging info
  printf("Tagged:         %s\n",
	 doc->getStructTreeRoot()->isDict() ? "yes" : "no");

  // print page count
  printf("Pages:          %d\n", doc->getNumPages());

  // print encryption info
  printf("Encrypted:      ");
  if (doc->isEncrypted()) {
    printf("yes (print:%s copy:%s change:%s addNotes:%s)\n",
	   doc->okToPrint(gTrue) ? "yes" : "no",
	   doc->okToCopy(gTrue) ? "yes" : "no",
	   doc->okToChange(gTrue) ? "yes" : "no",
	   doc->okToAddNotes(gTrue) ? "yes" : "no");
  } else {
    printf("no\n");
  }

  // print page size
  for (pg = firstPage; pg <= lastPage; ++pg) {
    w = doc->getPageCropWidth(pg);
    h = doc->getPageCropHeight(pg);
    if (multiPage) {
      printf("Page %4d size: %g x %g pts", pg, w, h);
    } else {
      printf("Page size:      %g x %g pts", w, h);
    }
    if ((fabs(w - 612) < 0.1 && fabs(h - 792) < 0.1) ||
	(fabs(w - 792) < 0.1 && fabs(h - 612) < 0.1)) {
      printf(" (letter)");
    } else {
      hISO = sqrt(sqrt(2.0)) * 7200 / 2.54;
      wISO = hISO / sqrt(2.0);
      for (i = 0; i <= 6; ++i) {
	if ((fabs(w - wISO) < 1 && fabs(h - hISO) < 1) ||
	    (fabs(w - hISO) < 1 && fabs(h - wISO) < 1)) {
	  printf(" (A%d)", i);
	  break;
	}
	hISO = wISO;
	wISO /= sqrt(2.0);
      }
    }
    printf("\n");
  } 

  // print the boxes
  if (printBoxes) {
    if (multiPage) {
      for (pg = firstPage; pg <= lastPage; ++pg) {
	page = doc->getCatalog()->getPage(pg);
	sprintf(buf, "Page %4d MediaBox: ", pg);
	printBox(buf, page->getMediaBox());
	sprintf(buf, "Page %4d CropBox:  ", pg);
	printBox(buf, page->getCropBox());
	sprintf(buf, "Page %4d BleedBox: ", pg);
	printBox(buf, page->getBleedBox());
	sprintf(buf, "Page %4d TrimBox:  ", pg);
	printBox(buf, page->getTrimBox());
	sprintf(buf, "Page %4d ArtBox:   ", pg);
	printBox(buf, page->getArtBox());
      }
    } else {
      page = doc->getCatalog()->getPage(firstPage);
      printBox("MediaBox:       ", page->getMediaBox());
      printBox("CropBox:        ", page->getCropBox());
      printBox("BleedBox:       ", page->getBleedBox());
      printBox("TrimBox:        ", page->getTrimBox());
      printBox("ArtBox:         ", page->getArtBox());
    }
  }

  // print file size
#ifdef VMS
  f = fopen(fileName->getCString(), "rb", "ctx=stm");
#else
  f = fopen(fileName->getCString(), "rb");
#endif
  if (f) {
#if HAVE_FSEEKO
    fseeko(f, 0, SEEK_END);
    printf("File size:      %u bytes\n", (Guint)ftello(f));
#elif HAVE_FSEEK64
    fseek64(f, 0, SEEK_END);
    printf("File size:      %u bytes\n", (Guint)ftell64(f));
#else
    fseek(f, 0, SEEK_END);
    printf("File size:      %d bytes\n", (int)ftell(f));
#endif
    fclose(f);
  }

  // print linearization info
  printf("Optimized:      %s\n", doc->isLinearized() ? "yes" : "no");

  // print PDF version
  printf("PDF version:    %.1f\n", doc->getPDFVersion());

  // print the metadata
  if (printMetadata && (metadata = doc->readMetadata())) {
    fputs("Metadata:\n", stdout);
    fputs(metadata->getCString(), stdout);
    fputc('\n', stdout);
    delete metadata;
  }

  return 0;
}

wchar_t * GetTitle(UnicodeMap *uMap, OutlineItem *item)
{
	wchar_t *ret;
	int j,n;
	char buf[8];
	GString *title=new GString();
	char *s;
	if(item /*&& uMap!=NULL*/){
		ret =new wchar_t[item->getTitleLength()];
		
		//12/July/2009 - Map unicode characters, using n bytes per character
		for (j = 0; j < item->getTitleLength(); ++j) {
		  n = uMap->mapUnicode(item->getTitle()[j], buf, sizeof(buf));
		  title->append(buf, n);
		  ret[j]=(wchar_t)item->getTitle()[j];
		}
		ret[j]='\0';
		s = title->getCString();
		
	}else{
		return L"\0";
	}
	USES_CONVERSION;
	//ret =A2W(s);
	return A2W(s);
}

int pdftotext(PDFDoc *doc, UnicodeMap *uMap)
{
  TextOutputDev *textOut;
  FILE *f;
  Object info;
  GBool ok;

 // check for copy permission
  if (!doc->okToCopy()) {
    error(-1, "Copying of text from this document is not allowed.");
    return 3;
  }

  // write HTML header
  if (htmlMeta) {
    if (!textFileName->cmp("-")) {
      f = stdout;
    } else {
      if (!(f = fopen(textFileName->getCString(), "wb"))) {
	error(-1, "Couldn't open text file '%s'", textFileName->getCString());
	return 2;
      }
    }
    fputs("<html>\n", f);
    fputs("<head>\n", f);
    doc->getDocInfo(&info);
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
    fputs("</head>\n", f);
    fputs("<body>\n", f);
	fputs("<ul>\n",f);
	for(int itOutline=0;itOutline <  doc->getOutline()->getItems()->getLength();itOutline++){
		OutlineItem *ol = (OutlineItem *) doc->getOutline()->getItems()->get(itOutline);
		wchar_t *title =GetTitle(uMap,ol);
		fprintf(f,"<li>%s</li>\n",title);
	}
	fputs("<\ul>\n",f);

    fputs("<pre>\n", f);
    if (f != stdout) {
      fclose(f);
    }
  }

  // write text file
  textOut = new TextOutputDev(textFileName->getCString(),
			      physLayout, rawOrder, htmlMeta);
  if (textOut->isOk()) {
    doc->displayPages(textOut, firstPage, lastPage, 72, 72, 0,
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
	error(-1, "Couldn't open text file '%s'", textFileName->getCString());
	return 2;
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

static void printInfoString(Dict *infoDict, char *key, char *text,  UnicodeMap *uMap)
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

static void printInfoDate(Dict *infoDict, char *key, char *text) {
  Object obj;
  char *s;
  int year, mon, day, hour, min, sec, n;
  struct tm tmStruct;
  char buf[256];

  if (infoDict->lookup(key, &obj)->isString()) {
    fputs(text, stdout);
    s = obj.getString()->getCString();
    if (s[0] == 'D' && s[1] == ':') {
      s += 2;
    }
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
      if (mktime(&tmStruct) != (time_t)-1 &&
	  strftime(buf, sizeof(buf), "%c", &tmStruct)) {
	fputs(buf, stdout);
      } else {
	fputs(s, stdout);
      }
    } else {
      fputs(s, stdout);
    }



    fputc('\n', stdout);
  }
  obj.free();
}

static void printBox(char *text, PDFRectangle *box) {
  printf("%s%8.2f %8.2f %8.2f %8.2f\n",
	 text, box->x1, box->y1, box->x2, box->y2);
}

static void printInfoString(FILE *f, Dict *infoDict, char *key, char *text1, char *text2, UnicodeMap *uMap) 
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

static void printInfoDate(FILE *f, Dict *infoDict, char *key, char *fmt) {
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

__inline static void InterlockedSet(LONG *val, LONG newval)
{
 *((volatile LONG *)val) = newval;
}

__inline static LONG InterlockedGet(LONG *val)
{
 return *((volatile LONG *)val);
}

GBool abortCallback(void *data)
{
	//Very fast and safe, but must be global
	if(::InterlockedExchange(&g_lLocker,0)!=0){
		return gTrue;
	}
	//Very fast but not safe
	/*if(InterlockedGet((LONG *)data)==1)
		return gTrue;*/
	//Extremelly slow!
//	DWORD result=WaitForSingleObject(hRenderFinished,0);
//	if(result==WAIT_OBJECT_0)
		//return gTrue;
	return gFalse;
}