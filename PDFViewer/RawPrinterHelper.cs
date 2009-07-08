using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.IO;

namespace PDFViewer
{
    public class PrintingUtils
    {
        public static Color ColorToGrey(Color c)
        {
            int lum = (int)(c.R * 0.3 + c.G * 0.59 + c.B * 0.11);
            return Color.FromArgb(lum, lum, lum);
        }

        /// <summary>
        /// Convertir a escala de grises, es necesario optimizarlo con LockBitmap
        /// </summary>
        /// <param name="Bitmap"></param>
        /// <returns></returns>
        public static Bitmap GrayScale(System.Drawing.Bitmap Bitmap)
        {
            System.Drawing.Bitmap
              bitmap = (System.Drawing.Bitmap)Bitmap.Clone();

            System.Drawing.Color color;

            for (System.Int32 i = 0; i < bitmap.Width; i++)
            {
                for (System.Int32 j = 0; j < bitmap.Height; j++)
                {
                    color = bitmap.GetPixel(i, j);
                    color = ColorToGrey(color);
                    Bitmap.SetPixel(i, j, color);
                }
            }
            return Bitmap;
        }

        private static void SetIndexedPixel(int x, int y, BitmapData bmd, bool pixel)
        {
            int index = y * bmd.Stride + (x >> 3);
            byte p = Marshal.ReadByte(bmd.Scan0, index);
            byte mask = (byte)(0x80 >> ((int)x & 0x7));
            if (pixel)
            {
                p = (byte)((int)p | (int)mask);
            }
            else
            {
                p = (byte)((int)p & (int)(byte)(mask ^ 0xFF));
            }
            Marshal.WriteByte(bmd.Scan0, index, p);
        }

        private static int BitsPerPixel(Bitmap img)
        {
            switch (img.PixelFormat)
            {
                case PixelFormat.Format8bppIndexed:
                    return 8;
                case PixelFormat.Format32bppRgb:
                case PixelFormat.Format32bppArgb:
                    return 32;
                case PixelFormat.Format16bppArgb1555:
                case PixelFormat.Format16bppGrayScale:
                case PixelFormat.Format16bppRgb555:
                case PixelFormat.Format16bppRgb565:
                    return 16;
                case PixelFormat.Format48bppRgb:
                    return 48;
                case PixelFormat.Format4bppIndexed:
                    return 4;
                case PixelFormat.Format1bppIndexed:
                    return 1;
                case PixelFormat.Format24bppRgb:
                    return 24;
                default:
                    throw new Exception("Resources.UIStrings.ErrorUnsoportedPixelFormat");
            }
        }

        /// <summary>
        /// Convertir a blanco y negro en 1 bit.
        /// </summary>
        /// <param name="imgSource">Bitmap a convertir</param>
        /// <returns>Un bitmap a 1 bit de profundidad</returns>
        /// 
        public static Bitmap Monocrome(Bitmap imgSource, bool inverted)
        {
            Rectangle rect = new Rectangle(0, 0, imgSource.Width, imgSource.Height);
            Bitmap imgDest = new Bitmap(imgSource.Width, imgSource.Height, PixelFormat.Format1bppIndexed);
            BitmapData bmdo = imgSource.LockBits(rect, ImageLockMode.ReadOnly, imgSource.PixelFormat);
            BitmapData bmdd = imgDest.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format1bppIndexed);

            for (int y = 0; y < imgDest.Height; y++)
            {
                for (int x = 0; x < imgDest.Width; x++)
                {
                    int index = y * bmdo.Stride + x * BitsPerPixel(imgSource) / 8;
                    Color color = Color.FromArgb(Marshal.ReadByte(bmdo.Scan0, index + 2),
                                        Marshal.ReadByte(bmdo.Scan0, index + 1),
                                        Marshal.ReadByte(bmdo.Scan0, index));
                    if ((!inverted && color.GetBrightness() > 0.55f) || (inverted && inverted && color.GetBrightness() < 0.55f))
                    {
                        SetIndexedPixel(x, y, bmdd, true);
                    }
                }
            }
            imgDest.UnlockBits(bmdd);
            imgSource.UnlockBits(bmdo);
            return imgDest;
        }

