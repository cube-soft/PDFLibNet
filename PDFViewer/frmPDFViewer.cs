using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices; 
using PDFLibNet;
namespace PDFViewer
{
  
    public partial class frmPDFViewer : Form
    {
        [DllImport("user32.dll")]
        static extern int GetForegroundWindow(); 
        
        #region Mouse Scrolling/Navigation Private Fields
        public enum CursorStatus
        {
            Select,
            Move,
            Zoom,
            Snapshot
        }

        Rectangle EmptyRectangle = new Rectangle(-1, -1, 0, 0);
        CursorStatus _cursorStatus = CursorStatus.Move;
        Point _pointStart = Point.Empty;
        Point _pointCurrent = Point.Empty;

        Point _bMouseCapturedStart = Point.Empty;
        bool _bMouseCaptured = false;
        int _pointX = 0;
        int _pointY = 0;
        #endregion

        PDFWrapper _pdfDoc = null;
        private Bitmap _backbuffer = null;

        public frmPDFViewer()
        {
            InitializeComponent();
        }

       

        void frmPDFViewer_FormClosing(object sender, FormClosingEventArgs e)
        {
            Gma.UserActivityMonitor.HookManager.MouseWheel -= new MouseEventHandler(HookManager_MouseWheel);
            Gma.UserActivityMonitor.HookManager.MouseDown -= new MouseEventHandler(HookManager_MouseDown);
            Gma.UserActivityMonitor.HookManager.MouseUp -= new MouseEventHandler(HookManager_MouseUp);
            Gma.UserActivityMonitor.HookManager.MouseMove -= new MouseEventHandler(HookManager_MouseMove);
        }

        #region Mouse Scrolling
        private CursorStatus getCursorStatus(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Middle)
            {
                return CursorStatus.Move;
            }
            return _cursorStatus;
        }
        private bool IsActive
        {
            get
            {
                return GetForegroundWindow() == this.Handle.ToInt32();
            }
        }
        private bool MouseInPage(Point p)
        {
            if (IsActive)
            {
                Rectangle rect = new Rectangle(new Point(0, 0), pnlPageContanier.ClientSize);
                return rect.Contains(p);
            }
            return false;
        }

        void HookManager_MouseMove(object sender, MouseEventArgs e)
        {
            Point pos = pnlPageContanier.PointToClient(e.Location);
            if (_pdfDoc != null)
            {
                if (MouseInPage(pos) && _bMouseCaptured)
                {
                    int offsetX;
                    int offsetY;

                    switch (getCursorStatus(e))
                    {
                        case CursorStatus.Move:
                            pnlPageContanier.Cursor = Cursors.Hand;
                            offsetX = e.X - _pointCurrent.X;
                            offsetY = e.Y - _pointCurrent.Y;
                            if (picPage.Width > pnlPageContanier.Width)
                            {
                                _pointX = -(pnlPageContanier.AutoScrollPosition.X);
                                _pointX += -offsetX;
                            }
                            if (picPage.Height > pnlPageContanier.Height)
                            {
                                _pointY = -(pnlPageContanier.AutoScrollPosition.Y);
                                _pointY += -offsetY;
                            }
                            pnlPageContanier.AutoScrollPosition = new Point(_pointX, _pointY);
                            break;
                        case CursorStatus.Zoom:
                            pnlPageContanier.Cursor = Cursors.SizeAll;
                            offsetX = e.X - _pointCurrent.X;
                            offsetY = e.Y - _pointCurrent.Y;
                            _pointCurrent = e.Location; //Update current Point
                            DrawRubberFrame();
                            break;
                    }
                }
                else if (MouseInPage(pos))
                {
                    //Check if we have the pointer on a link
                    if (getCursorStatus(e) == CursorStatus.Move)
                    {
                        PDFLibNet.PageLink link = SearchLink(e.Location);
                        if (link != null)
                        {
                            picPage.Cursor = Cursors.Hand;
                        }
                        else
                        {
                            picPage.Cursor = Cursors.Default;
                        }
                    }
                }
            }
            _pointCurrent = e.Location; //Update current Point
        }

