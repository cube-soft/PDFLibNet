#pragma once
#include "LinkDestInterop.h"
	class LinkActionInterop;
	class AFPDFDocInterop;
	class LinkInterop
	{
		private:
			void *_link;
			AFPDFDocInterop *_pdfDoc;
			LinkActionInterop *_action;
		public:
			LinkInterop(void *ptr,AFPDFDocInterop *pdfdoc);
			~LinkInterop(void);

			double getx0();
			double getx1();
			double gety0();
			double gety1();
			LinkActionInterop *getAction();
	};

	class LinkActionInterop
	{
	private:
		void *_link;
		void *_action;
		AFPDFDocInterop *_pdfDoc;
		LinkDestInterop *_dest;
	public:
		LinkActionInterop(void *link,void *action,AFPDFDocInterop *pdfodc);
		~LinkActionInterop();
		int getKind();
		char *getDestName();
		LinkDestInterop *getDest();
		char *getURI();
	};