namespace PDFViewer
{
    partial class frmPDFViewer
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmPDFViewer));
            this.tvwOutline = new System.Windows.Forms.TreeView();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.tsbOpen = new System.Windows.Forms.ToolStripButton();
            this.tsbPrintAs = new System.Windows.Forms.ToolStripButton();
            this.tsbPrev = new System.Windows.Forms.ToolStripButton();
            this.txtPage = new System.Windows.Forms.ToolStripTextBox();
            this.tsbNext = new System.Windows.Forms.ToolStripButton();
            this.tsbSearch = new System.Windows.Forms.ToolStripButton();
            this.tsbAbout = new System.Windows.Forms.ToolStripButton();
            this.tsbPrint = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.printDialog1 = new System.Windows.Forms.PrintDialog();
            this.printDocument1 = new System.Drawing.Printing.PrintDocument();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.ttpLink = new System.Windows.Forms.ToolTip(this.components);
            this.pageViewControl1 = new PDFViewer.PageViewer();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tvwOutline
            // 
            resources.ApplyResources(this.tvwOutline, "tvwOutline");
            this.tvwOutline.Name = "tvwOutline";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbOpen,
            this.tsbPrintAs,
            this.tsbPrev,
            this.txtPage,
            this.tsbNext,
            this.tsbSearch,
            this.tsbAbout,
            this.tsbPrint,
            this.toolStripButton1,
            this.toolStripButton2});
            resources.ApplyResources(this.toolStrip1, "toolStrip1");
            this.toolStrip1.Name = "toolStrip1";
            // 
            // tsbOpen
            // 
            this.tsbOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbOpen.Image = global::PDFViewer.Properties.Resources.dmdskres_373_9_16x16x32;
            resources.ApplyResources(this.tsbOpen, "tsbOpen");
            this.tsbOpen.Name = "tsbOpen";
            this.tsbOpen.Click += new System.EventHandler(this.tsbOpen_Click);
            // 
            // tsbPrintAs
            // 
            this.tsbPrintAs.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbPrintAs.Image = global::PDFViewer.Properties.Resources.PrintBrmUi_102_6_16x16x32;
            resources.ApplyResources(this.tsbPrintAs, "tsbPrintAs");
            this.tsbPrintAs.Name = "tsbPrintAs";
            this.tsbPrintAs.Click += new System.EventHandler(this.tsbPrintAs_Click);
            // 
            // tsbPrev
            // 
            this.tsbPrev.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbPrev.Image = global::PDFViewer.Properties.Resources.netshell_21611_1_16x16x32;
            resources.ApplyResources(this.tsbPrev, "tsbPrev");
            this.tsbPrev.Name = "tsbPrev";
            this.tsbPrev.Click += new System.EventHandler(this.tsbPrev_Click);
            // 
            // txtPage
            // 
            this.txtPage.Name = "txtPage";
            resources.ApplyResources(this.txtPage, "txtPage");
            this.txtPage.KeyDown += new System.Windows.Forms.KeyEventHandler(this.txtPage_KeyDown);
            // 
            // tsbNext
            // 
            this.tsbNext.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbNext.Image = global::PDFViewer.Properties.Resources.netshell_1611_1_16x16x32;
            resources.ApplyResources(this.tsbNext, "tsbNext");
            this.tsbNext.Name = "tsbNext";
            this.tsbNext.Click += new System.EventHandler(this.tsbNext_Click);
            // 
            // tsbSearch
            // 
            this.tsbSearch.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbSearch.Image = global::PDFViewer.Properties.Resources.SearchFolder_323_3_16x16x32;
            resources.ApplyResources(this.tsbSearch, "tsbSearch");
            this.tsbSearch.Name = "tsbSearch";
            this.tsbSearch.Click += new System.EventHandler(this.tsbSearch_Click);
            // 
            // tsbAbout
            // 
            this.tsbAbout.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbAbout.Image = global::PDFViewer.Properties.Resources.psr_206_4_16x16x32;
            resources.ApplyResources(this.tsbAbout, "tsbAbout");
            this.tsbAbout.Name = "tsbAbout";
            this.tsbAbout.Click += new System.EventHandler(this.tsbAbout_Click);
            // 
            // tsbPrint
            // 
            this.tsbPrint.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbPrint.Image = global::PDFViewer.Properties.Resources.FeedbackTool_109_12_16x16x32;
            resources.ApplyResources(this.tsbPrint, "tsbPrint");
            this.tsbPrint.Name = "tsbPrint";
            this.tsbPrint.Click += new System.EventHandler(this.toolStripButton1_Click);
            // 
            // toolStripButton1
            // 
            this.toolStripButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1.Image = global::PDFViewer.Properties.Resources.ZoomIn;
            resources.ApplyResources(this.toolStripButton1, "toolStripButton1");
            this.toolStripButton1.Name = "toolStripButton1";
            this.toolStripButton1.Click += new System.EventHandler(this.toolStripButton1_Click_1);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.Image = global::PDFViewer.Properties.Resources.ZoomOut;
            resources.ApplyResources(this.toolStripButton2, "toolStripButton2");
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Click += new System.EventHandler(this.toolStripButton2_Click);
            // 
            // printDialog1
            // 
            this.printDialog1.UseEXDialog = true;
            // 
            // pageViewControl1
            // 
            resources.ApplyResources(this.pageViewControl1, "pageViewControl1");
            this.pageViewControl1.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.pageViewControl1.BorderColor = System.Drawing.Color.Black;
            this.pageViewControl1.DrawBorder = false;
            this.pageViewControl1.DrawShadow = true;
            this.pageViewControl1.Name = "pageViewControl1";
            this.pageViewControl1.PageColor = System.Drawing.SystemColors.AppWorkspace;
            this.pageViewControl1.PageSize = new System.Drawing.Size(0, 0);
            this.pageViewControl1.PaintMethod = PDFViewer.PageViewer.DoubleBufferMethod.BuiltInOptimizedDoubleBuffer;
            this.pageViewControl1.ScrollPosition = new System.Drawing.Point(-10, -10);
            this.pageViewControl1.PreviousPage += new PDFViewer.PageViewer.MovePageHandler(this.doubleBufferControl1_PreviousPage);
            this.pageViewControl1.NextPage += new PDFViewer.PageViewer.MovePageHandler(this.doubleBufferControl1_NextPage);
            this.pageViewControl1.PaintControl += new PDFViewer.PageViewer.PaintControlHandler(this.doubleBufferControl1_PaintControl);
            // 
            // frmPDFViewer
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pageViewControl1);
            this.Controls.Add(this.tvwOutline);
            this.Controls.Add(this.toolStrip1);
            this.DoubleBuffered = true;
            this.Name = "frmPDFViewer";
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TreeView tvwOutline;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton tsbOpen;
        private System.Windows.Forms.ToolStripButton tsbPrev;
        private System.Windows.Forms.ToolStripTextBox txtPage;
        private System.Windows.Forms.ToolStripButton tsbNext;
        private System.Windows.Forms.ToolStripButton tsbSearch;
        private System.Windows.Forms.ToolStripButton tsbAbout;
        private System.Windows.Forms.ToolStripButton tsbPrint;
        private System.Windows.Forms.PrintDialog printDialog1;
        private System.Drawing.Printing.PrintDocument printDocument1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ToolStripButton toolStripButton1;
        private System.Windows.Forms.ToolStripButton toolStripButton2;
        private System.Windows.Forms.ToolTip ttpLink;
        private System.Windows.Forms.ToolStripButton tsbPrintAs;
        private PageViewer pageViewControl1;
    }
}

