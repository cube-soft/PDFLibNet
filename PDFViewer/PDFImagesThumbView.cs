using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace PDFViewer
{
    public partial class PDFImagesThumbView : ListView
    {
        ImageList _largeImages;
        private class procData
        {
            public PDFLibNet.PDFWrapper pdfDoc;
            public int pageNumber;
            public ListView listView;
            public ImageList imageList;
            public procData(ListView lv, ImageList iml, PDFLibNet.PDFWrapper doc, int page)
            {
                pdfDoc = doc;
                pageNumber = page;
                listView = lv;
                imageList = iml;
            }
        }

        public PDFImagesThumbView()
            : base()
        {
            InitializeComponent();
            _largeImages = new ImageList();
            _largeImages.ImageSize = new Size(64, 64);
            _largeImages.ColorDepth = ColorDepth.Depth24Bit;
            LargeImageList = _largeImages;
            this.View = View.LargeIcon;
        }

        public void LoadImageList(PDFLibNet.PDFWrapper pdfDoc,int pageNumber)
        {
            while (pdfDoc.IsBusy) System.Threading.Thread.Sleep(50);
            System.ComponentModel.BackgroundWorker bg = new BackgroundWorker();
            bg.DoWork += new DoWorkEventHandler(bg_DoWork);
            bg.ProgressChanged += new ProgressChangedEventHandler(bg_ProgressChanged);
            bg.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bg_RunWorkerCompleted);

            //.procData..bg.RunWorkerAsync(new procData(this,_largeImages, pdfDoc,pageNumber));
            this.AddImages(new procData(this, _largeImages, pdfDoc, pageNumber));
        }

        void bg_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            
        }

        void bg_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            
        }

        void bg_DoWork(object sender, DoWorkEventArgs e)
        {
            procData st = (e.Argument as procData);
            while (st.pdfDoc.IsBusy) System.Threading.Thread.Sleep(50);
            int imgCount = st.pdfDoc.Pages[st.pageNumber].ImagesCount;
            st.imageList.Images.Clear();
            st.listView.Items.Clear();
            for (int i = 0; i < imgCount; ++i)
            {
                Image img = st.pdfDoc.Pages[st.pageNumber].GetImage(i);
                Image thumb = GDIDraw.Utils.Crop(img, st.imageList.ImageSize.Width, st.imageList.ImageSize.Height, GDIDraw.Utils.AnchorPosition.Center);
                st.imageList.Images.Add(thumb);
                st.listView.Items.Add("Img" + i.ToString(), i);
            }
        }

        private void AddImages(procData st)
        {
            while (st.pdfDoc.IsBusy) System.Threading.Thread.Sleep(50);
            int imgCount = st.pdfDoc.Pages[st.pageNumber].ImagesCount;
            st.imageList.Images.Clear();
            st.listView.Items.Clear();
            for (int i = 0; i < imgCount; ++i)
            {
                Image img = st.pdfDoc.Pages[st.pageNumber].GetImage(i);
                if (img != null)
                {
                    Image thumb = GDIDraw.Utils.Crop(img, st.imageList.ImageSize.Width, st.imageList.ImageSize.Height, GDIDraw.Utils.AnchorPosition.Center);
                    st.imageList.Images.Add(thumb);
                    st.listView.Items.Add("Img" + i.ToString(), i);
                }
            }
        }


    }
}
