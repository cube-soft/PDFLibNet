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
            this.optPrintCurrent = new System.Windows.Forms.RadioButton();
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
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udTopage)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udFromPage)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udQuality)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udResolution)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AccessibleDescription = null;
            this.label1.AccessibleName = null;
            resources.ApplyResources(this.label1, "label1");
            this.label1.Font = null;
            this.label1.Name = "label1";
            // 
            // txtFileName
            // 
            this.txtFileName.AccessibleDescription = null;
            this.txtFileName.AccessibleName = null;
            resources.ApplyResources(this.txtFileName, "txtFileName");
            this.txtFileName.BackgroundImage = null;
            this.txtFileName.Font = null;
            this.txtFileName.Name = "txtFileName";
            // 
            // cmdChoseFile
            // 
            this.cmdChoseFile.AccessibleDescription = null;
            this.cmdChoseFile.AccessibleName = null;
            resources.ApplyResources(this.cmdChoseFile, "cmdChoseFile");
            this.cmdChoseFile.BackgroundImage = null;
            this.cmdChoseFile.Font = null;
            this.cmdChoseFile.Name = "cmdChoseFile";
            this.cmdChoseFile.UseVisualStyleBackColor = true;
            this.cmdChoseFile.Click += new System.EventHandler(this.cmdChoseFile_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.AccessibleDescription = null;
            this.groupBox1.AccessibleName = null;
            resources.ApplyResources(this.groupBox1, "groupBox1");
            this.groupBox1.BackgroundImage = null;
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
            this.groupBox1.Font = null;
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.TabStop = false;
            // 
            // optPrintCurrent
            // 
            this.optPrintCurrent.AccessibleDescription = null;
            this.optPrintCurrent.AccessibleName = null;
            resources.ApplyResources(this.optPrintCurrent, "optPrintCurrent");
            this.optPrintCurrent.BackgroundImage = null;
            this.optPrintCurrent.Font = null;
            this.optPrintCurrent.Name = "optPrintCurrent";
            this.optPrintCurrent.UseVisualStyleBackColor = true;
            // 
            // udTopage
            // 
            this.udTopage.AccessibleDescription = null;
            this.udTopage.AccessibleName = null;
            resources.ApplyResources(this.udTopage, "udTopage");
            this.udTopage.Font = null;
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
            this.udFromPage.AccessibleDescription = null;
            this.udFromPage.AccessibleName = null;
            resources.ApplyResources(this.udFromPage, "udFromPage");
            this.udFromPage.Font = null;
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
            this.optFromPage.AccessibleDescription = null;
            this.optFromPage.AccessibleName = null;
            resources.ApplyResources(this.optFromPage, "optFromPage");
            this.optFromPage.BackgroundImage = null;
            this.optFromPage.Font = null;
            this.optFromPage.Name = "optFromPage";
            this.optFromPage.UseVisualStyleBackColor = true;
            this.optFromPage.CheckedChanged += new System.EventHandler(this.optFromPage_CheckedChanged);
            // 
            // optPrintAll
            // 
            this.optPrintAll.AccessibleDescription = null;
            this.optPrintAll.AccessibleName = null;
            resources.ApplyResources(this.optPrintAll, "optPrintAll");
            this.optPrintAll.BackgroundImage = null;
            this.optPrintAll.Checked = true;
            this.optPrintAll.Font = null;
            this.optPrintAll.Name = "optPrintAll";
            this.optPrintAll.TabStop = true;
            this.optPrintAll.UseVisualStyleBackColor = true;
            this.optPrintAll.CheckedChanged += new System.EventHandler(this.optPrintAll_CheckedChanged);
            // 
            // label7
            // 
            this.label7.AccessibleDescription = null;
            this.label7.AccessibleName = null;
            resources.ApplyResources(this.label7, "label7");
            this.label7.Font = null;
            this.label7.Name = "label7";
            // 
            // label5
            // 
            this.label5.AccessibleDescription = null;
            this.label5.AccessibleName = null;
            resources.ApplyResources(this.label5, "label5");
            this.label5.Font = null;
            this.label5.Name = "label5";
            // 
            // udQuality
            // 
            this.udQuality.AccessibleDescription = null;
            this.udQuality.AccessibleName = null;
            resources.ApplyResources(this.udQuality, "udQuality");
            this.udQuality.Font = null;
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
            this.label4.AccessibleDescription = null;
            this.label4.AccessibleName = null;
            resources.ApplyResources(this.label4, "label4");
            this.label4.Font = null;
            this.label4.Name = "label4";
            // 
            // label3
            // 
            this.label3.AccessibleDescription = null;
            this.label3.AccessibleName = null;
            resources.ApplyResources(this.label3, "label3");
            this.label3.Font = null;
            this.label3.Name = "label3";
            // 
            // udResolution
            // 
            this.udResolution.AccessibleDescription = null;
            this.udResolution.AccessibleName = null;
            resources.ApplyResources(this.udResolution, "udResolution");
            this.udResolution.Font = null;
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
            this.label2.AccessibleDescription = null;
            this.label2.AccessibleName = null;
            resources.ApplyResources(this.label2, "label2");
            this.label2.Font = null;
            this.label2.Name = "label2";
            // 
            // pgBar
            // 
            this.pgBar.AccessibleDescription = null;
            this.pgBar.AccessibleName = null;
            resources.ApplyResources(this.pgBar, "pgBar");
            this.pgBar.BackgroundImage = null;
            this.pgBar.Font = null;
            this.pgBar.Name = "pgBar";
            // 
            // btnExport
            // 
            this.btnExport.AccessibleDescription = null;
            this.btnExport.AccessibleName = null;
            resources.ApplyResources(this.btnExport, "btnExport");
            this.btnExport.BackgroundImage = null;
            this.btnExport.Font = null;
            this.btnExport.Name = "btnExport";
            this.btnExport.UseVisualStyleBackColor = true;
            this.btnExport.Click += new System.EventHandler(this.btnExport_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.AccessibleDescription = null;
            this.btnCancel.AccessibleName = null;
            resources.ApplyResources(this.btnCancel, "btnCancel");
            this.btnCancel.BackgroundImage = null;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Font = null;
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // dlgSave
            // 
            resources.ApplyResources(this.dlgSave, "dlgSave");
            // 
            // frmExportJpg
            // 
            this.AcceptButton = this.btnExport;
            this.AccessibleDescription = null;
            this.AccessibleName = null;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = null;
            this.CancelButton = this.btnCancel;
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnExport);
            this.Controls.Add(this.pgBar);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cmdChoseFile);
            this.Controls.Add(this.txtFileName);
            this.Controls.Add(this.label1);
            this.Font = null;
            this.Icon = null;
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