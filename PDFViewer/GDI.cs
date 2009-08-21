using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
/* to be example to the xoring problem 
 * i ve written a simple namespace to referance to standat GDI.
 * But the GDIDraw namespace 
 * doesnt referance to whole methots such as drawing 
 * rectangles or other shapes
 * but can easily be enhanged if necessary */
namespace GDIDraw
{
    public enum PenStyles
    {
        PS_SOLID=0
        ,PS_DASH=1
        ,PS_DOT=2
        ,PS_DASHDOT=3
        ,PS_DASHDOTDOT=4
        ,PS_NULL=5
        ,PS_INSIDEFRAME=6
        ,PS_USERSTYLE=7
        ,PS_ALTERNATE=8
        ,PS_STYLE_MASK=0x0000000F

        ,PS_ENDCAP_ROUND=     0x00000000
        ,PS_ENDCAP_SQUARE=    0x00000100
        ,PS_ENDCAP_FLAT=      0x00000200
        ,PS_ENDCAP_MASK =     0x00000F00
        ,PS_JOIN_ROUND=       0x00000000
        ,PS_JOIN_BEVEL=       0x00001000
        ,PS_JOIN_MITER=       0x00002000
        ,PS_JOIN_MASK=        0x0000F000

        ,PS_COSMETIC=         0x00000000
        ,PS_GEOMETRIC=        0x00010000
        ,PS_TYPE_MASK=        0x000F0000
    };
    public enum drawingMode
    {
        R2_BLACK=            1   /*  0       */
        ,R2_NOTMERGEPEN=      2   /* DPon     */
        ,R2_MASKNOTPEN=       3   /* DPna     */
        ,R2_NOTCOPYPEN=       4   /* PN       */
        ,R2_MASKPENNOT=       5   /* PDna     */
        ,R2_NOT=              6   /* Dn       */
        ,R2_XORPEN=           7   /* DPx      */
        ,R2_NOTMASKPEN=       8   /* DPan     */
        ,R2_MASKPEN=          9   /* DPa      */
        ,R2_NOTXORPEN=        10  /* DPxn     */
        ,R2_NOP=              11  /* D        */
        ,R2_MERGENOTPEN=      12  /* DPno     */
        ,R2_COPYPEN=          13  /* P        */
        ,R2_MERGEPENNOT=      14  /* PDno     */
        ,R2_MERGEPEN=         15  /* DPo      */
        ,R2_WHITE=            16  /*  1       */
        ,R2_LAST=             16
    };

    public class GDI
    {
        private IntPtr hdc;
        private System.Drawing.Graphics grp;

        public GDI(IntPtr ptrHdc)
        {
            grp = null;
            hdc = ptrHdc;
        }
        public GDI(System.Drawing.Graphics g)
        {
            grp = g;
            hdc = grp.GetHdc();
        }
        ~GDI()
        {
            if (grp != null)
            {
                grp.ReleaseHdc(hdc);
            }
        }
        
        public void DrawLine(System.Drawing.Color color, System.Drawing.Point p1, System.Drawing.Point p2){
            	
			
			SetROP2(GDIDraw.drawingMode.R2_XORPEN);
			IntPtr gdipen  =CreatePEN(GDIDraw.PenStyles.PS_SOLID,2,GDIDraw.GDI.RGB(color.R,color.G,color.B));
            IntPtr oldpen;
			oldpen=SelectObject(gdipen);
			MoveTo(p1.X,p1.Y);
			LineTo(p2.X,p2.X);
			SelectObject(oldpen);
            DeleteOBJECT(gdipen);
        }
        public IntPtr CreatePEN(
            PenStyles fnPenStyle,    // pen style

            int nWidth,        // pen width

            int crColor   // pen color

            )
        {
           return CreatePen(fnPenStyle,nWidth,crColor);
        }

        public bool DeleteOBJECT(IntPtr hObject)
        {
          return DeleteObject(hObject);
        }

