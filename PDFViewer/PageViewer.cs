using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace PDFViewer
{
    public class PageViewer : UserControl
    {
        public delegate void PaintControlHandler(object sender,Rectangle view, Point location, Graphics g);
        public delegate bool MovePageHandler(object sender);
        public event MovePageHandler NextPage;
        public event MovePageHandler PreviousPage;

        bool _drawShadow;
        bool _drawRect;
        Color _bgColor;
        Color _rectColor;

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

        #region Bounds
        private bool InvalidateScrollBarChanged = true;
        private Size _viewSize;
        private Rectangle _viewBounds;

        private Size _clientSize;
        private Rectangle _clientBounds;
        private Point _clientLocation;

        private Size _pageSize;
        private Rectangle _pageBounds;
        private Point _pageLocation;

        private Rectangle _currentView;
        private Size _scrollbarSize;
        #endregion

        #region ctors
        public PageViewer()
        {
            InitializeComponent();

            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint, true);
            Application.ApplicationExit += new EventHandler(MemoryCleanup);

            _scrollbarSize = new Size(vsb.Width, hsb.Height);
            _clientLocation = new Point(Margin.Left, Margin.Top);
            _bgColor = BackColor;
            _rectColor = Color.Black;
        }
        #endregion

        public Point ClientToPage(Point p)
        {
            Point pp = new Point(p.X + Margin.Left + ScrollPosition.X, p.Y + Margin.Top + ScrollPosition.Y);
            return pp;
        }
        
        public Point PointUserToPage(Point p)
        {
            /*
            int offsetX=0;
            int offsetY=0;

            if(PageBounds.Y>=0)
                offsetY = -(2*Margin.Top-PageBounds.Y);
            else 
                offsetY = 0;
            if (PageBounds.X >= 0)
                offsetX = -(2 * Margin.Left - PageBounds.X);
            else
                offsetX = 0;

            if (PageBounds.Height <= ViewSize.Height)
                offsetY = PageBounds.Y-Margin.Top;
            if (PageBounds.Width <= ViewSize.Width)
                offsetX = PageBounds.X-Margin.Left;
            */
            Point copy = p;
            
            copy.Offset(-PageBounds.X, -PageBounds.Y);
            copy.Offset(CurrentView.X, CurrentView.Y);
            /*if(PageBounds.Left>Margin.Left)
                copy.Offset(-Margin.Left,0);
            if (PageBounds.Top > Margin.Top)
                copy.Offset(0, -Margin.Top);*/
            return copy;
        }

        public bool DrawShadow {
            get
            {
                return _drawShadow;
            }
            set
            {
                _drawShadow = value;
                Invalidate();
            }
        }
        public bool DrawBorder
        {
            get
            {
                return _drawRect;
            }
            set
            {
                _drawRect = value;
                Invalidate();
            }
        }
        public Color PageColor
        {
            get
            {
                return _bgColor;
            }

            set
            {
                _bgColor = value;
                Invalidate();
            }
        }
        public Color BorderColor
        {
            get
            {
                return _rectColor;
            }
            set
            {
                _rectColor = value;
                Invalidate();
            }

        }

        #region DoubleBuffer providers
        const Graphics NO_BUFFER_GRAPHICS = null;
        const Bitmap NO_BACK_BUFFER = null;
        const BufferedGraphics NO_MANAGED_BACK_BUFFER = null;

        Bitmap BackBuffer;
        Graphics BufferGraphics;
        BufferedGraphicsContext GraphicManager;
        BufferedGraphics ManagedBackBuffer;

        DoubleBufferMethod _PaintMethod = DoubleBufferMethod.BuiltInOptimizedDoubleBuffer;
        private HScrollBar hsb;
        private VScrollBar vsb;

        public event PaintControlHandler PaintControl;

        private void InitializeComponent()
        {
            this.hsb = new System.Windows.Forms.HScrollBar();
            this.vsb = new System.Windows.Forms.VScrollBar();
            this.SuspendLayout();
            // 
            // hsb
            // 
            this.hsb.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.hsb.Location = new System.Drawing.Point(0, 402);
            this.hsb.Name = "hsb";
            this.hsb.Size = new System.Drawing.Size(361, 17);
            this.hsb.TabIndex = 0;
            this.hsb.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hsb_Scroll);
            this.hsb.Resize += new System.EventHandler(this.vsb_Resize);
            // 
            // vsb
            // 
            this.vsb.Dock = System.Windows.Forms.DockStyle.Right;
            this.vsb.Location = new System.Drawing.Point(344, 0);
            this.vsb.Name = "vsb";
            this.vsb.Size = new System.Drawing.Size(17, 402);
            this.vsb.TabIndex = 1;
            this.vsb.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vScrollBar1_Scroll);
            this.vsb.Resize += new System.EventHandler(this.vsb_Resize);
            // 
            // DoubleBufferControl
            // 
            this.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.Controls.Add(this.vsb);
            this.Controls.Add(this.hsb);
            this.Margin = new System.Windows.Forms.Padding(10);
            this.Name = "DoubleBufferControl";
            this.Size = new System.Drawing.Size(361, 419);
            this.MarginChanged += new System.EventHandler(this.DoubleBufferControl_MarginChanged);
            this.Resize += new System.EventHandler(this.DoubleBufferControl_Resize);
            this.ResumeLayout(false);

        }

        public DoubleBufferMethod PaintMethod
        {
            get { return _PaintMethod; }
            set
            {
                _PaintMethod = value;
                RemovePaintMethods();

                switch (value)
                {
                    case DoubleBufferMethod.BuiltInDoubleBuffer:
                        this.SetStyle(ControlStyles.UserPaint, true);
                        this.DoubleBuffered = true;
                        break;
                    case DoubleBufferMethod.BuiltInOptimizedDoubleBuffer:
                        this.SetStyle(
                            ControlStyles.OptimizedDoubleBuffer |
                            ControlStyles.AllPaintingInWmPaint, true);
                        break;
                    case DoubleBufferMethod.ManualDoubleBuffer11:
                        this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
                        BackBuffer = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
                        BufferGraphics = Graphics.FromImage(BackBuffer);
                        break;

                    case DoubleBufferMethod.ManualDoubleBuffer20:
                        this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
                        GraphicManager = BufferedGraphicsManager.Current;
                        GraphicManager.MaximumBuffer = new Size(this.Width + 1, this.Height + 1);
                        ManagedBackBuffer = GraphicManager.Allocate(this.CreateGraphics(), ClientRectangle);
                        break;
                }
            }
        }

        public enum DoubleBufferMethod
        {
            NoDoubleBuffer,
            BuiltInDoubleBuffer,
            BuiltInOptimizedDoubleBuffer,
            ManualDoubleBuffer11,
            ManualDoubleBuffer20
        };

        private void MemoryCleanup(object sender, EventArgs e)
        {
            if (BufferGraphics != NO_BUFFER_GRAPHICS)
                BufferGraphics.Dispose();

            if (BackBuffer != NO_BACK_BUFFER)
                BackBuffer.Dispose();

            if (ManagedBackBuffer != NO_MANAGED_BACK_BUFFER)
                ManagedBackBuffer.Dispose();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (DesignMode) { base.OnPaint(e); return; }
            switch (_PaintMethod)
            {
                case DoubleBufferMethod.NoDoubleBuffer:
                    base.OnPaint(e);
                    Render(e.Graphics);
                    break;

                case DoubleBufferMethod.BuiltInDoubleBuffer:
                    Render(e.Graphics);
                    break;

                case DoubleBufferMethod.BuiltInOptimizedDoubleBuffer:
                    Render(e.Graphics);
                    break;

                case DoubleBufferMethod.ManualDoubleBuffer20:
                    PaintDoubleBuffer20(e.Graphics); break;
            }
        }

        private void RemovePaintMethods()
        {
            this.DoubleBuffered = false;

            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, false);

            if (BufferGraphics != NO_BUFFER_GRAPHICS)
            {
                BufferGraphics.Dispose();
                BufferGraphics = NO_BUFFER_GRAPHICS;
            }

            if (BackBuffer != NO_BACK_BUFFER)
            {
                BackBuffer.Dispose();
                BackBuffer = NO_BACK_BUFFER;
            }

            if (ManagedBackBuffer != NO_MANAGED_BACK_BUFFER)
                ManagedBackBuffer.Dispose();
        }

        private void DoubleBufferControl_Resize(object sender, EventArgs e)
        {
            Resized();
            switch (_PaintMethod)
            {
                case DoubleBufferMethod.ManualDoubleBuffer11:
                    BackBuffer = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
                    BufferGraphics = Graphics.FromImage(BackBuffer);
                    break;

                case DoubleBufferMethod.ManualDoubleBuffer20:
                    GraphicManager.MaximumBuffer = new Size(this.Width + 1, this.Height + 1);

                    if (ManagedBackBuffer != NO_MANAGED_BACK_BUFFER)
                        ManagedBackBuffer.Dispose();

                    ManagedBackBuffer = GraphicManager.Allocate(this.CreateGraphics(), ClientRectangle);
                    break;
            }
            
            this.Refresh();
        }

        private void PaintDoubleBuffer20(Graphics ControlGraphics)
        {
            try
            {
                Render(ManagedBackBuffer.Graphics);

                // paint the picture in from the back buffer into the form draw area
                ManagedBackBuffer.Render(ControlGraphics);
            }
            catch (Exception Exp) { Console.WriteLine(Exp.Message); }
        }

        #endregion

        private void Render(Graphics TempGraphics)
        {
            Rectangle page = PageBounds;
            
            //Clear with backcolor
            TempGraphics.Clear(BackColor);
            
            //Draw page background      
            if (!PageBounds.Size.Equals(Size.Empty))
            {
                if (page.Y < Margin.Top)
                    page.Height = page.Height + page.Y;
                if (page.X < Margin.Left)
                    page.Width = page.Width + page.X;
                Brush bg = new SolidBrush(_bgColor);
                TempGraphics.FillRectangle(bg, page);
            }
            
            //Custom Paint!
            if (PaintControl != null)
                PaintControl.Invoke(this,CurrentView,PageLocation, TempGraphics);

            if (!PageBounds.Size.Equals(Size.Empty) )
            {
                if (_drawRect)
                {
                    Pen mp = new Pen(_rectColor);
                    //Draw page margin
                    TempGraphics.DrawRectangle(mp, page);
                }
                
                if(DrawShadow)
                    //Draw a Shadow
                    ShadowDrawing.DrawShadow(TempGraphics, page);
            }

        }

        #region Bounds
        /// <summary>
        /// Returns the bounds of the current view
        /// </summary>
        public virtual Rectangle CurrentView
        {
            get
            {
                
                int x = hsb.Value;
                int y = vsb.Value;
                int w = ClientSize.Width;
                int h = ClientSize.Height;
               //Begin Coord after margin
                x -= (x <= Margin.Left)? x : Margin.Left;
                y -= (y <= Margin.Top) ? y : Margin.Top;
                //Add scrollbar space
                w += (PageSize.Width <= ViewSize.Width) ? ScrollBarSize.Width : 0;
                h += (PageSize.Height <= ViewSize.Height) ? ScrollBarSize.Height : 0;
                //Add margin space
                w += (x >= Margin.Left) ? Margin.Size.Width : Margin.Size.Width;
                h += (y >= Margin.Top) ? Margin.Size.Height : Margin.Size.Height;

                //Fix size to PageSize
                if ((x + w) >= PageSize.Width)
                    w = PageSize.Width - x;
                if ((y + w) >= PageSize.Height)
                    h = PageSize.Height - y;

                //Adjust for rounding errors
                _currentView = new Rectangle(x, y, w, h);
                //For test purpose, show a window with the CurrentView box
                //frm.CurrentView = _currentView;
                return _currentView;
            }
        }

        /// <summary>
        /// Bounds of the page
        /// </summary>
        public virtual Rectangle PageBounds
        {
            get
            {
                return _pageBounds;
            }
        }
        public virtual Point PageLocation
        {
            get
            {
                return _pageLocation;                
            }
        }
        public virtual Size PageSize
        {
            get
            {
                return _pageSize;
            }
            set
            {
                _Zoom = 1;
                _pageSize = value;
                
                Resized();  //Calculate
                Invalidate(); //Redraw
            }
        }
        private float _Zoom = 1;
        
        public virtual void ZoomIn()
        {
            ZoomIn(4f / 3f);
        }
        public virtual void ZoomIn(float factor)
        {
            _Zoom *= factor;
            _pageSize = Size.Round( new SizeF(PageSize.Width * factor, PageSize.Height * factor));
            
            Resized();  //Calculate
            Invalidate(); //Redraw
        }
        public virtual void ZoomOut()
        {
            ZoomOut(3f / 4f);
        }
        public virtual void ZoomOut(float factor)
        {
            _Zoom *= factor;
            _pageSize = Size.Round(new SizeF(PageSize.Width * factor, PageSize.Height * factor));

            Resized();  //Calculate
            Invalidate(); //Redraw
        }

        /// <summary>
        /// Bounds of the client view, takes care of margins and scroll bars
        /// </summary>
        public virtual Rectangle ClientBounds
        {
            get
            {
                return _clientBounds;
            }
        }
        public virtual Point ClientLocation
        {
            get
            {
                return _clientLocation;
            }
        }
        public virtual new Size ClientSize
        {
            get
            {
                return _clientSize;
            }

        }

        /// <summary>
        /// Bounds of the view, takes care of the scrollbars
        /// </summary>
        public virtual Rectangle ViewBounds
        {
            get
            {
                return _viewBounds;
            }
        }
        public virtual Point ViewLocation
        {
            get
            {
                return Point.Empty;
            }
        }
        public virtual Size ViewSize
        {
            get
            {
                return _viewSize;
            }
        }
        public virtual Size ScrollBarSize
        {
            get
            {
                return _scrollbarSize;
            }
        }

        public virtual Point ScrollPosition
        {
            get
            {
                Point pos = new Point(hsb.Value, vsb.Value);
                pos.Offset(-Margin.Left, -Margin.Top);
                return pos;
            }
            set
            {
                Point pos = value;
                //Greater equal to zero
                pos.X = Math.Max(0, pos.X);
                pos.Y = Math.Max(0, pos.Y);
                //Margin offset
                //pos.Offset(Margin.Left, Margin.Top);

                //Lower equal to hsb.Maximum
                hsb.Value = Math.Min(pos.X, hsb.Maximum);
                vsb.Value = Math.Min(pos.Y, vsb.Maximum);
                
                Resized();
                Invalidate();
            }
        }
        #endregion

        private void ZoomChanged()
        {
            //Resize Page
            //Page.Size= newSize;
            Resized();      //Recalculate
            Invalidate();   //Redraw
        }

        private void Resized()
        {
            //----Window resized----
            //Save status
            bool bvtoMax = vsb.Value == vsb.Maximum && vsb.Value>0;
            bool bhtoMax = hsb.Value == hsb.Maximum  && hsb.Value>0;

            //Recalc View
            int viewWidth = Width;
            int viewHeight = Height;
            if (PageSize.Width > (Width - Margin.Size.Width))
                viewHeight -= ScrollBarSize.Height;

            if (PageSize.Height > (Height - Margin.Size.Height))
                viewWidth -= ScrollBarSize.Width;
            _viewSize = new Size(viewWidth, viewHeight);
            _viewBounds = new Rectangle(Point.Empty, _viewSize);

            //Recalc ClientSize
            int clientWidth = viewWidth - Margin.Size.Width;
            int clientHeight = viewHeight - Margin.Size.Height;
            
            _clientSize = new Size(clientWidth, clientHeight);
            _clientBounds = new Rectangle(ClientLocation, _clientSize);
            
            
            InvalidateScrollBarChanged = false;
            if ((PageSize.Height - ClientSize.Height) > 0)
            {
                vsb.Maximum = (PageSize.Height - ClientSize.Height )/2 + Margin.Size.Height;
                vsb.Visible = true;
                if (bvtoMax) vsb.Value = vsb.Maximum;
            }
            else
            {
                vsb.Maximum = 0;
                vsb.Value = 0;
                vsb.Visible = false;
            }
            if ((int)(PageSize.Width - ClientSize.Width) > 0)
            {
                
                hsb.Maximum = (int)(PageSize.Width - ClientSize.Width )/2 + Margin.Size.Width;
                hsb.Visible = true;
                if (bhtoMax) hsb.Value = hsb.Maximum;
            }
            else
            {
                hsb.Maximum = 0;
                hsb.Value = 0;
                hsb.Visible = false;
            }
            InvalidateScrollBarChanged = true;

            //Recalc page location
            RecalcPageLocation();

        }

        private void RecalcPageLocation()
        {
            if (PageSize.Width < ClientBounds.Width && PageSize.Height > ClientBounds.Height)
                //Center vertically
                _pageLocation = new Point((ClientBounds.Width - PageSize.Width) / 2 + Margin.Left, Margin.Top - vsb.Value);
            else if (PageSize.Width > ClientBounds.Width && PageSize.Height < ClientBounds.Height)
                //Center horizontally
                _pageLocation = new Point(Margin.Left - hsb.Value, (ClientBounds.Height - PageSize.Height) / 2 + Margin.Top);
            else if (PageSize.Width < ClientBounds.Width && PageSize.Height < ClientBounds.Height)
                //center both
                _pageLocation = new Point((ClientBounds.Width - PageSize.Width) / 2 + Margin.Left, (ClientBounds.Height - PageSize.Height) / 2 + Margin.Top);
            else
                _pageLocation = new Point(Margin.Left - hsb.Value, Margin.Top - vsb.Value);
            _pageBounds = new Rectangle(_pageLocation, PageSize);

        }

        private void DoubleBufferControl_MarginChanged(object sender, EventArgs e)
        {
            _clientLocation = new Point(Margin.Left, Margin.Top);
            Resized();
            Invalidate();
        }

        #region ScrollBar
        private void hsb_Scroll(object sender, ScrollEventArgs e)
        {
            if (InvalidateScrollBarChanged)
            {
                RecalcPageLocation();
                Invalidate();
            }
        }
        private void vsb_Resize(object sender, EventArgs e)
        {
            _scrollbarSize = new Size(vsb.Width, hsb.Height);
            Resized();
            Invalidate();
        }
        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            if (InvalidateScrollBarChanged)
            {
                RecalcPageLocation();
                Invalidate();
            }

        }
        #endregion

        private CursorStatus getCursorStatus(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Middle)
                return CursorStatus.Move;

            return _cursorStatus;
        }
        private bool IsActive
        {
            get
            {
                return true;
                //return GetForegroundWindow() == this.Handle.ToInt32();
            }
        }
        public bool MouseInPage(Point p)
        {
            if (IsActive)
            {
                Rectangle rect = new Rectangle(0, 0, CurrentView.Width,CurrentView.Height);
                return rect.Contains(p);
            }
            return false;
        }

        private void DrawRubberFrame()
        {
            using(Graphics g =Graphics.FromHwndInternal(this.Handle)){
                GDIDraw.GDI gdi = new GDIDraw.GDI(g);
                gdi.DrawLine(Color.Gray, _pointStart, _pointCurrent);
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            Point pos = e.Location; // PointToClient(e.Location);
        
            if (MouseInPage(pos) && _bMouseCaptured)
            {
                int offsetX;
                int offsetY;

                switch (getCursorStatus(e))
                {
                    case CursorStatus.Move:
                        Cursor = Cursors.Hand;
                        offsetX = e.X - _pointCurrent.X;
                        offsetY = e.Y - _pointCurrent.Y;
                        if (PageBounds.Width > ViewBounds.Width)
                        {
                            _pointX = (hsb.Value);
                            _pointX -= offsetX;
                        }
                        if (PageBounds.Height > ViewBounds.Height)
                        {
                            _pointY = (vsb.Value);
                            _pointY -= offsetY;
                        }
                        ScrollPosition = new Point(_pointX, _pointY);
                        break;
                    case CursorStatus.Zoom:
                        Cursor = Cursors.SizeAll;
                        offsetX = e.X - _pointCurrent.X;
                        offsetY = e.Y - _pointCurrent.Y;
                        _pointCurrent = e.Location; //Update current Point
                        DrawRubberFrame();
                        break;
                }
            }
            if (MouseInPage(pos))
            {
                _pointCurrent = e.Location; //Update current Point
               //e.Location.Offset(Margin.Top, Margin.Bottom);
                base.OnMouseMove(e);
            }
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            Point pos = e.Location; // PointToClient(e.Location);
            if (MouseInPage(pos) && e.Button == MouseButtons.Left)
            {
                _pointCurrent = e.Location;
                _pointStart = e.Location;
                _bMouseCaptured = true;
            }
            e.Location.Offset(Margin.Top, Margin.Bottom);
            base.OnMouseDown(e);
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            Point pos = e.Location;// PointToClient(e.Location);
            if (MouseInPage(pos) && _bMouseCaptured)
            {
                switch (getCursorStatus(e))
                {
                    case CursorStatus.Move:
                        ScrollPosition = new Point(_pointX, _pointY);
                        break;
                }
                Cursor = Cursors.Default;
                _bMouseCaptured = false;
                base.OnMouseUp(e);
            }
            //ReleaseRubberFrame();
        }

        private int _deltasCount = 0;
        private bool bHasMorePagesT=true;
        private bool bHasMorePagesD=true;
        protected override void OnMouseWheel(MouseEventArgs e)
        {
            Point pos = e.Location; // pnlPageContanier.PointToClient(e.Location);
            if (MouseInPage(pos))
            {
                int pointY = ScrollPosition.Y;
                if (e.Delta < 0)
                    pointY = ScrollPosition.Y + 120 * PageBounds.Height / ViewBounds.Height / 6;
                else if (e.Delta > 0)
                    pointY = ScrollPosition.Y - 120 * PageBounds.Height / ViewBounds.Height / 6;
                ScrollPosition = new Point(ScrollPosition.X, pointY);
                if (vsb.Maximum == 0)
                {
                    if (e.Delta < 0)
                    {
                        if (NextPage != null)
                           NextPage.Invoke(this);
                    }
                    else
                    {
                        if (PreviousPage != null)
                            PreviousPage.Invoke(this);
                    }

                }
                else
                {
                    if (vsb.Value == vsb.Maximum)
                    {
                        //GoNext
                        _deltasCount++;
                        if (_deltasCount > 1)
                        {
                            _deltasCount = 0;
                            if (NextPage != null)
                                bHasMorePagesD = NextPage.Invoke(this);
                            bHasMorePagesT = true;
                            if (bHasMorePagesD)
                                ScrollPosition = new Point(hsb.Value, 0);
                        }
                    }
                    if (vsb.Value == 0)
                    {
                        //GoNext
                        _deltasCount++;
                        if (_deltasCount > 1)
                        {
                            _deltasCount = 0;
                            //GoBack
                            if (PreviousPage != null)
                                bHasMorePagesT = PreviousPage.Invoke(this);
                            bHasMorePagesD = true;
                            if (bHasMorePagesT)
                                ScrollPosition = new Point(hsb.Value, vsb.Maximum);
                        }
                    }
                }
                
            }
            base.OnMouseWheel(e);
        }
    }

    #region Shadow Drawing

    public class ShadowDrawing
    {
        private static int shadowSize = 5;
        private static int shadowMargin = 2;

        // static for good perfomance 
        static Image shadowDownRight = new Bitmap(Properties.Resources.tshadowdownright);
        static Image shadowDownLeft = new Bitmap(Properties.Resources.tshadowdownleft);
        static Image shadowDown = new Bitmap(Properties.Resources.tshadowdown);
        static Image shadowRight = new Bitmap(Properties.Resources.tshadowright);
        static Image shadowTopRight = new Bitmap(Properties.Resources.tshadowtopright);

        public static void DrawShadow(Graphics g,Rectangle r)
        {

            r.Offset(shadowSize,shadowSize);
            TextureBrush shadowRightBrush = new TextureBrush(shadowRight, WrapMode.Tile);
            TextureBrush shadowDownBrush = new TextureBrush(shadowDown, WrapMode.Tile);

            // Translate (move) the brushes so the top or left of the image matches the top or left of the
            // area where it's drawed. If you don't understand why this is necessary, comment it out. 
            // Hint: The tiling would start at 0,0 of the control, so the shadows will be offset a little.
            //shadowDownBrush.TranslateTransform(0, r.Height - shadowSize);
            //shadowRightBrush.TranslateTransform(r.Width - shadowSize, 0);
            shadowDownBrush.TranslateTransform(0,r.Height + r.Y - shadowSize);
            shadowRightBrush.TranslateTransform(r.Width + r.X - shadowSize, 0);

            // Define the rectangles that will be filled with the brush.
            // (where the shadow is drawn)
       
            Rectangle shadowDownRectangle = new Rectangle(
                r.X + shadowSize + shadowMargin,                      // X
                r.Y + r.Height - shadowSize,                            // Y
                r.Width - (2 * shadowSize + shadowMargin),        // width (stretches)
                shadowSize                                      // height
                );


            Rectangle shadowRightRectangle = new Rectangle(
                r.Width - shadowSize + r.X,                             // X
                shadowSize + shadowMargin + r.Y,                      // Y
                shadowSize,                                     // width
                r.Height - (shadowSize * 2 + shadowMargin)        // height (stretches)
                );

            // And draw the shadow on the right and at the bottom.
            g.FillRectangle(shadowDownBrush, shadowDownRectangle);
            g.FillRectangle(shadowRightBrush, shadowRightRectangle);

            // Now for the corners, draw the 3 5x5 pixel images.
            g.DrawImage(shadowTopRight, new Rectangle(r.Width - shadowSize+r.X, shadowMargin+r.Y, shadowSize, shadowSize));
            g.DrawImage(shadowDownRight, new Rectangle(r.Width - shadowSize+r.X, r.Height - shadowSize+r.Y, shadowSize, shadowSize));
            g.DrawImage(shadowDownLeft, new Rectangle(shadowMargin+r.X, r.Height - shadowSize+r.Y, shadowSize, shadowSize));

        
            // Memory efficiency
            shadowDownBrush.Dispose();
            shadowRightBrush.Dispose();

            shadowDownBrush = null;
            shadowRightBrush = null;

        }

        public static void DrawBorderLeft(Graphics g, Point p)
        {
            
        }
        public static void DrawBorderBottom(Graphics g, Point p)
        {
        }
        public static void DrawBorderDownRight(Graphics g, Point p)
        {
        }
        public static void DrawBorderRight(Graphics g, Point p)
        {
        }
        public static void DrawBorderTopRight(Graphics g, Point p)
        {
        }
    }
    #endregion
}