        private PDFLibNet.PageLink SearchLink(Point location)
        {
            Point p = picPage.PointToClient(location);
            List<PageLink> links = _pdfDoc.GetLinks(_pdfDoc.CurrentPage);
            if (links != null)
            {
                //Search for a link
                foreach (PDFLibNet.PageLink pl in links)
                {
                    //Transform location
                    Point loc = Point.Ceiling(_pdfDoc.PointUserToDev(pl.Bounds.Location));
                    //Adjust size, 72dpi
                    Size siz = Size.Round(new SizeF(pl.Bounds.Size.Width * (float)_pdfDoc.RenderDPI / 72f, pl.Bounds.Size.Height * (float)_pdfDoc.RenderDPI / 72f));
                    //Translate
                    loc.Offset(0, -siz.Height);
                    Rectangle linkLoc = new Rectangle(loc, siz);
                    if (linkLoc.Contains(p))
                        //Link found!
                        return pl;
                }
            }
            return null;
        }

        void HookManager_MouseUp(object sender, MouseEventArgs e)
        {
            Point pos = pnlPageContanier.PointToClient(e.Location);
            if (MouseInPage(pos) && _bMouseCaptured)
            {
                switch (getCursorStatus(e))
                {
                    case CursorStatus.Move:
                        pnlPageContanier.AutoScrollPosition = new Point(_pointX, _pointY);
                        break;
                    case CursorStatus.Zoom:
                        if (!_pointCurrent.Equals(EmptyPoint))
                        {
                            if (e.Button == MouseButtons.Left && _pdfDoc!=null)
                                _pdfDoc.ZoomIN();
                            else if (e.Button == MouseButtons.Right && _pdfDoc!=null)
                                _pdfDoc.ZoomOut();
                        }
                        else
                        {
                            //Zoom on rectangle
                            
                        }    
                        break;
                }
                pnlPageContanier.Cursor = Cursors.Default;
            }
            ReleaseRubberFrame();
            _bMouseCaptured = false;
        }

        void HookManager_MouseDown(object sender, MouseEventArgs e)
        {
            Point pos = pnlPageContanier.PointToClient(e.Location);
            if (MouseInPage(pos) && e.Button == MouseButtons.Left)
            {
                PDFLibNet.PageLink link = SearchLink(e.Location);
                if (link != null)
                {
                    switch (link.Action.Kind)
                    {
                        case LinkActionKind.actionGoTo:

                            PDFLibNet.PageLinkGoTo plgo = (link.Action as PDFLibNet.PageLinkGoTo);
                            if (plgo.Destination != null)
                            {
                                _pdfDoc.CurrentPage = plgo.Destination.Page;
                                PointF loc = _pdfDoc.PointUserToDev(new PointF((float)plgo.Destination.Left, (float)plgo.Destination.Top));
                                if (plgo.Destination.ChangeTop)
                                    ScrolltoTop((int)loc.Y);
                                else
                                    ScrolltoTop(0);
                                _pdfDoc.RenderPage(picPage.Handle);
                                Render();
                            }
                            break;
                        case LinkActionKind.actionURI:
                            PDFLibNet.PageLinkURI uri = (link.Action as PDFLibNet.PageLinkURI);
                            if (MessageBox.Show("Launching external application" + Environment.NewLine + uri.URL, Text, MessageBoxButtons.OKCancel) == DialogResult.OK)
                            {
                                System.Diagnostics.Process p = new System.Diagnostics.Process();
                                p.StartInfo.FileName = GetDefaultBrowserPath();
                                p.StartInfo.Arguments = uri.URL;
                                p.Start();
                            }
                            break;
                    }
                }
                else
                {
                    _pointCurrent = e.Location;
                    _pointStart = e.Location;
                    _bMouseCaptured = true;
                }
            }
        }

