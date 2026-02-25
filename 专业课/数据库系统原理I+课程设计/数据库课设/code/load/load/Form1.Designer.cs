namespace load
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.用户名 = new System.Windows.Forms.Label();
            this.密码 = new System.Windows.Forms.Label();
            this.TxtUser = new System.Windows.Forms.TextBox();
            this.TxtPass = new System.Windows.Forms.TextBox();
            this.BtnOK = new System.Windows.Forms.Button();
            this.BtnExit = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // 用户名
            // 
            this.用户名.AutoSize = true;
            this.用户名.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.用户名.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.用户名.Font = new System.Drawing.Font("宋体", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.用户名.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.用户名.Location = new System.Drawing.Point(444, 276);
            this.用户名.Name = "用户名";
            this.用户名.Size = new System.Drawing.Size(81, 25);
            this.用户名.TabIndex = 0;
            this.用户名.Text = "用户名";
            this.用户名.Click += new System.EventHandler(this.用户名_Click);
            // 
            // 密码
            // 
            this.密码.AutoSize = true;
            this.密码.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.密码.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.密码.Cursor = System.Windows.Forms.Cursors.Default;
            this.密码.Font = new System.Drawing.Font("宋体", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.密码.Location = new System.Drawing.Point(467, 333);
            this.密码.Name = "密码";
            this.密码.Size = new System.Drawing.Size(58, 25);
            this.密码.TabIndex = 1;
            this.密码.Text = "密码";
            this.密码.Click += new System.EventHandler(this.label2_Click);
            // 
            // TxtUser
            // 
            this.TxtUser.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.TxtUser.Location = new System.Drawing.Point(546, 276);
            this.TxtUser.Name = "TxtUser";
            this.TxtUser.Size = new System.Drawing.Size(127, 25);
            this.TxtUser.TabIndex = 2;
            this.TxtUser.TextChanged += new System.EventHandler(this.TxtUser_TextChanged);
            // 
            // TxtPass
            // 
            this.TxtPass.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.TxtPass.Location = new System.Drawing.Point(546, 333);
            this.TxtPass.Name = "TxtPass";
            this.TxtPass.PasswordChar = '*';
            this.TxtPass.Size = new System.Drawing.Size(127, 25);
            this.TxtPass.TabIndex = 3;
            this.TxtPass.TextChanged += new System.EventHandler(this.TxtPass_TextChanged);
            // 
            // BtnOK
            // 
            this.BtnOK.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.BtnOK.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.BtnOK.Cursor = System.Windows.Forms.Cursors.Default;
            this.BtnOK.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.BtnOK.Location = new System.Drawing.Point(430, 391);
            this.BtnOK.Name = "BtnOK";
            this.BtnOK.Size = new System.Drawing.Size(81, 35);
            this.BtnOK.TabIndex = 4;
            this.BtnOK.Text = "登录";
            this.BtnOK.UseVisualStyleBackColor = false;
            this.BtnOK.Click += new System.EventHandler(this.BtnOK_Click);
            // 
            // BtnExit
            // 
            this.BtnExit.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.BtnExit.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.BtnExit.Location = new System.Drawing.Point(628, 392);
            this.BtnExit.Name = "BtnExit";
            this.BtnExit.Size = new System.Drawing.Size(81, 34);
            this.BtnExit.TabIndex = 5;
            this.BtnExit.Text = "退出";
            this.BtnExit.UseVisualStyleBackColor = false;
            this.BtnExit.Click += new System.EventHandler(this.BtnExit_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::load.Properties.Resources.背景图;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.ClientSize = new System.Drawing.Size(1182, 503);
            this.Controls.Add(this.BtnExit);
            this.Controls.Add(this.BtnOK);
            this.Controls.Add(this.TxtPass);
            this.Controls.Add(this.TxtUser);
            this.Controls.Add(this.密码);
            this.Controls.Add(this.用户名);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label 用户名;
        private System.Windows.Forms.Label 密码;
        private System.Windows.Forms.TextBox TxtUser;
        private System.Windows.Forms.TextBox TxtPass;
        private System.Windows.Forms.Button BtnOK;
        private System.Windows.Forms.Button BtnExit;
    }
}

