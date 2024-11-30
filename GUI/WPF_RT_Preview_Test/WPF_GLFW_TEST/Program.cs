// See https://aka.ms/new-console-template for more information
// Console.WriteLine("Hello, World!");

// ========= DISPLAYS ALL CURRENTLY OPEN WINDOWS ====================
// using System;
// using System.Text;
// using System.Runtime.InteropServices;

// class Program
// {
//     // Import Windows API functions
//     [DllImport("user32.dll")]
//     private static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, IntPtr lParam);

//     [DllImport("user32.dll", CharSet = CharSet.Auto)]
//     private static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);

//     [DllImport("user32.dll")]
//     private static extern bool IsWindowVisible(IntPtr hWnd);

//     private delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);

//     static void Main()
//     {
//         EnumWindows((hWnd, lParam) =>
//         {
//             if (IsWindowVisible(hWnd))
//             {
//                 StringBuilder sb = new StringBuilder(256);
//                 GetWindowText(hWnd, sb, sb.Capacity);
//                 string title = sb.ToString();
//                 if (!string.IsNullOrEmpty(title))
//                 {
//                     Console.WriteLine($"Window: {title}");
//                 }
//             }
//             return true;
//         }, IntPtr.Zero);
//     }
// }


// ========= DISPLAYS ALL CURRENTLY OPEN WINDOWS ====================
using System;
using System.Text;
using System.Drawing;
using static System.Drawing.Image;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

class Program
{
	// ================= GET WINDOW POINTER ============================
	[DllImport("user32.dll", CharSet = CharSet.Unicode)]
	private static extern int GetWindowText(IntPtr hWnd, StringBuilder strText, int maxCount);

	[DllImport("user32.dll", CharSet = CharSet.Unicode)]
	private static extern int GetWindowTextLength(IntPtr hWnd);

	[DllImport("user32.dll")]
	private static extern bool EnumWindows(EnumWindowsProc enumProc, IntPtr lParam);

	[DllImport("user32.dll")]
	private static extern bool IsWindowVisible(IntPtr hWnd);

	// Delegate to filter which windows to include 
	public delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);

	/// <summary> Get the text for the window pointed to by hWnd </summary>
	public static string GetWindowText(IntPtr hWnd)
	{
		int size = GetWindowTextLength(hWnd);
		if (size > 0)
		{
			var builder = new StringBuilder(size + 1);
			GetWindowText(hWnd, builder, builder.Capacity);
			return builder.ToString();
		}

		return String.Empty;
	}

	/// <summary> Find all windows that match the given filter </summary>
	/// <param name="filter"> A delegate that returns true for windows
	///    that should be returned and false for windows that should
	///    not be returned </param>
	public static IEnumerable<IntPtr> FindWindows(EnumWindowsProc filter)
	{
		IntPtr found = IntPtr.Zero;
		List<IntPtr> windows = new List<IntPtr>();

		EnumWindows(delegate (IntPtr wnd, IntPtr param)
		{
			if (filter(wnd, param))
			{
				// only add the windows that pass the filter
				windows.Add(wnd);
			}

			// but return true here so that we iterate all windows
			return true;
		}, IntPtr.Zero);

		return windows;
	}

	/// <summary> Find all windows that contain the given title text </summary>
	/// <param name="titleText"> The text that the window title must contain. </param>
	public static IEnumerable<IntPtr> FindWindowsWithText(string titleText)
	{
		return FindWindows(delegate (IntPtr wnd, IntPtr param)
		{
			return GetWindowText(wnd).Contains(titleText);
		});
	}
	// ==============================================================

	// ================= DRAW TO BITMAP ============================

	[DllImport("user32.dll")]
	private static extern IntPtr GetDC(IntPtr hWnd);

	[DllImport("gdi32.dll")]
	private static extern IntPtr CreateCompatibleDC(IntPtr hdc);

	[DllImport("gdi32.dll")]
	private static extern IntPtr CreateCompatibleBitmap(IntPtr hdc, int nWidth, int nHeight);

	[DllImport("gdi32.dll")]
	private static extern IntPtr SelectObject(IntPtr hdc, IntPtr h);

	[DllImport("gdi32.dll")]
	private static extern bool BitBlt(IntPtr hdcDest, int xDest, int yDest, int wDest, int hDest,
									  IntPtr hdcSrc, int xSrc, int ySrc, int rop);

	[DllImport("gdi32.dll")]
	private static extern bool DeleteDC(IntPtr hdc);

	[DllImport("user32.dll")]
	private static extern bool ReleaseDC(IntPtr hWnd, IntPtr hdc);

	[DllImport("gdi32.dll")]
	private static extern bool DeleteObject(IntPtr hObject);

	const int SRCCOPY = 0x00CC0020;

	static Bitmap CaptureWindow(IntPtr hwnd, int width, int height)
	{
		IntPtr hdcScreen = GetDC(hwnd);
		IntPtr hdcMem = CreateCompatibleDC(hdcScreen);
		IntPtr hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);

		SelectObject(hdcMem, hBitmap);
		BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

		Bitmap bmp = Image.FromHbitmap(hBitmap);

		DeleteObject(hBitmap);
		DeleteDC(hdcMem);
		ReleaseDC(hwnd, hdcScreen);

		return bmp;
	}

	// ==============================================================

	static void Main()
	{
		// Print all visiable windows
		EnumWindows((hWnd, lParam) =>
		{
			if (IsWindowVisible(hWnd))
			{
				StringBuilder sb = new StringBuilder(256);
				GetWindowText(hWnd, sb, sb.Capacity);
				string title = sb.ToString();
				if (!string.IsNullOrEmpty(title))
				{
					Console.WriteLine($"{hWnd}: {title}");
				}
			}
			return true;
		}, IntPtr.Zero);

		// Pass the handle of the window you want to capture (for demonstration, use the desktop handle IntPtr.Zero)
		var windows = FindWindowsWithText("Mozilla Firefox");
		var str = string.Join(", ", windows.ToArray());
		Console.WriteLine($"Has \"Mozialla Firefox\": {str}");

		windows = FindWindowsWithText("RenderWindow"); // Raytracer
		str = string.Join(", ", windows.ToArray());
		Console.WriteLine($"Has \"RenderWindow\": {str}");

		// WILL THROW ERROR IF WINDOW CALLED "RenderWindow" ISN'T FOUND!!!!
		Bitmap screenshot = CaptureWindow(windows.First(), 1920, 1080);
		screenshot.Save("screenshot.png", ImageFormat.Png);
	}
}
