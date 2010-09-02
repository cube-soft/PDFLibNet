#pragma once
#include "LinkDestInterop.h"

namespace PDFLibNet
{
	public ref class LinkDest
	{
	private:
		LinkDestInterop *_ptr;
	internal:
		LinkDest(LinkDestInterop * ptr);
	protected:
		!LinkDest(void){
			delete _ptr;
		}
	public:
		property System::Double Left{
			System::Double get(){
				if(_ptr)
					return _ptr->getLeft();
				return 0;
			}
		}
		property System::Double Right{
			System::Double get(){
				if(_ptr)
					return _ptr->getRight();
				return 0;
			}
		}
		property System::Double Top{
			System::Double get(){
				if(_ptr)
					return _ptr->getTop();
				return 0;
			}
		}
		property System::Double Bottom{
			System::Double get(){
				if(_ptr)
					return _ptr->getBottom();
				return 0;
			}
		}
		property System::Double Zoom{
			System::Double get(){
				if(_ptr)
					return _ptr->getZoom();
				return 0;
			}
		}
		property System::Boolean ChangeLeft{
			System::Boolean get(){
				if(_ptr)
					return _ptr->getChangeLeft();
				return false;
			}
		}
		property System::Boolean ChangeTop{
			System::Boolean get(){
				if(_ptr)
					return _ptr->getChangeTop();
				return false;
			}
		}
		property System::Boolean ChangeZoom{
			System::Boolean get(){
				if(_ptr)
					return _ptr->getChangeZoom();
				return false;
			}
		}
		property System::Int32 Page{
			System::Int32 get(){
				if(_ptr!=0){
					int p=_ptr->getPageNum();
					return p;
				}
				return 1;
			}
		}
	};
}