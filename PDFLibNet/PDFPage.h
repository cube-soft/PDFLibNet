#pragma once
#include "pdfwrapper.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing;


namespace PDFLibNet {

	/// <summary>
	/// Summary for PDFPage
	/// </summary>
	public ref class PDFPage : public System::Windows::Forms::UserControl
	{
	private:
		PDFLibNet::PDFWrapper ^_wrapper;
	public:
		PDFPage(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void LoadFromFile(System::String ^filename);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PDFPage()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		}
#pragma endregion

		
		
	};
}
