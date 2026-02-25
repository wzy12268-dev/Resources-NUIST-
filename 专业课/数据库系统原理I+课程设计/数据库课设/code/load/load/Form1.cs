using System;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Security.Cryptography;

namespace load
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void BtnOK_Click(object sender, EventArgs e)
        {
            string userID = TxtUser.Text.Trim();
            string password = TxtPass.Text.Trim();
            string passwordHash = password; // 不加密，直接比对明文


            string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";

            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                string sql = "SELECT 用户类型 FROM UserAccount WHERE 用户编号 = @id AND 密码 = @pwd";
                SqlCommand cmd = new SqlCommand(sql, conn);
                cmd.Parameters.AddWithValue("@id", userID);
                cmd.Parameters.AddWithValue("@pwd", passwordHash);

                object result = cmd.ExecuteScalar();

                if (result != null)
                {
                    string role = result.ToString();
                    MessageBox.Show("登录成功，用户类型：" +(role == "S" ? "学生" : role == "T" ? "教师" : "管理员"));
                    if (role == "S")
                    {
                        StudentForm sf = new StudentForm(userID, this);
                        sf.Show();
                        this.Hide(); // 隐藏当前窗口
                    }
                    else if (role == "A")
                    {
                        AdminForm af = new AdminForm(userID);
                        af.Show();
                        this.Hide();
                    }
                    else if (role == "T")
                    {
                        TeacherForm tf = new TeacherForm(userID);
                        tf.Show();
                        this.Hide();
                    }
                }
                else
                {
                    MessageBox.Show("用户名或密码错误！");
                }
            }
        }
        private void 用户名_Click(object sender, EventArgs e) { }

        private void label2_Click(object sender, EventArgs e) { }

        private void TxtUser_TextChanged(object sender, EventArgs e) { }

        private void TxtPass_TextChanged(object sender, EventArgs e) { }

        private void Form1_Load(object sender, EventArgs e) { }

        private void BtnExit_Click(object sender, EventArgs e)
        {
            this.Close(); 
        }
        public void ResetLoginFields()
        {
            TxtUser.Text = "";
            TxtPass.Text = "";
            TxtUser.Focus(); // 让用户名框获取焦点
        }

    }
}
