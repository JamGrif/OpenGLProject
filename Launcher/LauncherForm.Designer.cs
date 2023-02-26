namespace Launcher
{
    partial class LauncherForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LauncherForm));
            LaunchButton = new Button();
            SceneSelectListBox = new ListBox();
            label1 = new Label();
            label2 = new Label();
            GitHub = new LinkLabel();
            YouTube = new LinkLabel();
            label3 = new Label();
            pictureBox1 = new PictureBox();
            pictureBox2 = new PictureBox();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).BeginInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox2).BeginInit();
            SuspendLayout();
            // 
            // LaunchButton
            // 
            LaunchButton.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point);
            LaunchButton.Location = new Point(125, 304);
            LaunchButton.Name = "LaunchButton";
            LaunchButton.Size = new Size(153, 71);
            LaunchButton.TabIndex = 0;
            LaunchButton.TabStop = false;
            LaunchButton.Text = "Launch Demo";
            LaunchButton.UseVisualStyleBackColor = true;
            LaunchButton.Click += LaunchButton_Click;
            // 
            // SceneSelectListBox
            // 
            SceneSelectListBox.FormattingEnabled = true;
            SceneSelectListBox.ItemHeight = 15;
            SceneSelectListBox.Location = new Point(125, 111);
            SceneSelectListBox.Name = "SceneSelectListBox";
            SceneSelectListBox.Size = new Size(153, 184);
            SceneSelectListBox.TabIndex = 1;
            SceneSelectListBox.SelectedIndexChanged += SceneSelectListBox_SelectedIndexChanged;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new Font("Segoe UI", 21.75F, FontStyle.Bold, GraphicsUnit.Point);
            label1.Location = new Point(23, 8);
            label1.Name = "label1";
            label1.Size = new Size(386, 40);
            label1.TabIndex = 3;
            label1.Text = "OpenGL Project - Launcher";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Segoe UI", 11.25F, FontStyle.Bold, GraphicsUnit.Point);
            label2.Location = new Point(124, 84);
            label2.Name = "label2";
            label2.Size = new Size(150, 20);
            label2.TabIndex = 4;
            label2.Text = "Select scene to load:";
            // 
            // GitHub
            // 
            GitHub.AutoSize = true;
            GitHub.Font = new Font("Segoe UI", 12F, FontStyle.Bold, GraphicsUnit.Point);
            GitHub.Location = new Point(80, 382);
            GitHub.Name = "GitHub";
            GitHub.Size = new Size(92, 21);
            GitHub.TabIndex = 5;
            GitHub.TabStop = true;
            GitHub.Text = "My GitHub";
            GitHub.LinkClicked += GitHub_LinkClicked;
            // 
            // YouTube
            // 
            YouTube.AutoSize = true;
            YouTube.Font = new Font("Segoe UI", 12F, FontStyle.Bold, GraphicsUnit.Point);
            YouTube.Location = new Point(222, 382);
            YouTube.Name = "YouTube";
            YouTube.Size = new Size(104, 21);
            YouTube.TabIndex = 6;
            YouTube.TabStop = true;
            YouTube.Text = "My YouTube";
            YouTube.LinkClicked += YouTube_LinkClicked;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Font = new Font("Segoe UI", 14.25F, FontStyle.Bold, GraphicsUnit.Point);
            label3.Location = new Point(148, 48);
            label3.Name = "label3";
            label3.Size = new Size(108, 25);
            label3.TabIndex = 7;
            label3.Text = "by JamGrif";
            // 
            // pictureBox1
            // 
            pictureBox1.Image = (Image)resources.GetObject("pictureBox1.Image");
            pictureBox1.Location = new Point(384, 361);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new Size(50, 50);
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox1.TabIndex = 8;
            pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            pictureBox2.Image = (Image)resources.GetObject("pictureBox2.Image");
            pictureBox2.Location = new Point(384, 311);
            pictureBox2.Name = "pictureBox2";
            pictureBox2.Size = new Size(50, 50);
            pictureBox2.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox2.TabIndex = 9;
            pictureBox2.TabStop = false;
            // 
            // LauncherForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(434, 411);
            Controls.Add(pictureBox2);
            Controls.Add(pictureBox1);
            Controls.Add(label3);
            Controls.Add(YouTube);
            Controls.Add(GitHub);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(SceneSelectListBox);
            Controls.Add(LaunchButton);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Icon = (Icon)resources.GetObject("$this.Icon");
            MaximizeBox = false;
            Name = "LauncherForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "OpenGL Project - Launcher";
            Load += LauncherForm_Load;
            ((System.ComponentModel.ISupportInitialize)pictureBox1).EndInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox2).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button LaunchButton;
        private ListBox SceneSelectListBox;
        private Label label1;
        private Label label2;
        private LinkLabel GitHub;
        private LinkLabel YouTube;
        private Label label3;
        private PictureBox pictureBox1;
        private PictureBox pictureBox2;
    }
}
