using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Configuration;
using PDFLibNet;
using System.Management;


namespace PDFViewer
{
  
    public partial class frmPDFViewer : Form
    {
        public delegate void RenderNotifyInvoker(int page, bool isCurrent);

        [DllImport("user32.dll")]
        static extern int GetForegroundWindow();

        public static frmPDFViewer Instance;
        
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
        public frmPDFViewer()
        {
            InitializeComponent();
            //Update path to xpdfrc
            if (ConfigurationManager.AppSettings.Get("xpdfrc") == "xpdfrc")
            {
                // Open App.Config of executable
                Configuration config = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                // Add an Application Setting.
                config.AppSettings.Settings.Remove("xpdfrc");
                config.AppSettings.Settings.Add("xpdfrc", AppDomain.CurrentDomain.BaseDirectory + "xpdfrc");
                // Save the configuration file.
                config.Save(ConfigurationSaveMode.Modified);
                // Force a reload of a changed section.
                ConfigurationManager.RefreshSection("appSettings");
            }
            pageViewControl1.PageSize = new Size(pageViewControl1.Width,(int)( pageViewControl1.Width*11/8.5));
            pageViewControl1.Visible = true;
            Instance = this;
        }

        void frmPDFViewer_FormClosing(object sender, FormClosingEventArgs e)
        {
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
                return pageViewControl1.MouseInPage(p);
            }
            return false;
        }

        void HookManager_MouseMove(object sender, MouseEventArgs e)
        {
            Point pos = pageViewControl1.PointToClient(e.Location);
            if (_pdfDoc != null)
            {
                if (MouseInPage(pos) && _bMouseCaptured)
                {
                    //Handled by the control
                }
                else if (MouseInPage(pos))
                {
                    //Check if we have the pointer on a link
                    if (getCursorStatus(e) == CursorStatus.Move)
                    {
                        PDFLibNet.PageLink link = SearchLink(e.Location);
                        if (link != null)
                        {
                            pageViewControl1.Cursor = Cursors.Hand;
                        }
                        else
                        {
                            pos = pageViewControl1.PointUserToPage(pos);
                            if (!_pdfDoc.IsBusy && 
                                _pdfDoc.Pages[_pdfDoc.CurrentPage].HasText(pos.X, pos.Y))
                            {
                                //String txt = _pdfDoc.Pages[_pdfDoc.CurrentPage].Text;
                                pageViewControl1.Cursor = Cursors.IBeam;
                            }
                            else
                                pageViewControl1.Cursor = Cursors.Default;
                        }
                    }
                }
            }
            _pointCurrent = e.Location; //Update current Point
        }

        private PDFLibNet.PageLink SearchLink(Point location)
        {
            Point p = pageViewControl1.PointToClient(location);
            List<PageLink> links = _pdfDoc.GetLinks(_pdfDoc.CurrentPage);
            if (links != null)
            {
                //Search for a link
                foreach (PDFLibNet.PageLink pl in links)
                {
                    //Convert coordinates
                    Point p1 = Point.Ceiling(_pdfDoc.PointUserToDev(new PointF(pl.Bounds.Left, pl.Bounds.Top)));
                    Point p2 = Point.Ceiling(_pdfDoc.PointUserToDev(new PointF(pl.Bounds.Right, pl.Bounds.Bottom)));
                    Rectangle linkLoc = new Rectangle(p1.X, p1.Y, p2.X - p1.X, p1.Y - p2.Y);
                    //Translate
                    linkLoc.Offset(-pageViewControl1.CurrentView.X, -pageViewControl1.CurrentView.Y);
                    linkLoc.Offset(pageViewControl1.PageBounds.X, pageViewControl1.PageBounds.Y);
                    linkLoc.Offset(0, p2.Y - p1.Y);
                    if(linkLoc.Contains(p))
                        //Link found!
                        return pl;
                }
            }
            return null;
        }

        void HookManager_MouseUp(object sender, MouseEventArgs e)
        {
            Point pos = pageViewControl1.PointToClient(e.Location);
            if (MouseInPage(pos) && _bMouseCaptured)
            {
                switch (getCursorStatus(e))
                {
                    case CursorStatus.Move:
                        
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
                pageViewControl1.Cursor = Cursors.Default;
            }
            ReleaseRubberFrame();
            _bMouseCaptured = false;
        }

        void HookManager_MouseDown(object sender, MouseEventArgs e)
        {
            Point pos = pageViewControl1.PointToClient(e.Location);
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
                                _pdfDoc.RenderPage(pageViewControl1.Handle);
                                Render();
                            }
                            else if (plgo.DestinationName != null)
                            {
                                
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

    
        #endregion

        void frmPDFViewer_Resize(object sender, EventArgs e)
        {
            if (_pdfDoc != null)
            {
                using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusResizing))
                {
                    FitWidth();
                    Render();
                }
            }
        }
        
