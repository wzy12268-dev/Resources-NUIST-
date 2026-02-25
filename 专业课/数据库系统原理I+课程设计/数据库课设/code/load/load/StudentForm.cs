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
    public partial class StudentForm : Form
    {
        private void label1_Click(object sender, EventArgs e)
        {

        }

        private Form loginForm;
        private string studentID;

        public StudentForm(string sid, Form login)
        {
            InitializeComponent();
            studentID = sid;
            loginForm = login;
            this.Text = "学生系统";
        }

        private void StudentForm_Load(object sender, EventArgs e)
        {
            Label label = new Label();
            label.Text = $"欢迎学生 {studentID} 登录系统";
            label.AutoSize = true;
            label.Location = new System.Drawing.Point(20, 20);
            this.Controls.Add(label);
        }

        // 个人信息管理
        private void button1_Click(object sender, EventArgs e)
        {
            this.Hide();
            StudentInfoForm form = new StudentInfoForm(studentID, this);
            form.Show();
        }
        // 用人单位查看
        private void button4_Click(object sender, EventArgs e)
        {
            this.Hide();
            EmployerViewForm evf = new EmployerViewForm(this);
            evf.Show();
        }

        // 招聘查询
        private void button2_Click(object sender, EventArgs e)
        {
            JobQueryForm jobQuery = new JobQueryForm(studentID, this);
            jobQuery.Show();
            this.Hide();
        }

        // 退出登录
        private void button3_Click(object sender, EventArgs e)
        {
            // 强制转换为 Form1，调用重置方法
            if (loginForm is Form1 form1)
            {
                form1.ResetLoginFields();
                form1.Show();
            }
            this.Close();
        }

        
    }
}
