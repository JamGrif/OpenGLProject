
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Launcher
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void StartProcess(string? text)
        {
            string exeFilepath = "Demo.exe";
            var rest = new ProcessStartInfo()
            {
                FileName = exeFilepath,
                RedirectStandardOutput = false,
                UseShellExecute = false,
                ArgumentList = { text },
            };

            Process? demoProcess = Process.Start(rest);
            Console.WriteLine(demoProcess.ProcessName);

        }

        private void button1_Click(object sender, EventArgs e)
        {
            StartProcess("materialTest");
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            StartProcess("shadowTest");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            StartProcess("lightTest");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            StartProcess("jamieTest");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            StartProcess("FMPscene");
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click_1(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void process1_Exited(object sender, EventArgs e)
        {

        }
    }
}