        public IntPtr SelectObject(
            IntPtr hgdiobj   // handle to object

            )
        {
             return SelectObject(hdc,hgdiobj);
        }

        public void MoveTo(int X,int Y)
        {
           MoveToEx(hdc,X,Y,0);
        }

        public void LineTo(int X,int Y)
        {
            LineTo(hdc,X,Y);
        }

        public int SetROP2(drawingMode fnDrawMode)
        {
           return SetROP2(hdc,fnDrawMode);
        }

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        public static extern int SetROP2(
            IntPtr hdc,         // handle of device context

            drawingMode fnDrawMode      // drawing mode

            );  
       
        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")] 
        public static extern bool MoveToEx(
            IntPtr hdc,          // handle to device context

            int X,            // x-coordinate of new current position

            int Y,            // y-coordinate of new current position

            int oldp// pointer to old current position

            );

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        public static extern bool LineTo(
            IntPtr hdc,    // device context handle

            int nXEnd,  // x-coordinate of line's ending point

            int nYEnd   // y-coordinate of line's ending point

            );

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        public static extern IntPtr CreatePen(
            PenStyles fnPenStyle,    // pen style

            int nWidth,        // pen width

            int crColor   // pen color

            );

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        public static extern bool DeleteObject(IntPtr hObject   
                                               // handle to graphic object

            );

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        public static extern IntPtr SelectObject(
            IntPtr hdc,          // handle to device context

            IntPtr hgdiobj   // handle to object

            );

        public static int RGB(int R, int G, int B)
        {
            return (R|(G<<8)|(B<<16));         
        }
        
    
    }

    public class Utils
    {
        public enum AnchorPosition
        {
            Top,
            Center,
            Bottom,
            Left,
            Right
        }


        public static Image Crop(Image imgPhoto, int Width, int Height, AnchorPosition Anchor)
        {
            int sourceWidth = imgPhoto.Width;
            int sourceHeight = imgPhoto.Height;
            int sourceX = 0;
            int sourceY = 0;
            int destX = 0;
            int destY = 0;

            float nPercent = 0;
            float nPercentW = 0;
            float nPercentH = 0;

            nPercentW = ((float)Width / (float)sourceWidth);
            nPercentH = ((float)Height / (float)sourceHeight);

            if (nPercentH < nPercentW)
            {
                nPercent = nPercentW;
                switch (Anchor)
                {
                    case AnchorPosition.Top:
                        destY = 0;
                        break;
                    case AnchorPosition.Bottom:
                        destY = (int)
                            (Height - (sourceHeight * nPercent));
                        break;
                    default:
                        destY = (int)
                            ((Height - (sourceHeight * nPercent)) / 2);
                        break;
                }
            }
            else
            {
                nPercent = nPercentH;
                switch (Anchor)
                {
                    case AnchorPosition.Left:
                        destX = 0;
                        break;
                    case AnchorPosition.Right:
                        destX = (int)
                          (Width - (sourceWidth * nPercent));
                        break;
                    default:
                        destX = (int)
                          ((Width - (sourceWidth * nPercent)) / 2);
                        break;
                }
            }

            int destWidth = (int)(sourceWidth * nPercent);
            int destHeight = (int)(sourceHeight * nPercent);

            Bitmap bmPhoto = new Bitmap(Width,
                    Height, PixelFormat.Format24bppRgb);
            bmPhoto.SetResolution(imgPhoto.HorizontalResolution,
                    imgPhoto.VerticalResolution);

            Graphics grPhoto = Graphics.FromImage(bmPhoto);
            grPhoto.InterpolationMode =
                    InterpolationMode.HighQualityBicubic;

            grPhoto.DrawImage(imgPhoto,
                new Rectangle(destX, destY, destWidth, destHeight),
                new Rectangle(sourceX, sourceY, sourceWidth, sourceHeight),
                GraphicsUnit.Pixel);

            grPhoto.Dispose();
            return bmPhoto;
        }
    }
}