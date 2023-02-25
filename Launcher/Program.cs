using System.Runtime.InteropServices;

namespace Launcher
{
    internal static class Program
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool AllocConsole();

        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
#if DEBUG
             AllocConsole();
#endif

            ApplicationConfiguration.Initialize();
            Application.Run(new Form1());
        }
    }

    
}
