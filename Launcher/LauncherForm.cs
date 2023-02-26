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
        // List of all selectable Scene files the user can select
        private List<string> SceneNameList = new List<string>();

        // Which scene name the user has selected in the ListBox
        private string SceneNameListSelected = string.Empty;

        public LauncherForm()
        {
            InitializeComponent();
        }

        // Invoked when the form is loaded
        private void LauncherForm_Load(object sender, EventArgs e)
        {
            string SceneResourcePath = @"res\scenes";

            // Get the filename of all loadable scene files
            string[] AllSceneFiles = System.IO.Directory.GetFiles(SceneResourcePath, "*.xml");
            foreach (string file in AllSceneFiles)
            {
                // Only store name of each scene file
                SceneNameList.Add(Path.GetFileNameWithoutExtension(file));
            }

            // Stops first item in ListBox from being selected by default
            SceneSelectListBox.SelectedIndexChanged -= SceneSelectListBox_SelectedIndexChanged;
            SceneSelectListBox.DataSource = SceneNameList;
            SceneSelectListBox.SelectedIndex = -1;
            SceneSelectListBox.SelectedIndexChanged += SceneSelectListBox_SelectedIndexChanged;

            LaunchButton.Enabled = false;
        }

        // Launch button - Start the Demo.exe process
        private void LaunchButton_Click(object sender, EventArgs e)
        {
            // Ensure a scene has been selected
            if (SceneNameListSelected == string.Empty)
                return;

            StartProcess(SceneNameListSelected);
        }

        // ListBox - Store what scene name user has pressed and update picture as appropriate
        private void SceneSelectListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Ensure valid index
            if (SceneSelectListBox.SelectedIndex == -1)
                return;

            // Update index selection
            SceneNameListSelected = SceneNameList[SceneSelectListBox.SelectedIndex];

            LaunchButton.Enabled = true;
        }

        // Start Demo.exe with the startSceneName scene
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

        private void GitHub_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start(new ProcessStartInfo("https://github.com/JamGrif") { UseShellExecute = true });
        }

        private void YouTube_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start(new ProcessStartInfo("https://www.youtube.com/@JamGrif") { UseShellExecute = true });
        }
    }
}
