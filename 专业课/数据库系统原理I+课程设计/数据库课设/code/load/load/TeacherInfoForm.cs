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
    public partial class TeacherInfoForm : Form
    {
        private string teacherID; // 当前教师工号
        private Form parentForm;  // 上级窗体
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";

        public TeacherInfoForm(string id, Form parent)
        {
            InitializeComponent();
            teacherID = id;
            parentForm = parent;
            LoadTeacherInfo();
            this.Text = "个人信息管理";
        }

        // 加载教师信息
        private void LoadTeacherInfo()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = @"
        SELECT 
            T.工号,
            T.姓名,
            T.性别,
            T.出生日期,
            T.联系电话,
            U.用户编号 AS 用户名,
            U.密码
        FROM Teacher T
        JOIN UserAccount U ON T.工号 = U.用户编号
        WHERE T.工号 = @id";

                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                adapter.SelectCommand.Parameters.AddWithValue("@id", teacherID); // 由外部传入

                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                // 绑定完数据之后
                dataGridView1.AllowUserToAddRows = false;

                // 去除索引
                dataGridView1.RowHeadersVisible = false;
                // 内容居中
                dataGridView1.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                // 列标题居中
                dataGridView1.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                // 自适应列宽
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
                // 填满
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;

                // 设置“工号”和“用户名”列为只读
                if (dataGridView1.Columns.Contains("工号"))
                    dataGridView1.Columns["工号"].ReadOnly = true;
                if (dataGridView1.Columns.Contains("用户名"))
                    dataGridView1.Columns["用户名"].ReadOnly = true;
                if (dataGridView1.Columns.Contains("密码"))
                    dataGridView1.Columns["密码"].ReadOnly = true;

                // **绑定密码列的格式化事件**：每次重载都先解绑再绑，避免重复
                dataGridView1.CellFormatting -= DataGridView1_CellFormatting;
                dataGridView1.CellFormatting += DataGridView1_CellFormatting;
            }
        }
        private void DataGridView1_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            // 只格式化“密码”这一列，且必须有值
            if (dataGridView1.Columns[e.ColumnIndex].Name == "密码" && e.Value != null)
            {
                // 把原始密码长度映射成相同长度的星号
                int len = e.Value.ToString().Length;
                e.Value = new string('*', len);
                e.FormattingApplied = true;
            }
        }

        // 保存
        private void button1_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();

                DataGridViewRow row = dataGridView1.Rows[0];

                string id = row.Cells["工号"].Value.ToString();
                string username = row.Cells["用户名"].Value?.ToString();
                string name = row.Cells["姓名"].Value?.ToString();
                string gender = row.Cells["性别"].Value?.ToString();
                string birth = row.Cells["出生日期"].Value?.ToString();
                string phone = row.Cells["联系电话"].Value?.ToString();
                string pwd = row.Cells["密码"].Value?.ToString();

                try
                {
                    // 更新 Teacher 表
                    string sql1 = "UPDATE Teacher SET 姓名=@name, 性别=@gender, 出生日期=@birth, 联系电话=@phone WHERE 工号=@id";
                    SqlCommand cmd1 = new SqlCommand(sql1, conn);
                    cmd1.Parameters.AddWithValue("@name", name);
                    cmd1.Parameters.AddWithValue("@gender", gender);
                    cmd1.Parameters.AddWithValue("@birth", birth);
                    cmd1.Parameters.AddWithValue("@phone", phone);
                    cmd1.Parameters.AddWithValue("@id", id);
                    cmd1.ExecuteNonQuery();

                    // 更新 UserAccount 表中的密码
                    string sql2 = "UPDATE UserAccount SET 密码=@pwd WHERE 用户编号=@id";
                    SqlCommand cmd2 = new SqlCommand(sql2, conn);
                    cmd2.Parameters.AddWithValue("@pwd", pwd);
                    cmd2.Parameters.AddWithValue("@id", id);
                    cmd2.ExecuteNonQuery();

                    MessageBox.Show("保存成功！");
                    LoadTeacherInfo();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("保存失败：" + ex.Message);
                }
            }
        }

        // 退出
        private void button2_Click(object sender, EventArgs e)
        {
            parentForm.Show(); // 回到教师主界面
            this.Close();
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void TeacherInfoForm_Load(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            using (var dlg = new ChangePasswordForm(teacherID))
            {
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    MessageBox.Show("密码修改成功。");
                    LoadTeacherInfo();
                }
            
            }
        }
    }
}
