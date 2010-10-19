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
        
        public void BeginGDI(System.Drawing.Graphics g)
        {
            grp=g;
            hdc=grp.GetHdc();    
        }

        public void EndGDI()
        {
            grp.ReleaseHdc(hdc);
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
        private static extern int SetROP2(
            IntPtr hdc,         // handle of device context

            drawingMode fnDrawMode      // drawing mode

            );  
       
        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")] 
        private static extern bool MoveToEx(
            IntPtr hdc,          // handle to device context

            int X,            // x-coordinate of new current position

            int Y,            // y-coordinate of new current position

            int oldp// pointer to old current position

            );

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        private static extern bool LineTo(
            IntPtr hdc,    // device context handle

            int nXEnd,  // x-coordinate of line's ending point

            int nYEnd   // y-coordinate of line's ending point

            );

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        private static extern IntPtr CreatePen(
            PenStyles fnPenStyle,    // pen style

            int nWidth,        // pen width

            int crColor   // pen color

            );

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        private static extern bool DeleteObject(IntPtr hObject   
                                               // handle to graphic object

            );

        [System.Runtime.InteropServices.DllImportAttribute(
             "gdi32.dll")]
        private static extern IntPtr SelectObject(
            IntPtr hdc,          // handle to device context

            IntPtr hgdiobj   // handle to object

            );

        public static int RGB(int R,int G,int B)
        {
            return (R|(G<<8)|(B<<16));         
        }
        
    
    }
}