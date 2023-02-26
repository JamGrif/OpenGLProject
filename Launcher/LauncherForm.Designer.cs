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
            SceneSelect = new ListBox();
            SceneSelectPicture = new PictureBox();
            label1 = new Label();
            label2 = new Label();
            ((System.ComponentModel.ISupportInitialize)SceneSelectPicture).BeginInit();
            SuspendLayout();
            // 
            // LaunchButton
            // 
            LaunchButton.Font = new Font("Segoe UI", 12F, FontStyle.Regular, GraphicsUnit.Point);
            LaunchButton.Location = new Point(20, 312);
            LaunchButton.Name = "LaunchButton";
            LaunchButton.Size = new Size(153, 71);
            LaunchButton.TabIndex = 0;
            LaunchButton.TabStop = false;
            LaunchButton.Text = "Launch Demo";
            LaunchButton.UseVisualStyleBackColor = true;
            LaunchButton.Click += LaunchButton_Click;
            // 
            // SceneSelect
            // 
            SceneSelect.FormattingEnabled = true;
            SceneSelect.ItemHeight = 15;
            SceneSelect.Location = new Point(20, 108);
            SceneSelect.Name = "SceneSelect";
            SceneSelect.Size = new Size(153, 184);
            SceneSelect.TabIndex = 1;
            SceneSelect.SelectedIndexChanged += SceneSelect_SelectedIndexChanged;
            // 
            // SceneSelectPicture
            // 
            SceneSelectPicture.Enabled = false;
            SceneSelectPicture.Location = new Point(196, 108);
            SceneSelectPicture.Name = "SceneSelectPicture";
            SceneSelectPicture.Size = new Size(209, 184);
            SceneSelectPicture.SizeMode = PictureBoxSizeMode.StretchImage;
            SceneSelectPicture.TabIndex = 2;
            SceneSelectPicture.TabStop = false;
            SceneSelectPicture.Click += SceneSelectPicture_Click;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new Font("Segoe UI", 21.75F, FontStyle.Bold, GraphicsUnit.Point);
            label1.Location = new Point(23, 24);
            label1.Name = "label1";
            label1.Size = new Size(386, 40);
            label1.TabIndex = 3;
            label1.Text = "OpenGL Project - Launcher";
            label1.Click += label1_Click;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Segoe UI", 11.25F, FontStyle.Bold, GraphicsUnit.Point);
            label2.Location = new Point(19, 86);
            label2.Name = "label2";
            label2.Size = new Size(150, 20);
            label2.TabIndex = 4;
            label2.Text = "Select scene to load:";
            // 
            // LauncherForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(434, 411);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(SceneSelectPicture);
            Controls.Add(SceneSelect);
            Controls.Add(LaunchButton);
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Icon = (Icon)resources.GetObject("$this.Icon");
            MaximizeBox = false;
            Name = "LauncherForm";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "OpenGL Project - Launcher";
            Load += LauncherForm_Load;
            ((System.ComponentModel.ISupportInitialize)SceneSelectPicture).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button LaunchButton;
        private ListBox SceneSelect;
        private PictureBox SceneSelectPicture;
        private Label label1;
        private Label label2;
    }
}