        void HookManager_MouseWheel(object sender, MouseEventArgs e)
        {
            Point pos = pnlPageContanier.PointToClient(e.Location);
            if (MouseInPage(pos))
            {
                pnlPageContanier.Focus();

                int pointY=pnlPageContanier.AutoScrollPosition.Y;
                if (e.Delta < 0)
                    pointY = pnlPageContanier.AutoScrollPosition.Y - 120*picPage.Height / pnlPageContanier.Height / 6;
                else if(e.Delta>0)
                    pointY = pnlPageContanier.AutoScrollPosition.Y + 120*picPage.Height / pnlPageContanier.Height / 6;
                pnlPageContanier.AutoScrollPosition = new Point(-pnlPageContanier.AutoScrollPosition.X, -pointY);
                if (-pointY > pnlPageContanier.Height )
                {
                    if (_pdfDoc.CurrentPage < _pdfDoc.PageCount)
                    {
                        pnlPageContanier.AutoScrollPosition = new Point(0, 0);
                        _pdfDoc.NextPage();
                        _pdfDoc.RenderPage(picPage.Handle);
                        Render();
                    }
                }
                if (-pointY < 0 && _pdfDoc!=null)
                {
                    pnlPageContanier.AutoScrollPosition = new Point(0, 0);
                    _pdfDoc.PreviousPage();
                    _pdfDoc.RenderPage(picPage.Handle);
                    Render();
                }
                (e as Gma.UserActivityMonitor.MouseEventExtArgs).Handled = true;
            }
        }
        #endregion

        void frmPDFViewer_Resize(object sender, EventArgs e)
        {
            if (_pdfDoc != null)
            {
                FitWidth();
                Render();
            }
        }
        
        private void Render()
        {
            if (_pdfDoc != null)
            {
                if (_backbuffer != null)
                {
                    if (_pdfDoc.PageWidth != _backbuffer.Width || _pdfDoc.PageHeight != _backbuffer.Height)
                    {
                        picPage.Image = null;
                        _backbuffer.Dispose();
                        _backbuffer = null;
                    }
                }
                if (_backbuffer == null)
                    _backbuffer = new Bitmap(_pdfDoc.PageWidth, _pdfDoc.PageHeight);

                if (_pdfDoc.PageHeight != picPage.Height)
                    picPage.Height = _pdfDoc.PageHeight;
                if (_pdfDoc.PageWidth != picPage.Width)
                    picPage.Width = _pdfDoc.PageWidth;
                
                using (Graphics g = Graphics.FromImage(_backbuffer))
                {
                    IntPtr hdc = g.GetHdc();
                    _pdfDoc.DrawPageHDC(hdc);
                    g.ReleaseHdc();
                    /*List<PageLink> links = _pdfDoc.GetLinks(_pdfDoc.CurrentPage);
                    if (links != null)
                    {
                        foreach (PDFLibNet.PageLink pl in links)
                        {
                            //Transform location
                            Point loc = Point.Ceiling(_pdfDoc.PointUserToDev(pl.Bounds.Location));
                            //Adjust size, 72dpi
                            Size siz =Size.Round(new SizeF(pl.Bounds.Size.Width*(float)_pdfDoc.RenderDPI/72f,pl.Bounds.Size.Height*(float)_pdfDoc.RenderDPI/72f));
                            //Translate
                            loc.Offset(0, -siz.Height);

                            Rectangle linkLoc = new Rectangle(loc,siz);
                            g.DrawRectangle(Pens.Blue, linkLoc);
                        }
                    }*/
                }
                picPage.Image = _backbuffer;
                picPage.Refresh();
                txtPage.Text = string.Format("{0}/{1}", _pdfDoc.CurrentPage, _pdfDoc.PageCount);
            }
        }

        private void FitWidth()
        {
            picPage.SuspendLayout();
            using (PictureBox p = new PictureBox())
            {
                p.Width = pnlPageContanier.Width - pnlPageContanier.AutoScrollMargin.Width * 2;
                _pdfDoc.FitToWidth(p.Handle);
            }
            _pdfDoc.RenderPage(picPage.Handle);
            picPage.Width = _pdfDoc.PageWidth;
            picPage.ResumeLayout();
        }

        private void FitHeight()
        {
            using (PictureBox p = new PictureBox())
            {
                p.Width = pnlPageContanier.Width - pnlPageContanier.AutoScrollMargin.Width * 2;
                _pdfDoc.FitToHeight(p.Handle);
            }
            _pdfDoc.RenderPage(picPage.Handle);
            picPage.Height = _pdfDoc.PageHeight;
        }

        #region Outline
        private void FillTree()
        {
            tvwOutline.Nodes.Clear();
		    foreach (PDFLibNet.OutlineItem ol in _pdfDoc.Outline)
            {
                TreeNode tn = new TreeNode(ol.Title);
                tn.Tag = ol;
                if (ol.KidsCount > 0)
                    tn.Nodes.Add(new TreeNode("dummy"));
                tvwOutline.Nodes.Add(tn);
            }
        }

