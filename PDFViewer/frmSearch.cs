using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PDFViewer
{
    public partial class frmSearch : Form
    {
        SearchPdfHandler _callback = null;
        bool _fromBegin = true;
        public frmSearch(SearchPdfHandler callback)
        {
            InitializeComponent();
            _callback = callback;
            Load += new EventHandler(frmSearch_Load);
            FormClosing += new FormClosingEventHandler(frmSearch_FormClosing);
        }

        void frmSearch_FormClosing(object sender, FormClosingEventArgs e)
        {
         //   Gma.UserActivityMonitor.HookManager.KeyUp -= new KeyEventHandler(HookManager_KeyUp);
        }

        void frmSearch_Load(object sender, EventArgs e)
        {
           // Gma.UserActivityMonitor.HookManager.KeyUp += new KeyEventHandler(HookManager_KeyUp);
        }

        void HookManager_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
                btnSearch.PerformClick();
            else if (e.KeyCode == Keys.F3)
                btnSearchNext.PerformClick();
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            int res = 0;
            if (_callback != null)
                res = _callback.Invoke(sender, new SearchArgs(
                    txtSearch.Text,
                    true,                   //Search from begin
                    chkMatchCase.Checked,   //Match upper/lower
                    chkFullSearch.Checked,  //Full document
                    false,                  //Find Next
                    chkSearchUp.Checked,    //Backward
                    chkWholeWord.Checked    //Match whole word
                    ));
        }

        private void btnSearchNext_Click(object sender, EventArgs e)
        {
            int res = 0;
            if (_callback != null)
                res = _callback.Invoke(sender, new SearchArgs(
                    txtSearch.Text, 
                    _fromBegin,
                    chkMatchCase.Checked,   //Match upper/lower
                    chkFullSearch.Checked,  //Full document
                    true,                   //Find Next
                    chkSearchUp.Checked,    //backward
                    chkWholeWord.Checked    //Match whole word
                    ));
            _fromBegin = false;
            if (res == 0)
            {
                if (MessageBox.Show("No se encontraron resultados. Desea iniciar la búsqueda desde el principio?", Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    _fromBegin = true;
                    btnSearch.PerformClick();
                }
            }
        }
    }
}