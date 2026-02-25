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
    public partial class StudentInfoForm : Form
    {
        private Form parentForm;
        private string studentID;
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";

        public StudentInfoForm(string sid, Form parent)
        {
            InitializeComponent();
            studentID = sid;
            parentForm = parent;
            LoadStudentInfo();
            this.Text = "个人信息管理";
        }

        private void LoadStudentInfo()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = @"
                SELECT 
                    G.学号,
                G.姓名,
                G.性别,
                G.出生日期,
                G.联系电话,
                P.专业名称,
                D.学院名称,
                T.姓名 AS 指导老师,
                G.单位编号,
                U.用户编号 AS 用户名,     
                U.密码
                    FROM Graduate G
                    JOIN Major P ON G.专业编号 = P.专业编号
                    JOIN College D ON P.学院编号 = D.学院编号
                    JOIN Teacher T ON G.工号 = T.工号
                    JOIN UserAccount U ON G.学号 = U.用户编号
                WHERE G.学号 = @sid";


                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                adapter.SelectCommand.Parameters.AddWithValue("@sid", studentID);

                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                // 设置只读列
                dataGridView1.Columns["学号"].ReadOnly = true;
                dataGridView1.Columns["专业名称"].ReadOnly = true;
                dataGridView1.Columns["学院名称"].ReadOnly = true;
                dataGridView1.Columns["指导老师"].ReadOnly = true;
                dataGridView1.Columns["用户名"].ReadOnly = true;

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

                // 解绑（防止重复绑定）
                dataGridView1.CellFormatting -= DataGridView1_CellFormatting;
                // 重新绑定
                dataGridView1.CellFormatting += DataGridView1_CellFormatting;
            }
        }
        // 格式化 “密码” 列：显示同等长度的 ‘*’
        private void DataGridView1_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            // 确保是 “密码” 这一列，且有值
            if (dataGridView1.Columns[e.ColumnIndex].Name == "密码" && e.Value != null)
            {
                var length = e.Value.ToString().Length;
                e.Value = new string('*', length);
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

                string name = row.Cells["姓名"].Value?.ToString();
                string gender = row.Cells["性别"].Value?.ToString();
                DateTime birth = Convert.ToDateTime(row.Cells["出生日期"].Value);
                string phone = row.Cells["联系电话"].Value?.ToString();
                string unit = row.Cells["单位编号"].Value?.ToString();
                string password = row.Cells["密码"].Value?.ToString();

                // 空单位编号处理为 NULL
                object unitValue = string.IsNullOrWhiteSpace(unit) ? DBNull.Value : (object)unit;

                // 更新 Graduate 表
                string sqlGraduate = @"
            UPDATE Graduate
            SET 姓名 = @name, 性别 = @gender, 出生日期 = @birth,
                联系电话 = @phone, 单位编号 = @unit
            WHERE 学号 = @sid";

                SqlCommand cmdGraduate = new SqlCommand(sqlGraduate, conn);
                cmdGraduate.Parameters.AddWithValue("@sid", studentID);
                cmdGraduate.Parameters.AddWithValue("@name", name);
                cmdGraduate.Parameters.AddWithValue("@gender", gender);
                cmdGraduate.Parameters.AddWithValue("@birth", birth);
                cmdGraduate.Parameters.AddWithValue("@phone", phone);
                cmdGraduate.Parameters.AddWithValue("@unit", unitValue);
                cmdGraduate.ExecuteNonQuery();

                // 更新 UserAccount 表中的密码
                string sqlAccount = @"UPDATE UserAccount SET 密码 = @pwd WHERE 用户编号 = @sid";
                SqlCommand cmdPwd = new SqlCommand(sqlAccount, conn);
                cmdPwd.Parameters.AddWithValue("@sid", studentID);
                cmdPwd.Parameters.AddWithValue("@pwd", password);
                cmdPwd.ExecuteNonQuery();

                MessageBox.Show("保存成功！");
                LoadStudentInfo(); // 重新加载更新后的数据
            }
        }

        // 退出
        private void button3_Click(object sender, EventArgs e)
        {
            parentForm.Show();
            this.Close();
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
        private void btnChangePwd_Click(object sender, EventArgs e)
        {
            using (var dlg = new ChangePasswordForm(studentID))
            {
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    MessageBox.Show("密码修改成功。");
                    LoadStudentInfo();
                }
                // 如果是 Cancel 就什么都不做，ChangePasswordForm 已经被 Dispose
            }
        }
    }
}