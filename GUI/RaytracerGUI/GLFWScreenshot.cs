using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Windows.Media;
using OpenTK.Graphics.OpenGL4;
using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using PixelFormat = System.Drawing.Imaging.PixelFormat;


namespace RaytracerGUI
{
    class GLFWScreenshot
    {
        public static void Capture(IntPtr glfwWindow, string savePath, int width, int height)
        {
            if (glfwWindow == IntPtr.Zero)
            {
                throw new ArgumentException("Invalid GLFW window handle.");
            }

            byte[] pixelData = new byte[width * height * 4];

            GL.ReadPixels(0, 0, width, height, OpenTK.Graphics.OpenGL4.PixelFormat.Bgra, PixelType.UnsignedByte, pixelData);

            using (Bitmap bitmap = new Bitmap(width, height, PixelFormat.Format32bppArgb))
            {
                BitmapData bitmapData = bitmap.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly, bitmap.PixelFormat);

                byte[] flippedData = FlipImageVertically(pixelData, width, height);

                System.Runtime.InteropServices.Marshal.Copy(flippedData, 0, bitmapData.Scan0, flippedData.Length);

                bitmap.UnlockBits(bitmapData);

                bitmap.Save(savePath, ImageFormat.Png);
            }
        }

        private static byte[] FlipImageVertically(byte[] data, int width, int height)
        {
            int rowSize = width * 4;
            byte[] flippedByteArray = new byte[data.Length];

            for (int row = 0; row < height; row++)
            {
                Array.Copy(data, row * rowSize, flippedByteArray, (height - row - 1) * rowSize, rowSize);
            }

            return flippedByteArray;
        }
    }
}
