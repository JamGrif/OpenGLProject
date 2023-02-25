using System.Runtime.InteropServices;

namespace Launcher
{
    internal static class Program
    {
#if DEBUG
        // Acquire Console specific functions

        [DllImport("kernel32.dll", SetLastError = true)]
        //[return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool AllocConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        internal static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll", SetLastError = true)]
        internal static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);
#endif

        [STAThread]
        static void Main()
        {

#if DEBUG
            // Enable console if in debug mode
            AllocConsole();
            MoveWindow(GetConsoleWindow(), -900, 25, 900, 500, true);
#endif

            ApplicationConfiguration.Initialize();
            Application.Run(new LauncherForm());
        }
    }
}
