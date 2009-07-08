#pragma once
#include "LinkInterop.h"
#include "LinkDest.h"

namespace PDFLibNet
{
	ref class PageLinkGoTo;
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
	internal:
		PageLinkAction(LinkInterop *linkInterop);
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
		PageLinkURI(LinkInterop *ptr): PageLinkAction(ptr)
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
	internal:
		PageLinkGoTo(LinkInterop *ptr): PageLinkAction(ptr)
			, _ptr(ptr)
			, _destination(nullptr)
		{
		}
	public:
		property LinkDest ^Destination
		{
			LinkDest ^get(){
				if(_destination==nullptr){
					if((int)this->_action->getDest()!=0)
						_destination = gcnew LinkDest((int)this->_action->getDest());
					else return nullptr;
				}
				
				return _destination;
			}
		}
	};

	public ref class PageLink
	{
	private:
		LinkInterop *_ptr;
		System::String ^_name;
		PageLinkAction ^_action;
		System::Drawing::Rectangle _bounds;
	internal:
		PageLink(LinkInterop *linkInterop);
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
							_action =gcnew PageLinkGoTo(_ptr);
							break;
						case LinkActionKind::actionURI:
							_action =gcnew PageLinkURI(_ptr);
							break;
						default:
							_action =gcnew PageLinkAction(_ptr);
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