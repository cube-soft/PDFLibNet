namespace PDFViewer
{
    partial class frmSearch
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmSearch));
            this.txtSearch = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSearch = new System.Windows.Forms.Button();
            this.btnSearchNext = new System.Windows.Forms.Button();
            this.btnCancelSearch = new System.Windows.Forms.Button();
            this.chkWholeWord = new System.Windows.Forms.CheckBox();
            this.chkSearchUp = new System.Windows.Forms.CheckBox();
            this.chkFullSearch = new System.Windows.Forms.CheckBox();
            this.chkMatchCase = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // txtSearch
            // 
            resources.ApplyResources(this.txtSearch, "txtSearch");
            this.txtSearch.Name = "txtSearch";
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // btnSearch
            // 
            resources.ApplyResources(this.btnSearch, "btnSearch");
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.UseVisualStyleBackColor = true;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // btnSearchNext
            // 
            resources.ApplyResources(this.btnSearchNext, "btnSearchNext");
            this.btnSearchNext.Name = "btnSearchNext";
            this.btnSearchNext.UseVisualStyleBackColor = true;
            this.btnSearchNext.Click += new System.EventHandler(this.btnSearchNext_Click);
            // 
            // btnCancelSearch
            // 
            this.btnCancelSearch.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            resources.ApplyResources(this.btnCancelSearch, "btnCancelSearch");
            this.btnCancelSearch.Name = "btnCancelSearch";
            this.btnCancelSearch.UseVisualStyleBackColor = true;
            // 
            // chkWholeWord
            // 
            resources.ApplyResources(this.chkWholeWord, "chkWholeWord");
            this.chkWholeWord.Name = "chkWholeWord";
            this.chkWholeWord.UseVisualStyleBackColor = true;
            // 
            // chkSearchUp
            // 
            resources.ApplyResources(this.chkSearchUp, "chkSearchUp");
            this.chkSearchUp.Name = "chkSearchUp";
            this.chkSearchUp.UseVisualStyleBackColor = true;
            // 
            // chkFullSearch
            // 
            resources.ApplyResources(this.chkFullSearch, "chkFullSearch");
            this.chkFullSearch.Checked = true;
            this.chkFullSearch.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkFullSearch.Name = "chkFullSearch";
            this.chkFullSearch.UseVisualStyleBackColor = true;
            // 
            // chkMatchCase
            // 
            resources.ApplyResources(this.chkMatchCase, "chkMatchCase");
            this.chkMatchCase.Name = "chkMatchCase";
            this.chkMatchCase.UseVisualStyleBackColor = true;
            // 
            // frmSearch
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancelSearch;
            this.Controls.Add(this.chkFullSearch);
            this.Controls.Add(this.chkSearchUp);
            this.Controls.Add(this.chkMatchCase);
            this.Controls.Add(this.chkWholeWord);
            this.Controls.Add(this.btnCancelSearch);
            this.Controls.Add(this.btnSearchNext);
            this.Controls.Add(this.btnSearch);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtSearch);
            this.Name = "frmSearch";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtSearch;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSearch;
        private System.Windows.Forms.Button btnSearchNext;
        private System.Windows.Forms.Button btnCancelSearch;
        private System.Windows.Forms.CheckBox chkWholeWord;
        private System.Windows.Forms.CheckBox chkSearchUp;
        private System.Windows.Forms.CheckBox chkFullSearch;
        private System.Windows.Forms.CheckBox chkMatchCase;
    }
}