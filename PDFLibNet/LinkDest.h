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
				return _ptr->getLeft();
			}
		}
		property System::Double Right{
			System::Double get(){
				return _ptr->getRight();
			}
		}
		property System::Double Top{
			System::Double get(){
				return _ptr->getTop();
			}
		}
		property System::Double Bottom{
			System::Double get(){
				return _ptr->getBottom();
			}
		}
		property System::Double Zoom{
			System::Double get(){
				return _ptr->getZoom();
			}
		}
		property System::Boolean ChangeLeft{
			System::Boolean get(){
				return _ptr->getChangeLeft();
			}
		}
		property System::Boolean ChangeTop{
			System::Boolean get(){
				return _ptr->getChangeTop();
			}
		}
		property System::Boolean ChangeZoom{
			System::Boolean get(){
				return _ptr->getChangeZoom();
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