using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace load
{
    public partial class ChangePasswordForm : Form
    {
        private readonly string studentID;
        private readonly string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private readonly string originalPwd;

        public ChangePasswordForm(string sid)
        {
            InitializeComponent();
            studentID = sid;

            // 1) 构造时只查询一次“旧密码”
            using (var conn = new SqlConnection(connStr))
            using (var cmd = new SqlCommand("SELECT 密码 FROM UserAccount WHERE 用户编号 = @uid", conn))
            {
                cmd.Parameters.AddWithValue("@uid", studentID);
                conn.Open();
                originalPwd = (cmd.ExecuteScalar()?.ToString() ?? "").Trim();
            }

            // 2) 绑定按钮事件
            
            button2.Click += (s, e) => this.Close();

            // 3) 三个框都隐藏输入
            txtOldPwd.UseSystemPasswordChar = false;
            txtNewPwd.UseSystemPasswordChar = true;
            txtConfirmPwd.UseSystemPasswordChar = true;
        }


        // 1. 把 VerifyPassword 改成不弹窗，只返回 true/false
        private bool VerifyPassword(string userName, string oldPwd)
        {
            // originalPwd 已经在构造里读取并存在字段里了
            return oldPwd == originalPwd;
        }

        // 2. 在 BtnOK_Click 里一次性处理所有提示
        private void button1_Click(object sender, EventArgs e)
        {
            string oldPwd = txtOldPwd.Text.Trim();
            string newPwd1 = txtNewPwd.Text.Trim();
            string newPwd2 = txtConfirmPwd.Text.Trim();

            // 非空检查
            if (string.IsNullOrEmpty(oldPwd) ||
                string.IsNullOrEmpty(newPwd1) ||
                string.IsNullOrEmpty(newPwd2))
            {
                MessageBox.Show("请完整填写所有密码字段。");
                return;
            }

            // 新密码一致性
            if (newPwd1 != newPwd2)
            {
                MessageBox.Show("两次新密码输入不一致。");
                return;
            }

            // 旧密码校验（只弹一次）
            if (!VerifyPassword(studentID, oldPwd))
            {
                MessageBox.Show("旧密码错误，请重新输入。");
                return;
            }

            // 更新数据库
            using (var conn = new SqlConnection(connStr))
            using (var cmd = new SqlCommand("UPDATE UserAccount SET 密码 = @pwd WHERE 用户编号 = @uid", conn))
            {
                cmd.Parameters.AddWithValue("@pwd", newPwd1);
                cmd.Parameters.AddWithValue("@uid", studentID);
                conn.Open();
                if (cmd.ExecuteNonQuery() == 1)
                {
                    this.DialogResult = DialogResult.OK;
                    this.Close();
                    return;
                }
            }

            //MessageBox.Show("密码修改失败，请稍后重试。");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
