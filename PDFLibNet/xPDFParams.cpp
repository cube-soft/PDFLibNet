
#include "xPDFParams.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace PDFLibNet
{
	xPDFParams::!xPDFParams()
	{
			
	}

	xPDFParams::xPDFParams(System::String ^configFile)
		: _globalParams(0)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(configFile);
		char *singleByte= (char*)ptr.ToPointer();
//		int ret;
		try{
			_globalParams=new XPDFParamsInterop(singleByte);
		}finally{
			Marshal::FreeCoTaskMem(ptr);
		}
	}
}
