using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace load
{
    public partial class AdminForm : Form
    {
        private void label1_Click(object sender, EventArgs e)
        {

        }
        private string userID;

        public AdminForm(string id)
        {
            InitializeComponent();
            userID = id;
            this.Text = "管理员系统";
        }

        private void AdminForm_Load(object sender, EventArgs e)
        {
            Label label = new Label();
            label.Text = $"欢迎管理员 {userID} 登录系统";
            label.AutoSize = true;
            label.Location = new System.Drawing.Point(20, 20);
            this.Controls.Add(label);
        }
        // 学生信息管理
        private void button1_Click(object sender, EventArgs e)
        {
            AdminStudentManageForm f = new AdminStudentManageForm(this);
            f.Show();
            this.Hide();
        }
        // 教师信息管理
        private void button2_Click(object sender, EventArgs e)
        {
            AdminTeacherManageForm f = new AdminTeacherManageForm(this);
            f.Show();
            this.Hide();
        }
        //用人单位管理
        private void button3_Click(object sender, EventArgs e)
        {
            AdminEmployerManageForm form = new AdminEmployerManageForm(this); // this 就是 AdminForm
            form.Show();
            this.Hide();
        }
        // 招聘信息管理
        private void button4_Click(object sender, EventArgs e)
        {
            AdminJobPostingManageForm form = new AdminJobPostingManageForm(this); // this 就是 AdminForm
            form.Show();
            this.Hide();
        }
        // 学院信息管理
        private void button5_Click(object sender, EventArgs e)
        {
            AdminCollegeManageForm form = new AdminCollegeManageForm(this); // this 就是 AdminForm
            form.Show();
            this.Hide();
        }
        // 专业信息管理
        private void button6_Click(object sender, EventArgs e)
        {
            AdminMajorManageForm f = new AdminMajorManageForm(this);
            f.Show();
            this.Hide();
        }
        // 行业信息管理
        private void button7_Click(object sender, EventArgs e)
        {
            AdminIndustryManageForm form = new AdminIndustryManageForm(this); // this 就是 AdminForm
            form.Show();
            this.Hide();
        }
        // 退出
        private void button8_Click(object sender, EventArgs e)
        {
            Form1 loginForm = new Form1();
            loginForm.Show();
            this.Close();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            AdminUserAccountManageForm form = new AdminUserAccountManageForm(this); // this 就是 AdminForm
            form.Show();
            this.Hide();
        }
    }
}
