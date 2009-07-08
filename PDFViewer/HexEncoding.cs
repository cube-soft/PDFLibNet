using System;
using System.Collections.Generic;
using System.Text;

namespace PDFViewer
{
    class HexEncoding
    {
        public HexEncoding()
        {
            //
            // TODO: Add constructor logic here
            //
        }
        public static int GetByteCount(string hexString)
        {
            int numHexChars = 0;
            char c;
            // remove all none A-F, 0-9, characters
            for (int i = 0; i < hexString.Length; i++)
            {
                c = hexString[i];
                if (IsHexDigit(c))
                    numHexChars++;
            }
            // if odd number of characters, discard last character
            if (numHexChars % 2 != 0)
            {
                numHexChars--;
            }
            return numHexChars / 2; // 2 characters per byte
        }
        /// <summary>
        /// Creates a byte array from the hexadecimal string. Each two characters are combined
        /// to create one byte. First two hexadecimal characters become first byte in returned array.
        /// Non-hexadecimal characters are ignored. 
        /// </summary>
        /// <param name="hexString">string to convert to byte array</param>
        /// <param name="discarded">number of characters in string ignored</param>
        /// <returns>byte array, in the same left-to-right order as the hexString</returns>
        public static byte[] GetBytes(string hexString, out int discarded)
        {
            discarded = 0;
            string newString = "";
            char c;
            // remove all none A-F, 0-9, characters
            for (int i = 0; i < hexString.Length; i++)
            {
                c = hexString[i];
                if (IsHexDigit(c))
                    newString += c;
                else
                    discarded++;
            }
            // if odd number of characters, discard last character
            if (newString.Length % 2 != 0)
            {
                discarded++;
                newString = newString.Substring(0, newString.Length - 1);
            }

            int byteLength = newString.Length / 2;
            byte[] bytes = new byte[byteLength];
            string hex;
            int j = 0;
            for (int i = 0; i < bytes.Length; i++)
            {
                hex = new String(new Char[] { newString[j], newString[j + 1] });
                bytes[i] = HexToByte(hex);
                j = j + 2;
            }
            return bytes;
        }
        public static string ToString(byte[] bytes)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < bytes.Length; i++)
            {
                sb.Append(bytes[i].ToString("X2"));
            }
            return sb.ToString();
        }
        /// <summary>
        /// Determines if given string is in proper hexadecimal string format
        /// </summary>
        /// <param name="hexString"></param>
        /// <returns></returns>
        public static bool InHexFormat(string hexString)
        {
            bool hexFormat = true;

            foreach (char digit in hexString)
            {
                if (!IsHexDigit(digit))
                {
                    hexFormat = false;
                    break;
                }
            }
            return hexFormat;
        }

        /// <summary>
        /// Returns true is c is a hexadecimal digit (A-F, a-f, 0-9)
        /// </summary>
        /// <param name="c">Character to test</param>
        /// <returns>true if hex digit, false if not</returns>
        public static bool IsHexDigit(Char c)
        {
            int numChar;
            int numA = Convert.ToInt32('A');
            int num1 = Convert.ToInt32('0');
            c = Char.ToUpper(c);
            numChar = Convert.ToInt32(c);
            if (numChar >= numA && numChar < (numA + 6))
                return true;
            if (numChar >= num1 && numChar < (num1 + 10))
                return true;
            return false;
        }
        /// <summary>
        /// Converts 1 or 2 character string into equivalant byte value
        /// </summary>
        /// <param name="hex">1 or 2 character string</param>
        /// <returns>byte</returns>
        private static byte HexToByte(string hex)
        {
            if (hex.Length > 2 || hex.Length <= 0)
                throw new ArgumentException("hex must be 1 or 2 characters in length");
            byte newByte = byte.Parse(hex, System.Globalization.NumberStyles.HexNumber);
            return newByte;
        }

        private static string[] encode1 = new string[] { "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y" };
        private static Dictionary<int, string> encode2 = new Dictionary<int, string>();

        /// <summary>
        /// Comprime la cadena hexadecimal usando el formato de zebra
        /// </summary>
        /// <param name="hexData"></param>
        /// <returns></returns>
        public static string CompressHex(string hexData)
        {
            if (encode2.Count == 0)
            {
                encode2.Add(20, "g"); encode2.Add(40, "h"); encode2.Add(60, "i"); encode2.Add(80, "j");
                encode2.Add(100, "k"); encode2.Add(120, "l"); encode2.Add(140, "m"); encode2.Add(160, "n");
                encode2.Add(180, "o"); encode2.Add(200, "p"); encode2.Add(220, "q"); encode2.Add(240, "r");
                encode2.Add(260, "s"); encode2.Add(280, "t"); encode2.Add(300, "u"); encode2.Add(320, "v");
                encode2.Add(340, "w"); encode2.Add(360, "x"); encode2.Add(380, "y"); encode2.Add(400, "z");
            }
            StringBuilder sb = new StringBuilder();
            string lastChar = hexData.Substring(0, 1);
            int countChar = 0;
            string compressed = "";
            for (int i = 1; i < hexData.Length; i++)
            {
                if (hexData.Substring(i, 1) == lastChar)
                {
                    countChar++;
                }
                else if (countChar > 0)
                {
                    if (countChar > 419)
                    {
                        int div = 2;
                        int res = countChar;
                        int num = 0;
                        while (countChar / div > 419)
                        {
                            div++;
                        }
                        do
                        {
                            if (num == 0)
                            {
                                num = (int)countChar / div;
                                res = countChar - div;
                            }
                            else
                            {
                                if (num > res)
                                    num = res;
                                res -= num;
                            }
                            compressed += getEncodedString(num, lastChar);
                        } while (res > 0);
                    }
                    else
                    {
                        compressed = getEncodedString(countChar, lastChar);
                    }

                    sb.Append(compressed);
                    countChar = 0;
                    lastChar = hexData.Substring(i, 1);

                }
            }
            return sb.ToString();
        }

        private static string getEncodedString(int countChar, string lastChar)
        {
            string compressed = "";
            int modCount = (countChar % 20);
            if (countChar - modCount > 0)
            {
                compressed = encode2[countChar - modCount];
            }
            if (modCount > 0)
            {
                compressed += encode1[modCount - 1];
            }
            compressed += lastChar;
            return compressed;
        }
    }
}
