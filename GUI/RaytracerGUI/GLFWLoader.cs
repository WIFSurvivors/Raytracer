using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;

namespace RaytracerGUI
{
    public class GLFWLoader
    {
        [DllImport("user32.dll", SetLastError = true)]
        private static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern int SetWindowLong(IntPtr hWnd, int nIndex, uint dwNewLong);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern uint GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool IsWindow(IntPtr hWnd);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool IsWindowVisible(IntPtr hWnd);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool IsWindowEnabled(IntPtr hWnd);

        [DllImport("kernel32.dll", SetLastError = true)]

        private static extern uint GetLastError();

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, IntPtr lParam);

        private const UInt32 WM_CLOSE = 0x0010;




        private const int GWL_STYLE = -16;
        private const uint WS_CHILD = 0x40000000;
        private const uint WS_VISIBLE = 0x10000000;
        private const int SW_HIDE = 0;
        private const int SW_SHOW = 5;
        private IntPtr hWndGLFW;
        private bool _isHandlingSizeChanged = false;
        private nint hWndParent;
        RaytracerGUI.MainWindow _mainWindow;
        public GLFWLoader(RaytracerGUI.MainWindow mainWindow, nint hWndParent)
        {
            _mainWindow = mainWindow;
            this.hWndParent = hWndParent;

        }
        public void CloseWindow()
        {
            SendMessage(hWndGLFW, WM_CLOSE, IntPtr.Zero, IntPtr.Zero);
        }

        private Rect GetRenderAreaBounds()
        {
            // Get the scaling factor for the current window
            double scalingFactor = DpiHelper.GetScalingFactor(new System.Windows.Interop.WindowInteropHelper(_mainWindow).Handle);

            // Get the position and size of the render area in WPF units
            Point location = _mainWindow.rctRenderArea.TransformToAncestor(_mainWindow).Transform(new Point(0, 0));
            double width = _mainWindow.rctRenderArea.ActualWidth;
            double height = _mainWindow.rctRenderArea.ActualHeight;

            // Scale the dimensions to account for DPI scaling
            return new Rect
            {
                X = location.X * scalingFactor,
                Y = location.Y * scalingFactor,
                Width = width * scalingFactor,
                Height = height * scalingFactor
            };
        }


        public void WindowLoaded()
        {

            hWndGLFW = FindGLFWWindow();

            if (hWndGLFW != IntPtr.Zero)
            {

                // Check if the GLFW window is valid and responsive
                if (!IsWindow(hWndGLFW) || !IsWindowVisible(hWndGLFW) || !IsWindowEnabled(hWndGLFW))
                {
                    MessageBox.Show("GLFW window is not responding or not in a valid state.");
                    return;
                }

                // Set the GLFW window as a child of the WPF window
                IntPtr result = SetParent(hWndGLFW, hWndParent);
                if (result == IntPtr.Zero)
                {
                    uint error = GetLastError();
                    MessageBox.Show($"Failed to set parent for GLFW window. Error code: {error}");
                    return;
                }

                // Adjust window styles
                uint styles = GetWindowLong(hWndGLFW, GWL_STYLE);
                SetWindowLong(hWndGLFW, GWL_STYLE, styles | WS_CHILD | WS_VISIBLE);

                // Get the bounds of the render area
                Rect bounds = GetRenderAreaBounds();
                bool moveResult = MoveWindow(hWndGLFW, (int)bounds.X, (int)bounds.Y, (int)bounds.Width, (int)bounds.Height, true);

                if (!moveResult)
                {
                    MessageBox.Show("Failed to move GLFW window.");
                }

                // Show the GLFW window
                ShowWindow(hWndGLFW, SW_SHOW);
            }
            else
            {
                MessageBox.Show("GLFW window not found. Make sure it is created before loading the WPF window.");
            }
        }

        private IntPtr FindGLFWWindow()
        {
            hWndGLFW = IntPtr.Zero;
            int attempts = 0;
            const int maxAttempts = 10;
            const int delay = 50; // milliseconds

            while (hWndGLFW == IntPtr.Zero && attempts < maxAttempts)
            {
                hWndGLFW = FindWindow(null, "RenderWindow");
                if (hWndGLFW == IntPtr.Zero)
                {
                    attempts++;
                    System.Threading.Thread.Sleep(delay);
                }
            }

            return hWndGLFW;
        }

        internal void OnResize()
        {
            if (_isHandlingSizeChanged)
                return;

            _isHandlingSizeChanged = true;

            // Adjust the size of the embedded GLFW window if necessary
            if (hWndGLFW != IntPtr.Zero)
            {
                Rect bounds = GetRenderAreaBounds();
                bool moveResult = MoveWindow(hWndGLFW, (int)bounds.X, (int)bounds.Y, (int)bounds.Width, (int)bounds.Height, true);

                if (!moveResult)
                {
                    MessageBox.Show("Failed to move GLFW window.");
                }
            }

            _isHandlingSizeChanged = false;
        }

    }

}