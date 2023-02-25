
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Launcher
{
    public partial class LauncherForm : Form
    {
        public LauncherForm()
        {
            InitializeComponent();
        }

        private void StartProcess(string startSceneName)
        {
            Process? demoProcess = Process.Start(new ProcessStartInfo()
            {
                FileName = "Demo.exe",
                RedirectStandardOutput = false,
                UseShellExecute = false,
                ArgumentList = { startSceneName },
            });

            Console.WriteLine(demoProcess.StartInfo);

        }

        private void materialTestButton_Click(object sender, EventArgs e)
        {
            StartProcess("materialTest");
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void shadowTestButton_Click(object sender, EventArgs e)
        {
            StartProcess("shadowTest");
        }

        private void lightTestButton_Click(object sender, EventArgs e)
        {
            StartProcess("lightTest");
        }

        private void jamieTestButton_Click(object sender, EventArgs e)
        {
            StartProcess("jamieTest");
        }

        private void FMPSceneButton_Click(object sender, EventArgs e)
        {
            StartProcess("FMPscene");
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {

        }
    }
}