        private void Render()
        {
            pageViewControl1.PageSize = new Size(_pdfDoc.PageWidth, _pdfDoc.PageHeight);
            txtPage.Text = string.Format("{0}/{1}", _pdfDoc.CurrentPage, _pdfDoc.PageCount);
        }

        private void FitWidth()
        {
            if (_pdfDoc != null && _pdfDoc.CurrentPage > 0)
            {
                using (PictureBox p = new PictureBox())
                {
                    p.Width = pageViewControl1.ClientSize.Width;
                    _pdfDoc.FitToWidth(p.Handle);
                }
                _pdfDoc.RenderFinished -= new RenderFinishedHandler(_pdfDoc_RenderFinished);
                _pdfDoc.RenderFinished += new RenderFinishedHandler(_pdfDoc_RenderFinished);
                _pdfDoc.RenderPageThread(pageViewControl1.Handle,true);
            }
        }

        private void FitHeight()
        {
            if (_pdfDoc != null && _pdfDoc.CurrentPage > 0)
            {
                using (PictureBox p = new PictureBox())
                {
                    p.Width = pageViewControl1.ClientSize.Height;
                    _pdfDoc.FitToHeight(p.Handle);
                }
                _pdfDoc.RenderFinished -= new RenderFinishedHandler(_pdfDoc_RenderFinished);
                _pdfDoc.RenderFinished += new RenderFinishedHandler(_pdfDoc_RenderFinished);
                _pdfDoc.RenderPageThread(pageViewControl1.Handle, false);
            }
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
            using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusSearchingBookmark))
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
                            PointF loc = _pdfDoc.PointUserToDev(new PointF((float)ol.Destination.Left, (float)ol.Destination.Top));
                            if (ol.Destination.ChangeTop)
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
                    _pdfDoc.RenderPage(pageViewControl1.Handle);
                    Render();
                }
            }
        }

        void tvwOutline_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusSearchingBookmark))
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
         }
        #endregion

        private void txtPage_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
                {
                    if (_pdfDoc != null && e.KeyCode == Keys.Return)
                    {
                        int page = -1;
                        if (int.TryParse(txtPage.Text, out page))
                        {
                            if (page > 0 && page <= _pdfDoc.PageCount)
                            {
                                _pdfDoc.CurrentPage = page;
                                UpdateText();
                                _pdfDoc.RenderPage(pageViewControl1.Handle);
                                Render();
                            }
                            else
                                page = -1;
                        }
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
            using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
            {
                if (_pdfDoc != null)
                {
                    _pdfDoc.NextPage();
                    UpdateText();
                    _pdfDoc.RenderPage(pageViewControl1.Handle);
                    Render();
                }
            }
        }

        private void tsbPrev_Click(object sender, EventArgs e)
        {
            using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
            {
                if (_pdfDoc != null && !IsDisposed)
                {
                    _pdfDoc.PreviousPage();
                    UpdateText();
                    _pdfDoc.RenderPage(pageViewControl1.Handle);
                    Render();
                }
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
                        _pdfDoc.RenderNotifyFinished += new RenderNotifyFinishedHandler(_pdfDoc_RenderNotifyFinished);
                        _pdfDoc.PDFLoadCompeted += new PDFLoadCompletedHandler(_pdfDoc_PDFLoadCompeted);
                        _pdfDoc.PDFLoadBegin += new PDFLoadBeginHandler(_pdfDoc_PDFLoadBegin);
                    }
                    xPDFParams.ErrorQuiet = false;
                    xPDFParams.ErrorFile = "C:\\stderr.log";
                    //}

                    using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingFile))
                    {
                        if (LoadFile(dlg.FileName, _pdfDoc))
                        {
                            Text = "Powered by xPDF: " + _pdfDoc.Author + " - " + _pdfDoc.Title;
                            FillTree();
                            _pdfDoc.CurrentPage = 1;
                            UpdateText();

                            _pdfDoc.FitToWidth(pageViewControl1.Handle);
                            _pdfDoc.RenderPage(pageViewControl1.Handle);

                            Render();
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        void RenderNotifyFinished(int page, bool isCurrent)
        {
            if (tabView.SelectedTab.Equals(this.tpvText) && !_pdfDoc.IsBusy)
                txtTextView.Text = _pdfDoc.Pages[_pdfDoc.CurrentPage].Text;
        }
        void _pdfDoc_RenderNotifyFinished(int page, bool isCurrent)
        {    
         
            Invoke(new RenderNotifyInvoker(RenderNotifyFinished),page, isCurrent);
        }
        
        private bool LoadFile(string filename, PDFLibNet.PDFWrapper pdfDoc)
        {
            try
            {
                return pdfDoc.LoadPDF(filename);
            }
            catch (System.Security.SecurityException)
            {
                 frmPassword frm = new frmPassword();
                 if (frm.ShowDialog() == DialogResult.OK)
                 {
                     if (!frm.UserPassword.Equals(String.Empty))
                         pdfDoc.UserPassword = frm.UserPassword;
                     if (!frm.OwnerPassword.Equals(String.Empty))
                         pdfDoc.OwnerPassword = frm.OwnerPassword;
                     return LoadFile(filename, pdfDoc);
                 }
                 else
                 {
                     MessageBox.Show("File encrypted",Text);
                     return false;
                 }
            }
        }

        void _pdfDoc_PDFLoadBegin()
        {
            UpdateParamsUI(false);

            tvwOutline.BeforeExpand -= new TreeViewCancelEventHandler(tvwOutline_BeforeExpand);
            tvwOutline.NodeMouseClick -= new TreeNodeMouseClickEventHandler(tvwOutline_NodeMouseClick);
            Resize -= new EventHandler(frmPDFViewer_Resize);
            FormClosing -= new FormClosingEventHandler(frmPDFViewer_FormClosing);
            
            Gma.UserActivityMonitor.HookManager.MouseDown -= new MouseEventHandler(HookManager_MouseDown);
            Gma.UserActivityMonitor.HookManager.MouseUp -= new MouseEventHandler(HookManager_MouseUp);
            Gma.UserActivityMonitor.HookManager.MouseMove -= new MouseEventHandler(HookManager_MouseMove);
           
        }

        void _pdfDoc_PDFLoadCompeted()
        {
            tvwOutline.BeforeExpand += new TreeViewCancelEventHandler(tvwOutline_BeforeExpand);
            tvwOutline.NodeMouseClick += new TreeNodeMouseClickEventHandler(tvwOutline_NodeMouseClick);
            Resize += new EventHandler(frmPDFViewer_Resize);
            FormClosing += new FormClosingEventHandler(frmPDFViewer_FormClosing);
            
            Gma.UserActivityMonitor.HookManager.MouseDown += new MouseEventHandler(HookManager_MouseDown);
            Gma.UserActivityMonitor.HookManager.MouseUp += new MouseEventHandler(HookManager_MouseUp);
            Gma.UserActivityMonitor.HookManager.MouseMove += new MouseEventHandler(HookManager_MouseMove);

            UpdateParamsUI();
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
                PDFLibNet.xPDFParams.ErrorQuiet = false;
                PDFLibNet.xPDFParams.ErrorFile="C:\\errorstd.log";
                _pdfDoc.SearchCaseSensitive = e.Exact;
                
                if (e.FromBegin)
                {
                    lFound = _pdfDoc.FindFirst(e.Text,
                    e.WholeDoc ? PDFLibNet.PDFSearchOrder.PDFSearchFromdBegin : PDFLibNet.PDFSearchOrder.PDFSearchFromCurrent,
                    e.Up,
                    e.WholeWord);
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
                        e.WholeWord);    
                }
                if (lFound > 0)
                {
                    _pdfDoc.CurrentPage = _pdfDoc.SearchResults[0].Page;
                    UpdateText();
                    _pdfDoc.RenderPage(pageViewControl1.Handle);
                    FocusSearchResult(_pdfDoc.SearchResults[0]);
                    Render();
                }
            }
            return lFound;
        }

        private void FocusSearchResult(PDFLibNet.PDFSearchResult res)
        {
            Point loc = res.Position.Location; 
            Point dr = pageViewControl1.ScrollPosition;
            if (_pdfDoc.PageWidth > pageViewControl1.Width)
                dr.X =(int)( loc.X * _pdfDoc.RenderDPI / 72) -pageViewControl1.Width ;
            if (_pdfDoc.PageHeight > pageViewControl1.Height)
                dr.Y =(int) ( loc.Y * _pdfDoc.RenderDPI / 72) -pageViewControl1.Height;
            pageViewControl1.ScrollPosition = dr;

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
            Point dr = this.pageViewControl1.ScrollPosition;
            if (_pdfDoc.PageHeight > pageViewControl1.Height)
                dr.Y = y;
            pageViewControl1.ScrollPosition = dr;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            try
            {
                string fileName = System.IO.Path.GetTempPath() + Guid.NewGuid().ToString() + ".ps";

                if (printDialog1.ShowDialog() == DialogResult.OK)
                {
                    using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusPrinting))
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
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);

            }
        }
        private void tsbZoomIn_Click(object sender, EventArgs e)
        {
            try
            {
                using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
                {
                    if (_pdfDoc != null)
                    {
                        _pdfDoc.ZoomIN();
                        _pdfDoc.RenderFinished -= new RenderFinishedHandler(_pdfDoc_RenderFinished);
                        _pdfDoc.RenderFinished += new RenderFinishedHandler(_pdfDoc_RenderFinished);
                        _pdfDoc.RenderPageThread(pageViewControl1.Handle, false);
                        Render();
                    }
                }
            }
            catch (Exception) { }
            
        }

        void _pdfDoc_RenderFinished()
        {
            try
            {
                Invoke(new frmExportJpg.FinishedInvoker(Render));
            }
            catch (Exception) { }
        }

        private void tsbZoomOut_Click(object sender, EventArgs e)
        {
            try
            {
                using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
                {
                    if (_pdfDoc != null)
                    {
                        _pdfDoc.ZoomOut();
                        _pdfDoc.RenderFinished -= new RenderFinishedHandler(_pdfDoc_RenderFinished);
                        _pdfDoc.RenderFinished += new RenderFinishedHandler(_pdfDoc_RenderFinished);
                        _pdfDoc.RenderPageThread(pageViewControl1.Handle, false);
                        Render();
                    }
                }
            }
            catch (Exception) { }
        }

        #region Rubber Frame
        Point EmptyPoint = new Point(-1, -1);
        Point _lastPoint = new Point(-1, -1);
        private void DrawRubberFrame()
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

        private void tsbPrintAs_Click(object sender, EventArgs e)
        {
            try
            {
                saveFileDialog1.Filter = "PostScript file (*.ps)|*.ps|Plain text (*.txt)|*.txt|HTML Markup(*.html)|*.html|Jpg Image (*.jpg)|*.jpg";
                if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusExporting))
                    {
                        if (saveFileDialog1.FileName.EndsWith(".ps"))
                        {
                            _pdfDoc.PrintToFile(saveFileDialog1.FileName, 1, _pdfDoc.PageCount);
                        }
                        else if (saveFileDialog1.FileName.EndsWith(".jpg"))
                        {
                            _pdfDoc.ExportJpgProgress += new ExportJpgProgressHandler(_pdfDoc_ExportJpgProgress);
                            _pdfDoc.ExportJpgFinished += new ExportJpgFinishedHandler(_pdfDoc_ExportJpgFinished);
                            frmExportJpg frm = new frmExportJpg(_pdfDoc, saveFileDialog1.FileName);
                            frm.Show();
                        }
                        else if (saveFileDialog1.FileName.EndsWith(".txt"))
                        {
                            _pdfDoc.ExportText(saveFileDialog1.FileName, 1, _pdfDoc.PageCount, true, true);
                        }
                        else if (saveFileDialog1.FileName.EndsWith(".html"))
                        {
                            //_pdfDoc.ExportHtml(saveFileDialog1.FileName, 1, _pdfDoc.PageCount,false,false,true);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                _pdfDoc.ExportJpgProgress -= new ExportJpgProgressHandler(_pdfDoc_ExportJpgProgress);
                _pdfDoc.ExportJpgFinished -= new ExportJpgFinishedHandler(_pdfDoc_ExportJpgFinished);
                MessageBox.Show(ex.ToString());
            }
        }

        void _pdfDoc_ExportJpgFinished()
        {
            _pdfDoc.ExportJpgProgress -= new ExportJpgProgressHandler(_pdfDoc_ExportJpgProgress);
            _pdfDoc.ExportJpgFinished -= new ExportJpgFinishedHandler(_pdfDoc_ExportJpgFinished);
            StatusLabel.Text ="Ready";
        }

        bool _pdfDoc_ExportJpgProgress(int pageCount, int currentPage)
        {
            StatusLabel.Text = "Exportando pagina " + currentPage.ToString() + " de " + pageCount.ToString()  + "...";
            return true;
        }

        private void doubleBufferControl1_PaintControl(object sender,Rectangle view, Point location, Graphics g)
        {
            if (_pdfDoc != null)
            {
                Size sF= new Size(view.Right,view.Bottom);
                Rectangle r = new Rectangle(location, sF);
                _pdfDoc.ClientBounds = r;
                _pdfDoc.CurrentX = view.X;
                _pdfDoc.CurrentY = view.Y;
                _pdfDoc.DrawPageHDC(g.GetHdc());
                g.ReleaseHdc();
                /*
                if (_pdfDoc.RenderDPI >= g.DpiX)
                {
                    foreach (PageLink pl in _pdfDoc.GetLinks(_pdfDoc.CurrentPage))
                    { 
                        //Convert coordinates
                        Point p1 =  Point.Ceiling(_pdfDoc.PointUserToDev(new PointF(pl.Bounds.Left,pl.Bounds.Top)));
                        Point p2 = Point.Ceiling(_pdfDoc.PointUserToDev(new PointF(pl.Bounds.Right, pl.Bounds.Bottom)));
                        Rectangle linkLoc = new Rectangle(p1.X, p1.Y, p2.X - p1.X, p1.Y-p2.Y);
                        //Translate
                        linkLoc.Offset(-view.X, -view.Y);
                        linkLoc.Offset(r.X, r.Y);
                        linkLoc.Offset(0, p2.Y - p1.Y);
                        //Draw Rectangle
                        g.DrawRectangle(Pens.Blue, linkLoc);
                    }
                }*/
 
            }
        }

        private bool doubleBufferControl1_NextPage(object sender)
        {
            try
            {
                using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
                {
                    if (_pdfDoc.CurrentPage < _pdfDoc.PageCount)
                    {
                        UpdateText();
                        _pdfDoc.NextPage();
                        _pdfDoc.RenderPage(pageViewControl1.Handle);
                        Render();
                        return true;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
            return false;
        }

        private bool doubleBufferControl1_PreviousPage(object sender)
        {
            try
            {
                using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
                {
                    if (_pdfDoc.CurrentPage > 1)
                    {
                        _pdfDoc.PreviousPage();
                        UpdateText();
                        _pdfDoc.RenderPage(pageViewControl1.Handle);
                        Render();
                        return true;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
            return false;
        }

        private void tsbAntialias_Click(object sender, EventArgs e)
        {
            using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
            {
                if (_pdfDoc != null)
                {
                    PDFLibNet.xPDFParams.Antialias = !PDFLibNet.xPDFParams.Antialias;
                    tsbAntialias.Checked = PDFLibNet.xPDFParams.Antialias;
                    _pdfDoc.RenderPage(pageViewControl1.Handle, true);
                    Render();
                    pageViewControl1.Invalidate();
                }
            }
        }

        private void tsbVectorAntialias_Click(object sender, EventArgs e)
        {
            using (StatusBusy sb = new StatusBusy(Resources.UIStrings.StatusLoadingPage))
            {
                if (_pdfDoc != null)
                {
                    PDFLibNet.xPDFParams.VectorAntialias = !PDFLibNet.xPDFParams.VectorAntialias;
                    tsbVectorAntialias.Checked = PDFLibNet.xPDFParams.VectorAntialias;
                    _pdfDoc.RenderPage(pageViewControl1.Handle, true);
                    Render();
                    pageViewControl1.Invalidate();
                }
            }
        }

        private void UpdateParamsUI()
        {
            UpdateParamsUI(true);
        }
        private void UpdateParamsUI(bool enabled)
        {
            tsbAntialias.Enabled = _pdfDoc != null && enabled; 
            tsbVectorAntialias.Enabled = _pdfDoc != null && enabled;

            tsbAntialias.Checked = PDFLibNet.xPDFParams.Antialias;
            tsbVectorAntialias.Checked = PDFLibNet.xPDFParams.VectorAntialias;
        }

        private void tabView_Selected(object sender, TabControlEventArgs e)
        {
            UpdateText();
        }

        private void UpdateText()
        {
            //Extract Text from Current Page
            if (tabView.SelectedTab.Equals(this.tpvText) && _pdfDoc != null && !_pdfDoc.IsBusy)
            {
                txtTextView.Text = _pdfDoc.Pages[_pdfDoc.CurrentPage].Text;
            }
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
        public bool WholeWord;

        internal SearchArgs(string text, bool frombegin, bool exact, bool wholedoc, bool findnext, bool up, bool wholeword)
        {
            Text = text;
            FromBegin = frombegin;
            Exact = exact;
            WholeDoc = wholedoc;
            FindNext = findnext;
            WholeWord = wholeword;
            Up = up;
        }
    }

    

}