        /// <summary>
        /// Cambiar tamaño de imagen, utiliza metodos nativos de GDI+
        /// </summary>
        /// <param name="bitmap"></param>
        /// <param name="newSize"></param>
        /// <returns></returns>
        public static Bitmap ResizeBitmap(Bitmap bitmap, SizeF newSize)
        {
            Bitmap result = new Bitmap((int)newSize.Width, (int)newSize.Height);
            using (Graphics g = Graphics.FromImage((Image)result))
                g.DrawImage(bitmap, 0, 0, newSize.Width, newSize.Height);
            return result;
        }

        /// <summary>
        /// Obtener una imagen desde la informacion RAW en formato hexadecimal
        /// </summary>
        /// <param name="hexRaw">RAW en hexadecimal</param>
        /// <param name="pixelFormat">Profundidad en bits</param>
        /// <param name="size">Tamaño de la imagen (ancho x alto)</param>
        /// <returns>Imagen creada</returns>
        public static Bitmap RawToImage(string hexRaw, PixelFormat pixelFormat, Size size)
        {
            Bitmap bmp = null;
            BitmapData bd = null;
            try
            {
                int discarded = 0;
                byte[] raw = HexEncoding.GetBytes(hexRaw, out discarded);
                bmp = new Bitmap(size.Width, size.Height, pixelFormat);
                bd = bmp.LockBits(new Rectangle(Point.Empty, size), ImageLockMode.WriteOnly, pixelFormat);
                Marshal.Copy(raw, 0, bd.Scan0, raw.Length);
                return bmp;
            }
            finally
            {
                if (bd != null)
                    bmp.UnlockBits(bd);
            }
        }

