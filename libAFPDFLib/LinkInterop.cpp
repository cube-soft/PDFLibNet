#include "LinkInterop.h"
#include "AFPDFDoc.h"
#include "AFPDFDocInterop.h"

LinkInterop::LinkInterop(void *ptr,AFPDFDocInterop *pdfdoc)
: _link(ptr)
, _pdfDoc(pdfdoc)
, _action(0)
{
	
}

LinkInterop::~LinkInterop(void)
{
	_link=0;
	if(_action!=0){
		delete _action;
		_action=0;
	}
}

double LinkInterop::getx0(){
	double x1,x2,y1,y2;
	((Link *)_link)->getRect(&x1,&y1,&x2,&y2);
	return x1;
}

double LinkInterop::getx1(){
	double x1,x2,y1,y2;
	((Link *)_link)->getRect(&x1,&y1,&x2,&y2);
	return x2;
}

double LinkInterop::gety0(){
	double x1,x2,y1,y2;
	((Link *)_link)->getRect(&x1,&y1,&x2,&y2);
	return y1;
}
double LinkInterop::gety1(){
	double x1,x2,y1,y2;
	((Link *)_link)->getRect(&x1,&y1,&x2,&y2);
	return y2;
}






int LinkActionInterop::getKind(){
	if(((Link *)_link)->getAction()!=0)
		return (int)((LinkAction *)_action)->getKind();
	return 0;
}
LinkActionInterop *LinkInterop::getAction(){
	if(_action==0)
		_action=new LinkActionInterop(_link,(void *)((Link *)_link)->getAction(),_pdfDoc);
	return _action;
}

char *LinkActionInterop::getDestName(){
	if(_link!=0){
		if(this->getKind()==LinkActionKind::actionGoTo){
			if(((Link *)_link)->isOk() 
				&& ((LinkGoTo *)_action)->isOk()){

				GString *g=((LinkGoTo *)_action)->getNamedDest();
				if(g!=0)
					return g->getCString();

			}
		}
	}
	return '\0';
}

LinkDestInterop *LinkActionInterop::getDest(){
	if(this->getKind() == LinkActionKind::actionGoTo){
		if(((LinkGoTo *)_action)->isOk()){
			if(_dest==0){
				LinkDest *dest =((LinkGoTo *)_action)->getDest();
				if(dest!=0)
					_dest=new LinkDestInterop(dest,_pdfDoc);
			}
			return  _dest;
		}
	}
	return 0;
}

char *LinkActionInterop::getURI(){
	if(this->getKind() ==LinkActionKind::actionURI){
		if(((Link *)_link)->isOk() && 
			((LinkURI *)_action)->isOk()){
			GString *g=((LinkURI *)_action)->getURI();
			return g->getCString();
		}
	}
	return '\0';
}

LinkActionInterop::LinkActionInterop(void *link,void *action,AFPDFDocInterop *pdfdoc)
: _link(link)
, _action(action)
, _dest(0)
, _pdfDoc(pdfdoc)
{

}


LinkActionInterop::~LinkActionInterop()
{
	if(_dest!=0){
		delete _dest;
		_dest=0;
	}
}
