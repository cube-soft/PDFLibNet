#pragma once

class XPDFParamsInterop
{
public:
	XPDFParamsInterop();
	XPDFParamsInterop(char *configFile);
	~XPDFParamsInterop(void);

	void OpenFile(char *configFile);

	void  setAntialias(bool antialias);
	bool  getAntialias();

	void  setVectorAntialias(bool antialias);
	bool  getVectorAntialias();

	void  setPrintError(bool printError);
	bool  getPrintError();

	void  setPrintErrorFile(char *fileName);
	
	void  setEncoding(char *encoding);
	char *getEncoding();
	
	void  setTextPageBreaks(bool textpagebreak);
	bool  getTextPageBreaks();

	void  setTextEOL(bool textEol);
	bool  getTextEOL();

	void  setEnableFreeType(bool enableFreeType);
	bool  getEnableFreeType();


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
