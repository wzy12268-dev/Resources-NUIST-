using System;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

namespace load
{
    public partial class TeacherForm : Form
    {
        private string userID;

        public TeacherForm(string id)
        {
            InitializeComponent();
            userID = id;
            this.Text = "教师系统";
            this.Load += TeacherForm_Load;

            // 手动绑定 DrawItem 事件（确保绑定成功）
            this.cmbStatType.DrawItem += new DrawItemEventHandler(cmbStatType_DrawItem);
        }

        // ✅ ComboBox 重绘逻辑，解决下拉项看不见的问题
        private void cmbStatType_DrawItem(object sender, DrawItemEventArgs e)
        {
            if (e.Index < 0) return;

            e.DrawBackground();

            string text = cmbStatType.Items[e.Index].ToString();
            using (SolidBrush brush = new SolidBrush(Color.Black))
            {
                e.Graphics.DrawString(text, e.Font ?? this.Font, brush, e.Bounds);
            }

            e.DrawFocusRectangle();
        }

        private void TeacherForm_Load(object sender, EventArgs e)
        {
            cmbStatType.Items.Clear();
            cmbStatType.Items.Add("各专业就业率");
            cmbStatType.Items.Add("各学院就业率");
            cmbStatType.Items.Add("用人单位就业人数");
            cmbStatType.Items.Add("指导学生就业率");

            cmbStatType.SelectedIndex = 0;

            // ✅ 强制设置样式，防止系统主题影响
            cmbStatType.DrawMode = DrawMode.OwnerDrawFixed;
            cmbStatType.DropDownStyle = ComboBoxStyle.DropDownList;
            cmbStatType.FlatStyle = FlatStyle.Standard;
            cmbStatType.Font = new Font("Microsoft YaHei", 10, FontStyle.Regular);
            cmbStatType.BackColor = Color.White;
            cmbStatType.ForeColor = Color.Black;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Hide();
            StudentManageForm smf = new StudentManageForm(userID, this);
            smf.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Form1 loginForm = new Form1();
            loginForm.Show();
            this.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Hide();
            TeacherInfoForm infoForm = new TeacherInfoForm(userID, this);
            infoForm.Show();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            this.Hide();
            EmployerViewForm form = new EmployerViewForm(this);
            form.Show();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            this.Hide();
            JobViewForm form = new JobViewForm(this);
            form.Show();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            // 信息统计与分析按钮保留扩展功能
        }

        private void cmbStatType_SelectedIndexChanged(object sender, EventArgs e)
        {
            // 可选：响应选择切换逻辑
        }

        private void btnStatEnter_Click(object sender, EventArgs e)
        {
            if (cmbStatType.SelectedItem == null)
            {
                MessageBox.Show("请选择一个统计类型！");
                return;
            }

            string selected = cmbStatType.SelectedItem.ToString();

            if (selected == "各学院就业率")
            {
                StatCollegeEmploymentRateForm form = new StatCollegeEmploymentRateForm(this);
                form.Show();
                this.Hide();
            }
            else if (selected == "各专业就业率")
            {
                StatMajorEmploymentRateForm form = new StatMajorEmploymentRateForm(this);
                form.Show();
                this.Hide();
            }
            else if (selected == "用人单位就业人数")
            {
                StatEmployerEmploymentCountForm form = new StatEmployerEmploymentCountForm(this);
                form.Show();
                this.Hide();
            }
            else if (selected == "指导学生就业率")
            {
                StatTeacherEmploymentRateForm form = new StatTeacherEmploymentRateForm(userID, this);
                form.Show();
                this.Hide();
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
