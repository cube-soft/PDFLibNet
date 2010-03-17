using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using PDFLibNet;

namespace PDFViewer
{
    public partial class PDFViewerCtrl : PageViewer
    {
        private PDFLibNet.PDFWrapper _pdfDoc;
        private string _configFile = "xpdfrc";
        public event EventHandler FileLoaded;
        public event EventHandler FileRendered;

        public PDFViewerCtrl()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Path to to the xpdfrc file
        /// </summary>
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

        /// <summary>
        /// Load PDF from stream
        /// </summary>
        /// <param name="stream">System Stream</param>
        public void LoadPDF(System.IO.Stream stream)
        {

        }

        /// <summary>
        /// Load PDF from file
        /// </summary>
        /// <param name="sFile">File path</param>
        public void LoadPDF(string sFile)
        {
            if (_pdfDoc != null)
            {
                _pdfDoc.Dispose();
                _pdfDoc = null;
            }

            _pdfDoc = new PDFWrapper();
            _pdfDoc.RenderNotifyFinished += new RenderNotifyFinishedHandler(_pdfDoc_RenderNotifyFinished);
            _pdfDoc.PDFLoadCompeted += new PDFLoadCompletedHandler(_pdfDoc_PDFLoadCompeted);
            _pdfDoc.PDFLoadBegin += new PDFLoadBeginHandler(_pdfDoc_PDFLoadBegin);
            
            if (LoadFile(sFile, _pdfDoc))
            {
                
                FileLoaded(this,new EventArgs());
                
                _pdfDoc.CurrentPage = 1;
                _pdfDoc.FitToWidth(Handle);
                _pdfDoc.RenderPage(Handle);

                PageSize = new Size(_pdfDoc.PageWidth, _pdfDoc.PageHeight);

                FileRendered(this, new EventArgs());
            }

        }

        void _pdfDoc_RenderNotifyFinished(int page, bool bSuccesss)
        {
        }

        void _pdfDoc_PDFLoadBegin()
        {   
        }

        void _pdfDoc_PDFLoadCompeted()
        {   
        }

        /// <summary>
        /// Auto ZoomIN
        /// </summary>
        public void ZoomIn()
        {
            CheckStatus();
            _pdfDoc.ZoomOut();
            _pdfDoc.RenderPage(Handle);
            Invalidate();
        }

        /// <summary>
        /// Auto ZoomOut
        /// </summary>
        public void ZoomOut()
        {
            CheckStatus();
            _pdfDoc.ZoomIN();
            _pdfDoc.RenderPage(Handle);
            Invalidate();
        }

        /// <summary>
        /// Enable/disable Antialias
        /// </summary>
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

        /// <summary>
        /// Enable/Disable vect orantialias
        /// </summary>
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

        /// <summary>
        /// Go to next page
        /// </summary>
        public new void NextPage()
        {
            CheckStatus();
            _pdfDoc.NextPage();
            _pdfDoc.RenderPage(this.Handle);
            Invalidate();
        }

        /// <summary>
        /// Go to previous page
        /// </summary>
        public new void  PreviousPage()
        {
            CheckStatus();
            _pdfDoc.PreviousPage();
            _pdfDoc.RenderPage(this.Handle);
            Invalidate();
        }

        /// <summary>
        /// Go to page number
        /// </summary>
        /// <param name="page">Absolute page number</param>
        public void GotoPage(int page)
        {
            CheckStatus();
            _pdfDoc.CurrentPage = page;
            _pdfDoc.RenderPage(this.Handle);
            Invalidate();
        }

        /// <summary>
        /// Page count
        /// </summary>
        public int PageCount
        {
            get
            {
                CheckStatus();
                return _pdfDoc.PageCount;
            }
        }

        string _OwnerPassword = string.Empty;
        /// <summary>
        /// Owner password
        /// </summary>
        public string OwnerPassword
        {
            set
            {
                CheckStatus();
                _OwnerPassword = value;
            }
        }
        string _UserPassword = string.Empty;
        /// <summary>
        /// User password
        /// </summary>
        public string UserPassword
        {
            set
            {
                CheckStatus();
                _UserPassword = value;
            }
        }

        private void CheckStatus()
        {
            if (_pdfDoc == null)
                throw new NullReferenceException("No file open");
        }


        System.IO.FileStream fs = null;
        private bool LoadFile(string filename, PDFLibNet.PDFWrapper pdfDoc)
        {
            try
            {
                if (fs != null)
                {
                    fs.Close();
                    fs = null;
                }
                fs = new System.IO.FileStream(filename, System.IO.FileMode.Open);
                return pdfDoc.LoadPDF(fs);

            }
            catch (System.Security.SecurityException)
            {    
                if (!_UserPassword.Equals(String.Empty))
                    pdfDoc.UserPassword =_UserPassword;
                if (!_OwnerPassword.Equals(String.Empty))
                    pdfDoc.OwnerPassword = _OwnerPassword;
                return LoadFile(filename, pdfDoc);
         
            }
        }

        /// <summary>
        /// Scroll page to position y
        /// </summary>
        /// <param name="y">Y position</param>
        public void ScrolltoTop(int y)
        {
            Point dr = ScrollPosition;
            if (_pdfDoc.PageHeight > Height)
                dr.Y = y;
            ScrollPosition = dr;
        }

    }
}

