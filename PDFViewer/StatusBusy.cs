using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
namespace PDFViewer
{
    class StatusBusy: IDisposable
    {
        private string _oldStatus;
//        private DevExpress.XtraBars.BarItemVisibility _oldWbStatus;
        private Cursor _oldCursor;

        

        public StatusBusy(string statusText)
        {
            _oldStatus = frmPDFViewer.Instance.StatusLabel.Text;
            //_oldWbStatus = FormMainProvider.WorkingBar.Visibility;
            _oldCursor = frmPDFViewer.Instance.Cursor;

            frmPDFViewer.Instance.StatusLabel.Text = statusText;
            //FormMainProvider.WorkingBar.Visibility = DevExpress.XtraBars.BarItemVisibility.Always;
            frmPDFViewer.Instance.Cursor = Cursors.WaitCursor;
            Application.DoEvents();
        }

        #region IDisposable
        // IDisposable
        private bool _disposedValue = false; // Evitar llamadas recursivas

        protected void Dispose(bool disposing)
        {
            if (!_disposedValue)
                if (disposing)
                {
                    frmPDFViewer.Instance.StatusLabel.Text = _oldStatus;
                    frmPDFViewer.Instance.Cursor = _oldCursor;
                    //FormMainProvider.WorkingBar.Visibility = _oldWbStatus;
                }
            _disposedValue = true;
        }

        public void Dispose()
        {
            //No cambiar este codigo, limpiar codigo en la funcion protected void Dispose(bool disposin)
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion
    }
}
