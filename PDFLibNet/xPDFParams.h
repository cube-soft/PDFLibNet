#pragma once
#include "XPDFParamsInterop.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace PDFLibNet
{
	public ref class xPDFParams
	{
	private:
		static xPDFParams ^_default = gcnew xPDFParams(System::Convert::ToString(System::Configuration::ConfigurationSettings::GetConfig("xpdfrc")));
		XPDFParamsInterop *_globalParams;
		System::String ^_configFile;
		System::String ^_errorFile;
	public:
		xPDFParams(System::String ^configFile);
		!xPDFParams();


		property static System::String ^FileName 
		{
			System::String ^get(){
				return _default->_configFile;
			}
			void set(System::String ^value){
				IntPtr ptr = Marshal::StringToCoTaskMemAnsi(value);
				char *singleByte= (char*)ptr.ToPointer();
//				int ret;
				try{
					_default->_globalParams->OpenFile(singleByte);
					_default->_configFile = value;
				}finally{
					Marshal::FreeCoTaskMem(ptr);
				}				
			}
		}
		property static System::Boolean Antialias {
			System::Boolean get(){
				return _default->_globalParams->getAntialias();
			}
			void set(System::Boolean value){
				_default->_globalParams->setAntialias(value);
			}
		}

		property static System::Boolean VectorAntialias {
			System::Boolean get(){
				return _default->_globalParams->getVectorAntialias();
			}
			void set(System::Boolean value){
				_default->_globalParams->setVectorAntialias(value);
			}
		}

		property static System::Boolean ErrorQuiet {
			System::Boolean get(){
				return _default->_globalParams->getPrintError();
			}
			void set(System::Boolean value){
				_default->_globalParams->setPrintError(value);
			}
		}

		property static System::String ^ErrorFile 
		{
			System::String ^get(){
				return _default->_errorFile;
			}
			void set(System::String ^value){
				IntPtr ptr = Marshal::StringToCoTaskMemAnsi(value);
				char *singleByte= (char*)ptr.ToPointer();
//				int ret;
				try{
					_default->_globalParams->setPrintErrorFile(singleByte);
					_default->_errorFile = value;
				}finally{
					Marshal::FreeCoTaskMem(ptr);
				}	
			}
		}
	};
}