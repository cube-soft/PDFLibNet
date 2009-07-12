#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PDFLibNet {

	/// <summary>
	/// Summary for PDFPage
	/// </summary>
	public ref class PDFPage :
		public System::Windows::Forms::UserControl
	{
	private:
		System::Drawing::Bitmap ^_backbuffer;
	public:
		
		PDFPage(void)
			: _backbuffer(nullptr)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->SetStyle(System::Windows::Forms::ControlStyles::UserPaint |
				System::Windows::Forms::ControlStyles::AllPaintingInWmPaint |
				System::Windows::Forms::ControlStyles::OptimizedDoubleBuffer,true);
			
		}

	

		bool LoadFile(System::String ^fileName){
			return true;
		}
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
	private: System::Windows::Forms::HScrollBar^  hScrollBar1;
	protected: 
	private: System::Windows::Forms::VScrollBar^  vScrollBar1;

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
			this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->vScrollBar1 = (gcnew System::Windows::Forms::VScrollBar());
			this->SuspendLayout();
			// 
			// hScrollBar1
			// 
			this->hScrollBar1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->hScrollBar1->Location = System::Drawing::Point(0, 499);
			this->hScrollBar1->Name = L"hScrollBar1";
			this->hScrollBar1->Size = System::Drawing::Size(534, 17);
			this->hScrollBar1->TabIndex = 0;
			// 
			// vScrollBar1
			// 
			this->vScrollBar1->Dock = System::Windows::Forms::DockStyle::Right;
			this->vScrollBar1->Location = System::Drawing::Point(517, 0);
			this->vScrollBar1->Name = L"vScrollBar1";
			this->vScrollBar1->Size = System::Drawing::Size(17, 499);
			this->vScrollBar1->TabIndex = 1;
			// 
			// PDFPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->vScrollBar1);
			this->Controls->Add(this->hScrollBar1);
			this->Name = L"PDFPage";
			this->Size = System::Drawing::Size(534, 516);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
