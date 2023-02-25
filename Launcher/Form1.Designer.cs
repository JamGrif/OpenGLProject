namespace Launcher
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            button1 = new Button();
            button2 = new Button();
            button3 = new Button();
            button4 = new Button();
            button5 = new Button();
            pictureBox1 = new PictureBox();
            pictureBox2 = new PictureBox();
            pictureBox3 = new PictureBox();
            pictureBox4 = new PictureBox();
            pictureBox5 = new PictureBox();
            textBox1 = new TextBox();
            label1 = new Label();
            process1 = new System.Diagnostics.Process();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).BeginInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox2).BeginInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox3).BeginInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox4).BeginInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox5).BeginInit();
            SuspendLayout();
            // 
            // button1
            // 
            button1.Location = new Point(472, 289);
            button1.Name = "button1";
            button1.Size = new Size(150, 80);
            button1.TabIndex = 0;
            button1.Text = "materialTest";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // button2
            // 
            button2.Location = new Point(628, 289);
            button2.Name = "button2";
            button2.Size = new Size(150, 80);
            button2.TabIndex = 2;
            button2.Text = "shadowTest";
            button2.UseVisualStyleBackColor = true;
            button2.Click += button2_Click;
            // 
            // button3
            // 
            button3.Location = new Point(316, 289);
            button3.Name = "button3";
            button3.Size = new Size(150, 80);
            button3.TabIndex = 3;
            button3.Text = "lightTest";
            button3.UseVisualStyleBackColor = true;
            button3.Click += button3_Click;
            // 
            // button4
            // 
            button4.Location = new Point(160, 289);
            button4.Name = "button4";
            button4.Size = new Size(150, 80);
            button4.TabIndex = 4;
            button4.Text = "jamieTest";
            button4.UseVisualStyleBackColor = true;
            button4.Click += button4_Click;
            // 
            // button5
            // 
            button5.Location = new Point(4, 289);
            button5.Name = "button5";
            button5.Size = new Size(150, 80);
            button5.TabIndex = 5;
            button5.Text = "FMPscene";
            button5.UseVisualStyleBackColor = true;
            button5.Click += button5_Click;
            // 
            // pictureBox1
            // 
            pictureBox1.Image = Properties.Resources.testPicture;
            pictureBox1.Location = new Point(4, 123);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new Size(150, 160);
            pictureBox1.TabIndex = 6;
            pictureBox1.TabStop = false;
            pictureBox1.Click += pictureBox1_Click_1;
            // 
            // pictureBox2
            // 
            pictureBox2.Image = Properties.Resources.testPicture;
            pictureBox2.Location = new Point(160, 123);
            pictureBox2.Name = "pictureBox2";
            pictureBox2.Size = new Size(150, 160);
            pictureBox2.TabIndex = 7;
            pictureBox2.TabStop = false;
            // 
            // pictureBox3
            // 
            pictureBox3.Image = Properties.Resources.testPicture;
            pictureBox3.Location = new Point(316, 123);
            pictureBox3.Name = "pictureBox3";
            pictureBox3.Size = new Size(150, 160);
            pictureBox3.TabIndex = 8;
            pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            pictureBox4.Image = Properties.Resources.testPicture;
            pictureBox4.Location = new Point(472, 123);
            pictureBox4.Name = "pictureBox4";
            pictureBox4.Size = new Size(150, 160);
            pictureBox4.TabIndex = 9;
            pictureBox4.TabStop = false;
            // 
            // pictureBox5
            // 
            pictureBox5.Image = Properties.Resources.testPicture;
            pictureBox5.Location = new Point(628, 123);
            pictureBox5.Name = "pictureBox5";
            pictureBox5.Size = new Size(150, 160);
            pictureBox5.TabIndex = 10;
            pictureBox5.TabStop = false;
            // 
            // textBox1
            // 
            textBox1.Location = new Point(316, 25);
            textBox1.Multiline = true;
            textBox1.Name = "textBox1";
            textBox1.ReadOnly = true;
            textBox1.RightToLeft = RightToLeft.No;
            textBox1.Size = new Size(166, 37);
            textBox1.TabIndex = 11;
            textBox1.Text = "OpenGL Project - Launcher - Really Cool";
            textBox1.TextAlign = HorizontalAlignment.Center;
            textBox1.TextChanged += textBox1_TextChanged;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.BackColor = Color.Transparent;
            label1.Location = new Point(179, 76);
            label1.Name = "label1";
            label1.Size = new Size(150, 15);
            label1.TabIndex = 12;
            label1.Text = "OpenGL Project - Launcher";
            label1.Click += label1_Click;
            // 
            // process1
            // 
            process1.StartInfo.Domain = "";
            process1.StartInfo.LoadUserProfile = false;
            process1.StartInfo.Password = null;
            process1.StartInfo.StandardErrorEncoding = null;
            process1.StartInfo.StandardInputEncoding = null;
            process1.StartInfo.StandardOutputEncoding = null;
            process1.StartInfo.UserName = "";
            process1.SynchronizingObject = this;
            process1.Exited += process1_Exited;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            BackgroundImage = Properties.Resources.WindowBackground;
            BackgroundImageLayout = ImageLayout.Stretch;
            ClientSize = new Size(784, 461);
            Controls.Add(label1);
            Controls.Add(textBox1);
            Controls.Add(pictureBox5);
            Controls.Add(pictureBox4);
            Controls.Add(pictureBox3);
            Controls.Add(pictureBox2);
            Controls.Add(pictureBox1);
            Controls.Add(button5);
            Controls.Add(button4);
            Controls.Add(button3);
            Controls.Add(button2);
            Controls.Add(button1);
            DoubleBuffered = true;
            FormBorderStyle = FormBorderStyle.FixedSingle;
            Icon = (Icon)resources.GetObject("$this.Icon");
            MaximizeBox = false;
            Name = "Form1";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "HelloWorld";
            Load += Form1_Load;
            ((System.ComponentModel.ISupportInitialize)pictureBox1).EndInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox2).EndInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox3).EndInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox4).EndInit();
            ((System.ComponentModel.ISupportInitialize)pictureBox5).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button button1;
        private Button button2;
        private Button button3;
        private Button button4;
        private Button button5;
        private PictureBox pictureBox1;
        private PictureBox pictureBox2;
        private PictureBox pictureBox3;
        private PictureBox pictureBox4;
        private PictureBox pictureBox5;
        private TextBox textBox1;
        private Label label1;
        private System.Diagnostics.Process process1;
    }
}
