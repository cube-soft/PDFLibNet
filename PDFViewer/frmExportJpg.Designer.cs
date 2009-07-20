namespace PDFViewer
{
    partial class frmExportJpg
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmExportJpg));
            this.label1 = new System.Windows.Forms.Label();
            this.txtFileName = new System.Windows.Forms.TextBox();
            this.cmdChoseFile = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.udTopage = new System.Windows.Forms.NumericUpDown();
            this.udFromPage = new System.Windows.Forms.NumericUpDown();
            this.optFromPage = new System.Windows.Forms.RadioButton();
            this.optPrintAll = new System.Windows.Forms.RadioButton();
            this.label7 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.udQuality = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.udResolution = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.pgBar = new System.Windows.Forms.ProgressBar();
            this.btnExport = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.dlgSave = new System.Windows.Forms.SaveFileDialog();
            this.optPrintCurrent = new System.Windows.Forms.RadioButton();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udTopage)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udFromPage)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udQuality)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udResolution)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // txtFileName
            // 
            resources.ApplyResources(this.txtFileName, "txtFileName");
            this.txtFileName.Name = "txtFileName";
            // 
            // cmdChoseFile
            // 
            resources.ApplyResources(this.cmdChoseFile, "cmdChoseFile");
            this.cmdChoseFile.Name = "cmdChoseFile";
            this.cmdChoseFile.UseVisualStyleBackColor = true;
            this.cmdChoseFile.Click += new System.EventHandler(this.cmdChoseFile_Click);
            // 
            // groupBox1
            // 
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.Controls.Add(this.optPrintCurrent);
            this.groupBox1.Controls.Add(this.udTopage);
            this.groupBox1.Controls.Add(this.udFromPage);
            this.groupBox1.Controls.Add(this.optFromPage);
            this.groupBox1.Controls.Add(this.optPrintAll);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.udQuality);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.udResolution);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            // 
            // udTopage
            // 
            resources.ApplyResources(this.udTopage, "udTopage");
            this.udTopage.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.udTopage.Name = "udTopage";
            this.udTopage.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // udFromPage
            // 
            resources.ApplyResources(this.udFromPage, "udFromPage");
            this.udFromPage.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.udFromPage.Name = "udFromPage";
            this.udFromPage.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // optFromPage
            // 
            resources.ApplyResources(this.optFromPage, "optFromPage");
            this.optFromPage.Name = "optFromPage";
            this.optFromPage.UseVisualStyleBackColor = true;
            this.optFromPage.CheckedChanged += new System.EventHandler(this.optFromPage_CheckedChanged);
            // 
            // optPrintAll
            // 
            this.optPrintAll.Checked = true;
            resources.ApplyResources(this.optPrintAll, "optPrintAll");
            this.optPrintAll.Name = "optPrintAll";
            this.optPrintAll.TabStop = true;
            this.optPrintAll.UseVisualStyleBackColor = true;
            this.optPrintAll.CheckedChanged += new System.EventHandler(this.optPrintAll_CheckedChanged);
            // 
            // label7
            // 
            resources.ApplyResources(this.label7, "label7");
            this.label7.Name = "label7";
            // 
            // label5
            // 
            resources.ApplyResources(this.label5, "label5");
            this.label5.Name = "label5";
            // 
            // udQuality
            // 
            resources.ApplyResources(this.udQuality, "udQuality");
            this.udQuality.Minimum = new decimal(new int[] {
            25,
            0,
            0,
            0});
            this.udQuality.Name = "udQuality";
            this.udQuality.Value = new decimal(new int[] {
            90,
            0,
            0,
            0});
            // 
            // label4
            // 
            resources.ApplyResources(this.label4, "label4");
            this.label4.Name = "label4";
            // 
            // label3
            // 
            resources.ApplyResources(this.label3, "label3");
            this.label3.Name = "label3";
            // 
            // udResolution
            // 
            resources.ApplyResources(this.udResolution, "udResolution");
            this.udResolution.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.udResolution.Minimum = new decimal(new int[] {
            72,
            0,
            0,
            0});
            this.udResolution.Name = "udResolution";
            this.udResolution.Value = new decimal(new int[] {
            150,
            0,
            0,
            0});
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // pgBar
            // 
            resources.ApplyResources(this.pgBar, "pgBar");
            this.pgBar.Name = "pgBar";
            // 
            // btnExport
            // 
            resources.ApplyResources(this.btnExport, "btnExport");
            this.btnExport.Name = "btnExport";
            this.btnExport.UseVisualStyleBackColor = true;
            this.btnExport.Click += new System.EventHandler(this.btnExport_Click);
            // 
            // btnCancel
            // 
            resources.ApplyResources(this.btnCancel, "btnCancel");
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // dlgSave
            // 
            resources.ApplyResources(this.dlgSave, "dlgSave");
            // 
            // optPrintCurrent
            // 
            resources.ApplyResources(this.optPrintCurrent, "optPrintCurrent");
            this.optPrintCurrent.Name = "optPrintCurrent";
            this.optPrintCurrent.UseVisualStyleBackColor = true;
            // 
            // frmExportJpg
            // 
            this.AcceptButton = this.btnExport;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnExport);
            this.Controls.Add(this.pgBar);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cmdChoseFile);
            this.Controls.Add(this.txtFileName);
            this.Controls.Add(this.label1);
            this.Name = "frmExportJpg";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmExportJpg_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udTopage)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udFromPage)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udQuality)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udResolution)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtFileName;
        private System.Windows.Forms.Button cmdChoseFile;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown udTopage;
        private System.Windows.Forms.NumericUpDown udFromPage;
        private System.Windows.Forms.RadioButton optFromPage;
        private System.Windows.Forms.RadioButton optPrintAll;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown udQuality;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown udResolution;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ProgressBar pgBar;
        private System.Windows.Forms.Button btnExport;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.SaveFileDialog dlgSave;
        private System.Windows.Forms.RadioButton optPrintCurrent;
    }
}