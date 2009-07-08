#include "PDFPage.h"
#include "PDFWrapper.h"

namespace PDFLibNet {

	void PDFPage::LoadFromFile(System::String ^filename){
		_wrapper=gcnew PDFLibNet::PDFWrapper();
		_wrapper->LoadPDF(filename);
	}
}