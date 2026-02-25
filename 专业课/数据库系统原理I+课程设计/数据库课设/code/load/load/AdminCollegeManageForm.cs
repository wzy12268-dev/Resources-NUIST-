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
    public partial class AdminCollegeManageForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;
        public AdminCollegeManageForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "学院信息管理";
        }
        private void AdminCollegeManageForm_Load(object sender, EventArgs e)
        {
            LoadColleges();
        }

        private void LoadColleges()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                string sql = "SELECT 学院编号, 学院名称 FROM College";
                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                dataGridView1.ReadOnly = false;
                dataGridView1.AllowUserToAddRows = false;
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
                dataGridView1.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView1.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
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
                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (!row.IsNewRow)
                        row.Cells["学院编号"].ReadOnly = true;
                }
            }
        }
        // 添加
        private void button1_Click(object sender, EventArgs e)
        {
            DataTable dt = (DataTable)dataGridView1.DataSource;
            DataRow newRow = dt.NewRow();
            foreach (DataColumn col in dt.Columns)
            {
                newRow[col.ColumnName] = col.DataType == typeof(DateTime) ? (object)DBNull.Value : (object)"";
            }
            dt.Rows.Add(newRow);
        }
        // 保存
        private void button2_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (row.IsNewRow) continue;

                    string cid = row.Cells["学院编号"].Value?.ToString()?.Trim();
                    string name = row.Cells["学院名称"].Value?.ToString()?.Trim();

                    if (string.IsNullOrWhiteSpace(cid)) continue;

                    SqlCommand checkCmd = new SqlCommand("SELECT COUNT(*) FROM College WHERE 学院编号 = @cid", conn);
                    checkCmd.Parameters.AddWithValue("@cid", cid);
                    int exists = (int)checkCmd.ExecuteScalar();

                    if (exists > 0)
                    {
                        // 更新
                        string updateSql = "UPDATE College SET 学院名称=@name WHERE 学院编号=@cid";
                        SqlCommand cmd = new SqlCommand(updateSql, conn);
                        cmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@cid", cid);
                        cmd.ExecuteNonQuery();
                    }
                    else
                    {
                        // 插入
                        string insertSql = "INSERT INTO College (学院编号, 学院名称) VALUES (@cid, @name)";
                        SqlCommand cmd = new SqlCommand(insertSql, conn);
                        cmd.Parameters.AddWithValue("@cid", cid);
                        cmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        cmd.ExecuteNonQuery();
                    }
                }

                MessageBox.Show("保存成功！");
                LoadColleges();
            }
        }
        // 删除
        private void button3_Click(object sender, EventArgs e)
        {
            if (dataGridView1.CurrentRow != null)
            {
                string cid = dataGridView1.CurrentRow.Cells["学院编号"].Value?.ToString();
                if (string.IsNullOrWhiteSpace(cid)) return;

                DialogResult dr = MessageBox.Show($"是否删除学院编号 {cid}？", "确认删除", MessageBoxButtons.YesNo);
                if (dr == DialogResult.Yes)
                {
                    using (SqlConnection conn = new SqlConnection(connStr))
                    {
                        conn.Open();

                        // 检查是否被 Major 表引用
                        SqlCommand checkMajor = new SqlCommand("SELECT COUNT(*) FROM Major WHERE 学院编号 = @cid", conn);
                        checkMajor.Parameters.AddWithValue("@cid", cid);
                        int majorCount = (int)checkMajor.ExecuteScalar();

                        // 检查是否被 Teacher 表引用
                        SqlCommand checkTeacher = new SqlCommand("SELECT COUNT(*) FROM Teacher WHERE 学院编号 = @cid", conn);
                        checkTeacher.Parameters.AddWithValue("@cid", cid);
                        int teacherCount = (int)checkTeacher.ExecuteScalar();

                        if (majorCount > 0)
                        {
                            MessageBox.Show($"该学院编号 {cid} 已被专业表引用，无法删除！");
                            return;
                        }

                        if (teacherCount > 0)
                        {
                            MessageBox.Show($"该学院编号 {cid} 已被教师表引用，无法删除！");
                            return;
                        }

                        // 若无引用，执行删除
                        SqlCommand delete = new SqlCommand("DELETE FROM College WHERE 学院编号 = @cid", conn);
                        delete.Parameters.AddWithValue("@cid", cid);
                        delete.ExecuteNonQuery();

                        MessageBox.Show("删除成功！");
                        LoadColleges();
                    }
                }
            }
        }

        // 退出
        private void button4_Click(object sender, EventArgs e)
        {
            parentForm?.Show();
            this.Close();
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
    }
}
