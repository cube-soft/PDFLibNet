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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tvwOutline = new System.Windows.Forms.TreeView();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.listView2 = new System.Windows.Forms.ListView();
            this.tabView = new System.Windows.Forms.TabControl();
            this.tpvPDF = new System.Windows.Forms.TabPage();
            this.pageViewControl1 = new PDFViewer.PageViewer();
            this.tpvText = new System.Windows.Forms.TabPage();
            this.txtTextView = new System.Windows.Forms.TextBox();
            this.tpvImages = new System.Windows.Forms.TabPage();
            this.tsImages = new System.Windows.Forms.ToolStrip();
            this.tsImagesUpdate = new System.Windows.Forms.ToolStripButton();
            this.tsImagesSave = new System.Windows.Forms.ToolStripButton();
            this.pdfImagesThumbView1 = new PDFViewer.PDFImagesThumbView();
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
            this.tsbAntialias = new System.Windows.Forms.ToolStripButton();
            this.tsbVectorAntialias = new System.Windows.Forms.ToolStripButton();
            this.printDialog1 = new System.Windows.Forms.PrintDialog();
            this.printDocument1 = new System.Drawing.Printing.PrintDocument();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.ttpLink = new System.Windows.Forms.ToolTip(this.components);
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.StatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.bgLoadPages = new System.ComponentModel.BackgroundWorker();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabView.SuspendLayout();
            this.tpvPDF.SuspendLayout();
            this.tpvText.SuspendLayout();
            this.tpvImages.SuspendLayout();
            this.tsImages.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            resources.ApplyResources(this.splitContainer1, "splitContainer1");
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tabControl1);
            resources.ApplyResources(this.splitContainer1.Panel1, "splitContainer1.Panel1");
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tabView);
            resources.ApplyResources(this.splitContainer1.Panel2, "splitContainer1.Panel2");
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            resources.ApplyResources(this.tabControl1, "tabControl1");
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.tvwOutline);
            resources.ApplyResources(this.tabPage1, "tabPage1");
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tvwOutline
            // 
            resources.ApplyResources(this.tvwOutline, "tvwOutline");
            this.tvwOutline.Name = "tvwOutline";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.listView2);
            resources.ApplyResources(this.tabPage2, "tabPage2");
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // listView2
            // 
            resources.ApplyResources(this.listView2, "listView2");
            this.listView2.FullRowSelect = true;
            this.listView2.GridLines = true;
            this.listView2.MultiSelect = false;
            this.listView2.Name = "listView2";
            this.listView2.OwnerDraw = true;
            this.listView2.UseCompatibleStateImageBehavior = false;
            this.listView2.View = System.Windows.Forms.View.Tile;
            this.listView2.DrawItem += new System.Windows.Forms.DrawListViewItemEventHandler(this.listView2_DrawItem);
            // 
            // tabView
            // 
            resources.ApplyResources(this.tabView, "tabView");
            this.tabView.Controls.Add(this.tpvPDF);
            this.tabView.Controls.Add(this.tpvText);
            this.tabView.Controls.Add(this.tpvImages);
            this.tabView.Name = "tabView";
            this.tabView.SelectedIndex = 0;
            this.tabView.Selected += new System.Windows.Forms.TabControlEventHandler(this.tabView_Selected);
            // 
            // tpvPDF
            // 
            this.tpvPDF.Controls.Add(this.pageViewControl1);
            resources.ApplyResources(this.tpvPDF, "tpvPDF");
            this.tpvPDF.Name = "tpvPDF";
            this.tpvPDF.UseVisualStyleBackColor = true;
            // 
            // pageViewControl1
            // 
            this.pageViewControl1.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.pageViewControl1.BorderColor = System.Drawing.Color.Black;
            resources.ApplyResources(this.pageViewControl1, "pageViewControl1");
            this.pageViewControl1.DrawBorder = false;
            this.pageViewControl1.DrawShadow = true;
            this.pageViewControl1.Name = "pageViewControl1";
            this.pageViewControl1.PageColor = System.Drawing.Color.White;
            this.pageViewControl1.PageSize = new System.Drawing.Size(0, 0);
            this.pageViewControl1.PaintMethod = PDFViewer.PageViewer.DoubleBufferMethod.BuiltInOptimizedDoubleBuffer;
            this.pageViewControl1.ScrollPosition = new System.Drawing.Point(-10, -10);
            this.pageViewControl1.PreviousPage += new PDFViewer.PageViewer.MovePageHandler(this.doubleBufferControl1_PreviousPage);
            this.pageViewControl1.NextPage += new PDFViewer.PageViewer.MovePageHandler(this.doubleBufferControl1_NextPage);
            this.pageViewControl1.PaintControl += new PDFViewer.PageViewer.PaintControlHandler(this.doubleBufferControl1_PaintControl);
            // 
            // tpvText
            // 
            this.tpvText.Controls.Add(this.txtTextView);
            resources.ApplyResources(this.tpvText, "tpvText");
            this.tpvText.Name = "tpvText";
            this.tpvText.UseVisualStyleBackColor = true;
            // 
            // txtTextView
            // 
            resources.ApplyResources(this.txtTextView, "txtTextView");
            this.txtTextView.Name = "txtTextView";
            this.txtTextView.TabStop = false;
            // 
            // tpvImages
            // 
            this.tpvImages.Controls.Add(this.tsImages);
            this.tpvImages.Controls.Add(this.pdfImagesThumbView1);
            resources.ApplyResources(this.tpvImages, "tpvImages");
            this.tpvImages.Name = "tpvImages";
            this.tpvImages.UseVisualStyleBackColor = true;
            // 
            // tsImages
            // 
            this.tsImages.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsImagesUpdate,
            this.tsImagesSave});
            resources.ApplyResources(this.tsImages, "tsImages");
            this.tsImages.Name = "tsImages";
            // 
            // tsImagesUpdate
            // 
            this.tsImagesUpdate.Image = global::PDFViewer.Properties.Resources.RefreshDocView;
            resources.ApplyResources(this.tsImagesUpdate, "tsImagesUpdate");
            this.tsImagesUpdate.Name = "tsImagesUpdate";
            this.tsImagesUpdate.Click += new System.EventHandler(this.tsImagesUpdate_Click);
            // 
            // tsImagesSave
            // 
            this.tsImagesSave.Image = global::PDFViewer.Properties.Resources.Save;
            resources.ApplyResources(this.tsImagesSave, "tsImagesSave");
            this.tsImagesSave.Name = "tsImagesSave";
            // 
            // pdfImagesThumbView1
            // 
            resources.ApplyResources(this.pdfImagesThumbView1, "pdfImagesThumbView1");
            this.pdfImagesThumbView1.Name = "pdfImagesThumbView1";
            this.pdfImagesThumbView1.UseCompatibleStateImageBehavior = false;
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
            this.toolStripButton2,
            this.tsbAntialias,
            this.tsbVectorAntialias});
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
            this.toolStripButton1.Click += new System.EventHandler(this.tsbZoomIn_Click);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.Image = global::PDFViewer.Properties.Resources.ZoomOut;
            resources.ApplyResources(this.toolStripButton2, "toolStripButton2");
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Click += new System.EventHandler(this.tsbZoomOut_Click);
            // 
            // tsbAntialias
            // 
            this.tsbAntialias.Checked = true;
            this.tsbAntialias.CheckOnClick = true;
            this.tsbAntialias.CheckState = System.Windows.Forms.CheckState.Checked;
            this.tsbAntialias.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            resources.ApplyResources(this.tsbAntialias, "tsbAntialias");
            this.tsbAntialias.Name = "tsbAntialias";
            this.tsbAntialias.Click += new System.EventHandler(this.tsbAntialias_Click);
            // 
            // tsbVectorAntialias
            // 
            this.tsbVectorAntialias.CheckOnClick = true;
            this.tsbVectorAntialias.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            resources.ApplyResources(this.tsbVectorAntialias, "tsbVectorAntialias");
            this.tsbVectorAntialias.Name = "tsbVectorAntialias";
            this.tsbVectorAntialias.Click += new System.EventHandler(this.tsbVectorAntialias_Click);
            // 
            // printDialog1
            // 
            this.printDialog1.UseEXDialog = true;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusLabel});
            resources.ApplyResources(this.statusStrip1, "statusStrip1");
            this.statusStrip1.Name = "statusStrip1";
            // 
            // StatusLabel
            // 
            this.StatusLabel.Name = "StatusLabel";
            resources.ApplyResources(this.StatusLabel, "StatusLabel");
            // 
            // bgLoadPages
            // 
            this.bgLoadPages.WorkerReportsProgress = true;
            this.bgLoadPages.WorkerSupportsCancellation = true;
            // 
            // frmPDFViewer
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.statusStrip1);
            this.DoubleBuffered = true;
            this.Name = "frmPDFViewer";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabView.ResumeLayout(false);
            this.tpvPDF.ResumeLayout(false);
            this.tpvText.ResumeLayout(false);
            this.tpvText.PerformLayout();
            this.tpvImages.ResumeLayout(false);
            this.tpvImages.PerformLayout();
            this.tsImages.ResumeLayout(false);
            this.tsImages.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
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
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ToolStripButton tsbAntialias;
        private System.Windows.Forms.ToolStripButton tsbVectorAntialias;
        private System.Windows.Forms.StatusStrip statusStrip1;
        public System.Windows.Forms.ToolStripStatusLabel StatusLabel;
        private System.Windows.Forms.TabControl tabView;
        private System.Windows.Forms.TabPage tpvPDF;
        private PageViewer pageViewControl1;
        private System.Windows.Forms.TabPage tpvText;
        private System.Windows.Forms.TextBox txtTextView;
        private System.Windows.Forms.TabPage tpvImages;
        private System.Windows.Forms.ToolStrip tsImages;
        private System.Windows.Forms.ToolStripButton tsImagesUpdate;
        private System.Windows.Forms.ToolStripButton tsImagesSave;
        private PDFImagesThumbView pdfImagesThumbView1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.ComponentModel.BackgroundWorker bgLoadPages;
        private System.Windows.Forms.ListView listView2;
    }
}

