#pragma once
namespace PDFLibNet
{
	
	public ref class PDFSearchResult
	{
	private:
		System::Drawing::Rectangle _position;
		long _pageNumber;
	internal:
		PDFSearchResult(long page,int left, int top, int right, int bottom);
	public:
		property System::Drawing::Rectangle Position{
			 System::Drawing::Rectangle get(){
				 return _position;
			}
		}
		property long Page{
			long get(){
				return _pageNumber;
			}
		}
	};

}