        void tvwOutline_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            OutlineItem ol = (OutlineItem)e.Node.Tag;
            if (ol != null)
            {
                ol.DoAction();
                switch (ol.GetKind())
                {
                    case LinkActionKind.actionGoTo:
                    case LinkActionKind.actionGoToR:
                        //Transform the coordinates
                        PointF loc = _pdfDoc.PointUserToDev(new PointF( (float)ol.Destination.Left,(float) ol.Destination.Top));
                        if(ol.Destination.ChangeTop)
                            ScrolltoTop((int)loc.Y);
                        else
                            ScrolltoTop(0);
                        break;
                    case LinkActionKind.actionLaunch:
                        break;
                    case LinkActionKind.actionMovie:
                        break;
                    case LinkActionKind.actionURI:
                        break;
                }
                FitWidth();
                Render();
            }
        }

        void tvwOutline_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {

             OutlineItem ol = (OutlineItem)e.Node.Tag;
             if (ol != null)
             {   
                 if (e.Node.Nodes.Count > 0 && e.Node.Nodes[0].Text == "dummy")
                 {
                     e.Node.Nodes.Clear();
                     foreach (OutlineItem col in ol.Childrens)
                     {
                         TreeNode tn = new TreeNode(col.Title);
                         tn.Tag = col;
                         if (col.KidsCount > 0)
                             tn.Nodes.Add(new TreeNode("dummy"));
                         e.Node.Nodes.Add(tn);
                     }
                 }
             }
         }
        #endregion

        private void txtPage_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                if (_pdfDoc != null && e.KeyCode == Keys.Return)
                {
                    int page=-1;
                    if (int.TryParse(txtPage.Text, out page))
                    {
                        if (page > 0 && page <= _pdfDoc.PageCount)
                        {
                            _pdfDoc.CurrentPage = page;
                            _pdfDoc.RenderPage(picPage.Handle);
                            Render();
                        }
                        else
                            page = -1;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error");
            }
        }

        private void tsbNext_Click(object sender, EventArgs e)
        {
            if (_pdfDoc != null)
            {
                _pdfDoc.NextPage();
                _pdfDoc.RenderPage(picPage.Handle);
                Render();
            }
        }

        private void tsbPrev_Click(object sender, EventArgs e)
        {
            if (_pdfDoc != null)
            {
               _pdfDoc.PreviousPage();
               _pdfDoc.RenderPage(picPage.Handle);
               Render();
            }
        }

        private void tsbOpen_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog dlg = new OpenFileDialog();
                dlg.Filter = "Portable Document Format (*.pdf)|*.pdf";
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    if (_pdfDoc == null)
                    {
                        _pdfDoc = new PDFWrapper();
                        _pdfDoc.PDFLoadCompeted += new PDFLoadCompletedHandler(_pdfDoc_PDFLoadCompeted);
                        _pdfDoc.PDFLoadBegin += new PDFLoadBeginHandler(_pdfDoc_PDFLoadBegin);
                    }
                    _pdfDoc.LoadPDF(dlg.FileName);
                    _pdfDoc.CurrentPage = 1;

                    Text = _pdfDoc.Author + " - " + _pdfDoc.Title;
                    FillTree();
                    FitWidth();
                    Render();

                    picPage.Visible = true;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());

            }
        }

        void _pdfDoc_PDFLoadBegin()
        {
            tvwOutline.BeforeExpand -= new TreeViewCancelEventHandler(tvwOutline_BeforeExpand);
            tvwOutline.NodeMouseClick -= new TreeNodeMouseClickEventHandler(tvwOutline_NodeMouseClick);
            Resize -= new EventHandler(frmPDFViewer_Resize);
            FormClosing -= new FormClosingEventHandler(frmPDFViewer_FormClosing);
            
            Gma.UserActivityMonitor.HookManager.MouseWheel -= new MouseEventHandler(HookManager_MouseWheel);
            Gma.UserActivityMonitor.HookManager.MouseDown -= new MouseEventHandler(HookManager_MouseDown);
            Gma.UserActivityMonitor.HookManager.MouseUp -= new MouseEventHandler(HookManager_MouseUp);
            Gma.UserActivityMonitor.HookManager.MouseMove -= new MouseEventHandler(HookManager_MouseMove);
            /*picPage.MouseWheel -= new MouseEventHandler(HookManager_MouseWheel);
            picPage.MouseDown -= new MouseEventHandler(HookManager_MouseDown);
            picPage.MouseUp -= new MouseEventHandler(HookManager_MouseUp);
            picPage.MouseMove -= new MouseEventHandler(HookManager_MouseMove);*/
        }

        void _pdfDoc_PDFLoadCompeted()
        {
            tvwOutline.BeforeExpand += new TreeViewCancelEventHandler(tvwOutline_BeforeExpand);
            tvwOutline.NodeMouseClick += new TreeNodeMouseClickEventHandler(tvwOutline_NodeMouseClick);
            Resize += new EventHandler(frmPDFViewer_Resize);
            FormClosing += new FormClosingEventHandler(frmPDFViewer_FormClosing);
            
            Gma.UserActivityMonitor.HookManager.MouseWheel += new MouseEventHandler(HookManager_MouseWheel);
            Gma.UserActivityMonitor.HookManager.MouseDown += new MouseEventHandler(HookManager_MouseDown);
            Gma.UserActivityMonitor.HookManager.MouseUp += new MouseEventHandler(HookManager_MouseUp);
            Gma.UserActivityMonitor.HookManager.MouseMove += new MouseEventHandler(HookManager_MouseMove);
            /*picPage.MouseWheel += new MouseEventHandler(HookManager_MouseWheel);
            picPage.MouseDown += new MouseEventHandler(HookManager_MouseDown);
            picPage.MouseUp += new MouseEventHandler(HookManager_MouseUp);
            picPage.MouseMove += new MouseEventHandler(HookManager_MouseMove);*/
        }

        private void tsbAbout_Click(object sender, EventArgs e)
        {
            frmAbout frm = new frmAbout();
            frm.ShowDialog();
        }

        #region Searching
        private int SearchCallBack(object sender, SearchArgs e)
        {
            int lFound=0;
            if (_pdfDoc != null)
            {
                _pdfDoc.SearchCaseSensitive = e.Exact;
                
                if (e.FromBegin)
                {
                    lFound = _pdfDoc.FindFirst(e.Text,
                    e.WholeDoc ? PDFLibNet.PDFSearchOrder.PDFSearchFromdBegin : PDFLibNet.PDFSearchOrder.PDFSearchFromCurrent,
                    e.Up);
                }
                else if (e.FindNext)
                {
                    
                    if (e.Up)
                        lFound = _pdfDoc.FindPrevious(e.Text);
                    else
                        lFound = _pdfDoc.FindNext(e.Text);
                     
                }
                else
                {
                    lFound = _pdfDoc.FindText(e.Text,
                        _pdfDoc.CurrentPage,
                        (e.WholeDoc ? PDFLibNet.PDFSearchOrder.PDFSearchFromdBegin : PDFLibNet.PDFSearchOrder.PDFSearchFromCurrent),
                        e.Exact,
                        e.Up,
                        true,
                        e.WholeDoc);    
                }
                if (lFound > 0)
                {
                    _pdfDoc.CurrentPage = _pdfDoc.SearchResults[0].Page;
                    _pdfDoc.RenderPage(picPage.Handle);
                    FocusSearchResult(_pdfDoc.SearchResults[0]);
                    Render();
                }
            }
            return lFound;
        }

        private void FocusSearchResult(PDFLibNet.PDFSearchResult res)
        {
            Point dr = pnlPageContanier.AutoScrollPosition;
            if (_pdfDoc.PageWidth > pnlPageContanier.Width)
                dr.X = res.Position.Left - pnlPageContanier.Width - pnlPageContanier.AutoScrollMargin.Width;
            if (_pdfDoc.PageHeight > pnlPageContanier.Height)
                dr.Y = res.Position.Top - pnlPageContanier.Height - pnlPageContanier.AutoScrollMargin.Height;
            pnlPageContanier.AutoScrollPosition = dr;

        }

        private void tsbSearch_Click(object sender, EventArgs e)
        {
            try
            {
                frmSearch frm = new frmSearch(SearchCallBack);
                frm.ShowDialog();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }
        }

        #endregion

        private void ScrolltoTop(int y)
        {
            Point dr = pnlPageContanier.AutoScrollPosition;
            if (_pdfDoc.PageHeight > pnlPageContanier.Height)
                dr.Y = y;
            pnlPageContanier.AutoScrollPosition = dr;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            try
            {
                string fileName = System.IO.Path.GetTempPath() + Guid.NewGuid().ToString() + ".ps";

                if (printDialog1.ShowDialog() == DialogResult.OK)
                {
                    if (printDialog1.PrinterSettings.PrintToFile)
                    {
                        saveFileDialog1.Filter = "PostScript File (*.ps)|*.ps";
                        if (this.saveFileDialog1.ShowDialog() == DialogResult.OK)
                            _pdfDoc.PrintToFile(saveFileDialog1.FileName, 1, _pdfDoc.PageCount);
                    }
                    else
                    {
                        _pdfDoc.PrintToFile(fileName, 1, _pdfDoc.PageCount);

                        RawPrinterHelper.SendFileToPrinter(printDialog1.PrinterSettings.PrinterName, fileName);
                    }
                }
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }
        }

        private void toolStripButton1_Click_1(object sender, EventArgs e)
        {
            try
            {
                if (_pdfDoc != null)
                {
                    _pdfDoc.ZoomIN();
                    _pdfDoc.RenderPage(picPage.Handle);
                    Render();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            if (_pdfDoc != null)
            {
                _pdfDoc.ZoomOut();
                _pdfDoc.RenderPage(picPage.Handle);
                Render();
            }
        }

        #region Rubber Frame
        Point EmptyPoint = new Point(-1, -1);
        Point _lastPoint = new Point(-1, -1);
        private void DrawRubberFrame()
        {
            if (picPage.Image != null)
            {
                if (!_lastPoint.Equals(EmptyPoint) ||
                    (_bMouseCaptured && !_pointCurrent.Equals(EmptyPoint))
                )

                    if (!_lastPoint.Equals(EmptyPoint))
                    {
                        ReleaseRubberFrame();
                    }
                _lastPoint = _pointCurrent;
                DrawRubberFrame(_pointStart, _pointCurrent);
            }
        }
        private void ReleaseRubberFrame()
        {
            if (!_lastPoint.Equals(EmptyPoint))
            {
                DrawRubberFrame(_pointStart, _lastPoint);
            }
            _lastPoint = EmptyPoint;
        }
        private void DrawRubberFrame(Point p1, Point p2)
        {
            Rectangle rc = new Rectangle();

            // Convert the points to screen coordinates.
            //p1 = PointToScreen(p1);
            //p2 = PointToScreen(p2);
            // Normalize the rectangle.
            if (p1.X < p2.X)
            {
                rc.X = p1.X;
                rc.Width = p2.X - p1.X;
            }
            else
            {
                rc.X = p2.X;
                rc.Width = p1.X - p2.X;
            }
            if (p1.Y < p2.Y)
            {
                rc.Y = p1.Y;
                rc.Height = p2.Y - p1.Y;
            }
            else
            {
                rc.Y = p2.Y;
                rc.Height = p1.Y - p2.Y;
            }
            // Draw the reversible frame.
            ControlPaint.DrawReversibleFrame(rc,
                            Color.Gray, FrameStyle.Dashed);

        }
        #endregion
        private static string GetDefaultBrowserPath()
        {
            string key = @"htmlfile\shell\open\command";
            Microsoft.Win32.RegistryKey registryKey =
            Microsoft.Win32.Registry.ClassesRoot.OpenSubKey(key, false);
            // get default browser path
            return ((string)registryKey.GetValue(null, null)).Split('"')[1];

        }
    }

    
    public delegate int SearchPdfHandler(object sender, SearchArgs e);

    public class SearchArgs : EventArgs
    {
        public string Text;
        public bool FromBegin;
        public bool Exact;
        public bool WholeDoc;
        public bool FindNext;
        public bool Up;

        internal SearchArgs(string text, bool frombegin, bool exact, bool wholedoc, bool findnext, bool up)
        {
            Text = text;
            FromBegin = frombegin;
            Exact = exact;
            WholeDoc = wholedoc;
            FindNext = findnext;
            Up = up;
        }
    }

    

}