        /// <summary>
        /// Obtener la informacion RAW en hexadecimal de la imagen
        /// </summary>
        /// <param name="bmBitmap"></param>
        /// <returns></returns>
        public static string ImageToRaw(Bitmap bmBitmap)
        {
            System.Drawing.Imaging.BitmapData raw = null;//used to get  attributes of the image
            byte[] rawImage = null; //the image as a byte[]

            try
            {
                if (bmBitmap.PixelFormat != PixelFormat.Format1bppIndexed)
                    bmBitmap = Monocrome(bmBitmap, false);
                //'Freeze the image in memory
                raw = bmBitmap.LockBits(new Rectangle(0, 0, bmBitmap.Width, bmBitmap.Height),
                                        System.Drawing.Imaging.ImageLockMode.ReadOnly,
                                        System.Drawing.Imaging.PixelFormat.Format1bppIndexed);
                int size = raw.Height * raw.Stride;
                rawImage = new byte[size];

                //Copy the image into the byte()
                System.Runtime.InteropServices.Marshal.Copy(raw.Scan0, rawImage, 0, size);
                return HexEncoding.ToString(rawImage);
            }
            finally
            {
                if (raw != null)
                {
                    //'Unfreeze the memory for the image
                    bmBitmap.UnlockBits(raw);
                }
            }
        }
    }

    public class RawPrinterHelper
    {
        // Structure and API declarions:
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public class DOCINFOA
        {
            [MarshalAs(UnmanagedType.LPStr)]
            public string pDocName;
            [MarshalAs(UnmanagedType.LPStr)]
            public string pOutputFile;
            [MarshalAs(UnmanagedType.LPStr)]
            public string pDataType;
        }
        [DllImport("winspool.Drv", EntryPoint = "OpenPrinterA", SetLastError = true, CharSet = CharSet.Ansi, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool OpenPrinter([MarshalAs(UnmanagedType.LPStr)] string szPrinter, out IntPtr hPrinter, IntPtr pd);

        [DllImport("winspool.Drv", EntryPoint = "ClosePrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool ClosePrinter(IntPtr hPrinter);

        [DllImport("winspool.Drv", EntryPoint = "StartDocPrinterA", SetLastError = true, CharSet = CharSet.Ansi, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool StartDocPrinter(IntPtr hPrinter, Int32 level, [In, MarshalAs(UnmanagedType.LPStruct)] DOCINFOA di);

        [DllImport("winspool.Drv", EntryPoint = "EndDocPrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool EndDocPrinter(IntPtr hPrinter);

        [DllImport("winspool.Drv", EntryPoint = "StartPagePrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool StartPagePrinter(IntPtr hPrinter);

        [DllImport("winspool.Drv", EntryPoint = "EndPagePrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool EndPagePrinter(IntPtr hPrinter);

        [DllImport("winspool.Drv", EntryPoint = "WritePrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool WritePrinter(IntPtr hPrinter, IntPtr pBytes, Int32 dwCount, out Int32 dwWritten);

        [DllImport("gdi32.dll")]
        private static extern Int32 GetDeviceCaps(IntPtr hdc, Int32 capindex);

        // SendBytesToPrinter()
        // When the function is given a printer name and an unmanaged array
        // of bytes, the function sends those bytes to the print queue.
        // Returns true on success, false on failure.
        public static bool SendBytesToPrinter(string szPrinterName, IntPtr pBytes, Int32 dwCount)
        {
            Int32 dwError = 0, dwWritten = 0;
            IntPtr hPrinter = new IntPtr(0);
            DOCINFOA di = new DOCINFOA();
            bool bSuccess = false; // Assume failure unless you specifically succeed.

            di.pDocName = "My C#.NET RAW Document";
            di.pDataType = "RAW";

            // Open the printer.
            if (OpenPrinter(szPrinterName.Normalize(), out hPrinter, IntPtr.Zero))
            {
                // Start a document.
                if (StartDocPrinter(hPrinter, 1, di))
                {
                    // Start a page.
                    if (StartPagePrinter(hPrinter))
                    {
                        // Write your bytes.
                        bSuccess = WritePrinter(hPrinter, pBytes, dwCount, out dwWritten);
                        EndPagePrinter(hPrinter);
                    }
                    EndDocPrinter(hPrinter);
                }
                ClosePrinter(hPrinter);
            }
            // If you did not succeed, GetLastError may give more information
            // about why not.
            if (bSuccess == false)
            {
                dwError = Marshal.GetLastWin32Error();
            }
            return bSuccess;
        }
        public static bool SendFileToPrinter(string szPrinterName, string szFileName)
        {
            // Open the file.
            FileStream fs = new FileStream(szFileName, FileMode.Open);
            // Create a BinaryReader on the file.
            BinaryReader br = new BinaryReader(fs);
            // Dim an array of bytes big enough to hold the file's contents.
            Byte[] bytes = new Byte[fs.Length];
            bool bSuccess = false;
            // Your unmanaged pointer.
            IntPtr pUnmanagedBytes = new IntPtr(0);
            int nLength;

            nLength = Convert.ToInt32(fs.Length);

            // Read the contents of the file into the array.
            bytes = br.ReadBytes(nLength);
            // Allocate some unmanaged memory for those bytes.
            pUnmanagedBytes = Marshal.AllocCoTaskMem(nLength);
            // Copy the managed byte array into the unmanaged array.
            Marshal.Copy(bytes, 0, pUnmanagedBytes, nLength);
            // Send the unmanaged bytes to the printer.
            bSuccess = SendBytesToPrinter(szPrinterName, pUnmanagedBytes, nLength);
            // Free the unmanaged memory that you allocated earlier.
            Marshal.FreeCoTaskMem(pUnmanagedBytes);
            return bSuccess;
        }
        public static bool SendStringToPrinter(string szPrinterName, string szString)
        {
            IntPtr pBytes;
            Int32 dwCount;
            // How many characters are in the string?
            dwCount = szString.Length;
            // Assume that the printer is expecting ANSI text, and then convert
            // the string to ANSI text.
            pBytes = Marshal.StringToCoTaskMemAnsi(szString);
            // Send the converted ANSI string to the printer.
            SendBytesToPrinter(szPrinterName, pBytes, dwCount);
            Marshal.FreeCoTaskMem(pBytes);
            return true;
        }

        public static int DotsPerMM(System.Drawing.Printing.PrinterSettings pr)
        {/*
            Graphics g=null;
            IntPtr hdc= IntPtr.Zero;
            try
            {

                if (pr != null)
                {
                    g = pr.CreateMeasurementGraphics();
                    hdc = g.GetHdc();
                    return (int)(GetDeviceCaps(hdc, 8)/25.375f);
                }
                else
                    return 8;
            }
            finally
            {
                if(g!=null)
                    g.ReleaseHdc(hdc);
            }*/
            if (pr != null)
            {
                return (int)(pr.DefaultPageSettings.PrinterResolution.X / 25.375f);
            }
            else
                return 8;
        }
    }

}
