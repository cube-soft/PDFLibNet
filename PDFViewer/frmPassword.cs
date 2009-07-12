using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PDFViewer
{
    public partial class frmPassword : Form
    {
        public frmPassword()
        {
            InitializeComponent();
        }

        public string UserPassword
        {
            get
            {
                return txtUserPwd.Text;
            }
        }
        public string OwnerPassword
        {
            get
            {
                return txtOwnPwd.Text;
            }
        }

        private void cmdAccept_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}