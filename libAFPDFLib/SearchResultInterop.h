#pragma once

class SearchResultInterop
{
	
public:
	SearchResultInterop(int page, int x0, int y0,int x1,int y1);
	~SearchResultInterop();
	int Page;
	int x0;
	int x1;
	int y0;
	int y1;
};
