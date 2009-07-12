using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PDFViewer
{
    public partial class frmCurrentView : Form
    {
        private float _factor;
        private float _scale;
        
        public frmCurrentView()
        {
            InitializeComponent();
            Factor = (float)pictureBox1.Width / pictureBox1.Height;
            _scale = 0;
        }

        public float FormScale
        {
            get
            {
                return _scale;
            }
            set
            {
                float tranFactor = value/_scale;
                pictureBox1.Width = (int)(_scale * tranFactor);
                pictureBox1.Height = (int)(_scale * tranFactor);
                Width = pictureBox1.Width + 30;
                Height = pictureBox1.Height + 30;
                _scale = value;
            }
        }
       
        public Size FormSize
        {
            set
            {
                if (_scale == 0)
                    _scale = 1f;

                pictureBox1.Width = (int)(value.Width * _scale);
                pictureBox1.Height = (int)(value.Height *_scale);
                _factor = value.Width / value.Height;

                bYaEstoy = true;
                Width = pictureBox1.Width + pictureBox1.Left * 3;
                Height = pictureBox1.Height + pictureBox1.Top * 6;
                bYaEstoy = false;
            }
        }

        public float Factor
        {
            get
            {
                return _factor;
            }
            set
            {
                _factor = value;

            }
        }
        private Rectangle _currentView;
        private Bitmap _bitmap;
        public Rectangle CurrentView
        {
            get
            {
                return _currentView;
            }
            set
            {
                _currentView = value;
                _bitmap =new Bitmap(pictureBox1.Width,pictureBox1.Height);
                using (Graphics g = Graphics.FromImage(_bitmap))
                {
                    g.Clear(Color.White);
                    Rectangle escaledView=new Rectangle((int)(_currentView.X * _scale),(int)(_currentView.Y * _scale), (int)(_currentView.Width * _scale),(int)(_currentView.Height * _scale));
                    g.DrawRectangle(Pens.Blue,Rectangle.Round(escaledView));
                }
                pictureBox1.Image = _bitmap;
                pictureBox1.Refresh();
            }
        }
        private void frmCurrentView_Load(object sender, EventArgs e)
        {

        }
        private bool bYaEstoy = false;
        private void frmCurrentView_Resize(object sender, EventArgs e)
        {
            if (!bYaEstoy)
            {
                int newWidth = Width - 2 * pictureBox1.Left;
                int realWidth = (int)(pictureBox1.Width / _scale);
                _scale = newWidth / realWidth;
                pictureBox1.Width = newWidth;
                pictureBox1.Height = (int)(pictureBox1.Width / _factor);
                bYaEstoy = true;
                Width = pictureBox1.Width + pictureBox1.Left * 3;
                Height = pictureBox1.Height + pictureBox1.Top * 6;
                bYaEstoy=false;
            }
        }
    }

}