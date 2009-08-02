using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PDFViewer
{
    public partial class PDFViewerCtrl : PageViewer
    {
        private PDFLibNet.PDFWrapper _pdfDoc;
        private string _configFile = "xpdfrc";
        public PDFViewerCtrl()
        {
            InitializeComponent();
        }

        public string ConfigFile
        {
            get
            {
                return _configFile;
            }
            set
            {
                _configFile = value;
            }
        }

        public void LoadFromFile(string sFile)
        {
            if (_pdfDoc == null)
            {
                _pdfDoc = new PDFLibNet.PDFWrapper();
                _pdfDoc.PDFLoadCompeted += new PDFLibNet.PDFLoadCompletedHandler(_pdfDoc_PDFLoadCompeted);
                _pdfDoc.PDFLoadBegin += new PDFLibNet.PDFLoadBeginHandler(_pdfDoc_PDFLoadBegin);
            }

            //if (LoadFile(sFile))
            {
                _pdfDoc.CurrentPage = 1;
                //Text = "Powered by xPDF: " + _pdfDoc.Author + " - " + _pdfDoc.Title;
            //    FillTree();
                //FitWidth();
                //Render();

                PageSize = new Size(_pdfDoc.PageWidth, _pdfDoc.PageHeight);
            }
        
        }

        void _pdfDoc_PDFLoadBegin()
        {
            
        }

        void _pdfDoc_PDFLoadCompeted()
        {
            
        }

        public void ZoomIn()
        {
            CheckStatus();
            _pdfDoc.ZoomOut();
            _pdfDoc.RenderPage(Handle);
            Invalidate();
        }

        public void ZoomOut()
        {
            CheckStatus();
            _pdfDoc.ZoomIN();
            _pdfDoc.RenderPage(Handle);
            Invalidate();
        }

        public bool Antialias
        {
            get
            {
                return PDFLibNet.xPDFParams.Antialias;
            }
            set
            {
                PDFLibNet.xPDFParams.Antialias = value;
                _pdfDoc.RenderPage(Handle, true);
                Invalidate();
            }
        }

        public bool VectorAntialias
        {
            get
            {
                return PDFLibNet.xPDFParams.VectorAntialias;
            }
            set
            {
                PDFLibNet.xPDFParams.VectorAntialias = value;
                _pdfDoc.RenderPage(Handle, true);
                Invalidate();
            }
        }

        public void NextPage()
        {
            CheckStatus();
            _pdfDoc.NextPage();
            _pdfDoc.RenderPage(this.Handle);
            Invalidate();
        }

        public void PreviousPage()
        {
            CheckStatus();
            _pdfDoc.PreviousPage();
            _pdfDoc.RenderPage(this.Handle);
            Invalidate();
        }

        public void GotoPage(int page)
        {
            CheckStatus();
            _pdfDoc.CurrentPage = page;
            _pdfDoc.RenderPage(this.Handle);
            Invalidate();
        }

        public int PageCount
        {
            get
            {
                CheckStatus();
                return _pdfDoc.PageCount;
            }
        }

        public string OwnerPassword
        {
            set
            {
                CheckStatus();
                _pdfDoc.OwnerPassword = value;
            }
        }

        public string UserPassword
        {
            set
            {
                CheckStatus();
                _pdfDoc.UserPassword=value;
            }
        }

        private void CheckStatus()
        {
            if (_pdfDoc == null)
                throw new NullReferenceException("No file open");
        }
    }
}

