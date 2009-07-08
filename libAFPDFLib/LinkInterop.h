#pragma once
#include "LinkDestInterop.h"
	class LinkActionInterop;

	class LinkInterop
	{
		private:
			void *_link;
			void *_pdfDoc;
			LinkActionInterop *_action;
		public:
			LinkInterop(void *ptr,void *pdfdoc);
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
		void *_pdfDoc;
		LinkDestInterop *_dest;
	public:
		LinkActionInterop(void *link,void *action,void *pdfodc);
		~LinkActionInterop();
		int getKind();
		char *getDestName();
		LinkDestInterop *getDest();
		char *getURI();
	};