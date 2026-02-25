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
    public partial class JobQueryForm : Form
    {
        private string studentID;
        private Form parentForm;
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";

        public JobQueryForm(string sid, Form parent)
        {
            InitializeComponent();
            studentID = sid;
            parentForm = parent;
            this.Text = "岗位招聘查询";
        }
        private void JobQueryForm_Load(object sender, EventArgs e)
        {
            // 页面加载时自动查询并显示所有岗位信息
            button1.PerformClick(); // 触发一次“查询”按钮点击

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
        }

        // 获取学生专业
        private string GetStudentMajor(string sid, SqlConnection conn)
        {
            string sql = "SELECT 专业编号 FROM Graduate WHERE 学号 = @id";
            SqlCommand cmd = new SqlCommand(sql, conn);
            cmd.Parameters.AddWithValue("@id", sid);
            return cmd.ExecuteScalar()?.ToString() ?? "";
        }


        // 是否专业版本
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }
        // 招聘人数条件
        private void label1_Click(object sender, EventArgs e)
        {

        }
        // 地理位置条件
        private void label2_Click(object sender, EventArgs e)
        {

        }
        // 截至日期条件
        private void label3_Click(object sender, EventArgs e)
        {

        }
        // 查询按钮
        private void button1_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();

                // 基础查询（岗位 + 用人单位 + 行业）
                string sql = @"
            SELECT 
                J.单位编号,E.单位名称,E.单位地址,E.联系电话,J.发布时间,J.截止时间,J.招聘人数,M.专业名称,I.行业名称
            FROM JobPosting J
            JOIN Employer E ON J.单位编号 = E.单位编号
            JOIN Industry I ON E.行业编号 = I.行业编号
            JOIN Major M ON M.专业编号 = J.专业编号
            WHERE 1=1";

                List<SqlParameter> parameters = new List<SqlParameter>();

                // 专业匹配
                if (checkBox1.Checked)
                {
                    string major = GetStudentMajor(studentID, conn);
                    sql += " AND J.专业编号 = @major";
                    parameters.Add(new SqlParameter("@major", major));
                }

                // 招聘人数 ≥
                if (int.TryParse(textBox1.Text.Trim(), out int minCount))
                {
                    sql += " AND J.招聘人数 >= @minCount";
                    parameters.Add(new SqlParameter("@minCount", minCount));
                }

                // 地理位置 LIKE
                if (!string.IsNullOrWhiteSpace(textBox2.Text))
                {
                    sql += " AND E.单位地址 LIKE @address";
                    parameters.Add(new SqlParameter("@address", "%" + textBox2.Text.Trim() + "%"));
                }

                // 截止时间 ≥
                if (!string.IsNullOrWhiteSpace(textBox3.Text))
                {
                    if (DateTime.TryParse(textBox3.Text.Trim(), out DateTime deadline))
                    {
                        sql += " AND J.截止时间 >= @deadline";
                        parameters.Add(new SqlParameter("@deadline", deadline));
                    }
                    else
                    {
                        MessageBox.Show("请输入正确的截止时间格式（如 2025-06-01）", "日期格式错误");
                        return;
                    }
                }

                // 行业名称 LIKE
                if (!string.IsNullOrWhiteSpace(textBox4.Text))
                {
                    sql += " AND I.行业名称 LIKE @industry";
                    parameters.Add(new SqlParameter("@industry", "%" + textBox4.Text.Trim() + "%"));
                }

                // 执行查询
                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                foreach (var p in parameters)
                {
                    adapter.SelectCommand.Parameters.Add(p);
                }

                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                // 无结果时返回
                if (dt.Rows.Count == 0)
                {
                    MessageBox.Show("暂无匹配的招聘信息记录。", "查询结果", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                // 去除索引
                dataGridView1.RowHeadersVisible = false;

                // 样式设置
                dataGridView1.ReadOnly = true;
                dataGridView1.AllowUserToAddRows = false;
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
                dataGridView1.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView1.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            }
        }

        // 退出按钮
        private void button2_Click(object sender, EventArgs e)
        {
            parentForm.Show();
            this.Close();
        }
        // 行业名称
        private void label4_Click(object sender, EventArgs e)
        {

        }
        // 招聘人数text
        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
        // 地理位置text
        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }
        // 截止时间text
        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }
        // 行业名称text
        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
    }
}
