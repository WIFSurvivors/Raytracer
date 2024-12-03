using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;


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
    private const int GWL_STYLE = -16;
    private const uint WS_CHILD = 0x40000000;
    private const uint WS_VISIBLE = 0x10000000;
    private const int SW_HIDE = 0;
    private const int SW_SHOW = 5;

    private bool _isHandlingSizeChanged = false;
    private nint hWndParent;
    RaytracerGUI.MainWindow _mainWindow;
    public GLFWLoader(RaytracerGUI.MainWindow mainWindow, nint hWndParent)
    {
        _mainWindow = mainWindow;
        this.hWndParent = hWndParent;

    }

    private Rect GetRenderAreaBounds()
    {
        Point location = _mainWindow.RenderArea.TransformToAncestor(_mainWindow).Transform(new Point(0, 0));
        return new Rect
        {
            X = location.X,
            Y = location.Y,
            Width = _mainWindow.RenderArea.ActualWidth,
            Height = _mainWindow.RenderArea.ActualHeight
        };
    }

    public void WindowLoaded()
    {

        IntPtr hWndGLFW = FindGLFWWindow();

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
        IntPtr hWndGLFW = IntPtr.Zero;
        int attempts = 0;
        const int maxAttempts = 10;
        const int delay = 50; // milliseconds

        while (hWndGLFW == IntPtr.Zero && attempts < maxAttempts)
        {
            hWndGLFW = FindWindow(null, "RenderWindow"); // Replace with the actual title of your GLFW window
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
        IntPtr hWndGLFW = FindWindow(null, "RenderWindow"); // Replace with the actual title of your GLFW window

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

