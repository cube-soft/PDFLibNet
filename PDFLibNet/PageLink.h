#pragma once
#include "LinkInterop.h"
#include "LinkDest.h"

namespace PDFLibNet
{
	ref class PageLinkGoTo;
	ref class PDFWrapper;
	public enum class LinkActionKind {
	  actionGoTo,			// go to destination
	  actionGoToR,			// go to destination in new file
	  actionLaunch,			// launch app (or open document)
	  actionURI,			// URI
	  actionNamed,			// named action
	  actionMovie,			// movie action
	  actionUnknown			// anything else
	};


	public ref class PageLinkAction
	{
	protected:
		LinkActionInterop *_action;
		PDFWrapper ^_pdfDoc;
	internal:
		PageLinkAction(LinkInterop *linkInterop,PDFWrapper ^pdfDoc);
	public:
		property LinkActionKind Kind {
			LinkActionKind get(){
				long kind =_action->getKind();
				return (LinkActionKind)System::Enum::ToObject(LinkActionKind::typeid,kind);
			}
		}
	};

	public ref class PageLinkURI
		: public PageLinkAction
	{
	private:
		LinkInterop *_ptr;
		System::String ^_uri;
	internal:
		PageLinkURI(LinkInterop *ptr,PDFWrapper ^pdfDoc): PageLinkAction(ptr,pdfDoc)
			, _ptr(ptr)
			, _uri(nullptr)
		{
		}
	public:
		property System::String ^URL{
			System::String ^get(){
				if(_uri==nullptr)
					_uri =gcnew System::String(_ptr->getAction()->getURI());
				return _uri;
			}
		}
	};

	public ref class PageLinkGoTo
		: public PageLinkAction
	{
	private:
		LinkInterop *_ptr;
		LinkDest ^_destination;
		System::String ^_destName;

		LinkDest ^FindDest(System::String ^destName);
	internal:
		PageLinkGoTo(LinkInterop *ptr,PDFWrapper ^pdfDoc): PageLinkAction(ptr,pdfDoc)
			, _ptr(ptr)
			, _destination(nullptr)
			, _destName(nullptr)
		{
		}
	public:
		property LinkDest ^Destination
		{
			LinkDest ^get(){
				if(_destination==nullptr){
					char *destName =_action->getDestName();
					LinkDestInterop *dest=this->_action->getDest();
					if((int)dest!=0)
						_destination = gcnew LinkDest(this->_action->getDest());
					else if((int)destName!=0)
						_destination = this->FindDest(%System::String(destName));
				}
				return _destination;
			}
		}
		property System::String ^DestinationName
		{
			System::String ^get(){
				if(_destName==nullptr){
					char *destName =_action->getDestName();
					if(destName!=0 && destName!="\0")
						_destName =gcnew System::String(destName);
				}
				return _destName;
			}
		}
	};

	public ref class PageLink
	{
	private:
		LinkInterop *_ptr;
		System::String ^_name;
		PageLinkAction ^_action;
		PDFWrapper ^_pdfDoc;
		System::Drawing::Rectangle _bounds;
	internal:
		PageLink(LinkInterop *linkInterop,PDFWrapper ^pdfDoc);
	protected:
		!PageLink(void){
			
		}
	public:
		
		property PageLinkAction ^Action 
		{
			PageLinkAction ^get(){
				if(_action==nullptr){
					switch(_ptr->getAction()->getKind()){
						case LinkActionKind::actionGoTo:
						case LinkActionKind::actionGoToR:
							_action =gcnew PageLinkGoTo(_ptr,_pdfDoc);
							break;
						case LinkActionKind::actionURI:
							_action =gcnew PageLinkURI(_ptr,_pdfDoc);
							break;
						default:
							_action =gcnew PageLinkAction(_ptr,_pdfDoc);
							break;
					}
					
				}
				return _action;
			}
		}
		property System::Drawing::Rectangle Bounds {
			System::Drawing::Rectangle  get(){
				System::Drawing::Rectangle r(_ptr->getx0(),_ptr->gety0(),_ptr->getx1()-_ptr->getx0(),_ptr->gety1()-_ptr->gety0());
				return r;
			}
		}
	};

}