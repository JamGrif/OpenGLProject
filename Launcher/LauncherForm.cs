using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.IO;
using System.Windows.Forms;
using System.Reflection;

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

            Console.WriteLine($"Process Name: {demoProcess.ProcessName}");
            Console.WriteLine($"Starting Scene: {startSceneName}");
        }

        private void LaunchButton_Click(object sender, EventArgs e)
        {
            if (ListStringPressed == null)
                return;

            StartProcess(ListStringPressed);
        }

        private void SceneSelect_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (SceneSelect.SelectedIndex == -1)
                return;

            SceneSelectPicture.Visible = true;

            ListStringPressed = SceneNameList[SceneSelect.SelectedIndex];

            // Change picture

            SceneSelectPicture.ImageLocation = SceneImagePath[SceneSelect.SelectedIndex];
        }

        private List<string>? SceneNameList = null;
        private List<string>? SceneImagePath = null;

        private string? ListStringPressed = string.Empty;

        private void LauncherForm_Load(object sender, EventArgs e)
        {
            string? SceneResourcePath = @"res\scenes";

            string[] files = System.IO.Directory.GetFiles(SceneResourcePath, "*.xml");

            SceneNameList = new List<string>();

            foreach (string item in files)
            {
                SceneNameList.Add(Path.GetFileNameWithoutExtension(item));
            }

            string? SceneImageResourcePath = @"res/scenePictures";

            string[] files2 = System.IO.Directory.GetFiles(SceneImageResourcePath, "*.png");
            SceneImagePath = new List<string>(files2);

            // Make sure first item is not selected
            SceneSelect.SelectedIndexChanged -= SceneSelect_SelectedIndexChanged;
            SceneSelect.DataSource = SceneNameList;
            SceneSelect.SelectedIndex = -1;
            SceneSelect.SelectedIndexChanged += SceneSelect_SelectedIndexChanged;

        }

        private void SceneSelectPicture_Click(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
