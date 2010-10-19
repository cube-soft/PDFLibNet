#pragma once
class CRect 
	: public tagRECT
{
public:
	int width;
	int height;
	CRect(int x, int y, int r, int b){
		left=x;
		top=y;
		width = r - x;
		height = b - y;
		right = r;
		bottom = b;
	}
	
	bool NotEmpty(){
		return (width>0 || height>0 || left>0 || top>0);
	}
	void OffsetRect(int dx, int dy){
		::OffsetRect(this,dx,dy);
	}
	void InflateRect(int dx, int dy)
	{	
		::InflateRect(this,dx,dy);
	}
	void DeflateRect(int dx, int dy){
		::InflateRect(this,-dx,-dy);
	}
	CRect(){
		left=right=top=bottom=width=height=0;
	}
};