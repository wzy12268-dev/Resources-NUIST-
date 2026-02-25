using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace load
{
    public partial class AdminMajorManageForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;

        public AdminMajorManageForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "专业信息管理";
        }

        private void AdminMajorManageForm_Load(object sender, EventArgs e)
        {
            LoadMajors();
        }

        private void LoadMajors()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                string sql = "SELECT 专业编号, 专业名称, 学院编号 FROM Major";
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
                        row.Cells["专业编号"].ReadOnly = true;
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

                    string mid = row.Cells["专业编号"].Value?.ToString()?.Trim();
                    string name = row.Cells["专业名称"].Value?.ToString()?.Trim();
                    string cid = row.Cells["学院编号"].Value?.ToString()?.Trim();

                    if (string.IsNullOrWhiteSpace(mid)) continue;

                    // ✅ 校验学院编号是否存在（如果不为空）
                    if (!string.IsNullOrWhiteSpace(cid))
                    {
                        SqlCommand checkCollege = new SqlCommand("SELECT COUNT(*) FROM College WHERE 学院编号 = @cid", conn);
                        checkCollege.Parameters.AddWithValue("@cid", cid);
                        int valid = (int)checkCollege.ExecuteScalar();

                        if (valid == 0)
                        {
                            MessageBox.Show($"学院编号 {cid} 不存在，已置空！");
                            cid = null; // ✅ 设为空，仍然保存
                        }
                    }

                    // 是否存在该专业编号
                    SqlCommand checkCmd = new SqlCommand("SELECT COUNT(*) FROM Major WHERE 专业编号 = @mid", conn);
                    checkCmd.Parameters.AddWithValue("@mid", mid);
                    int exists = (int)checkCmd.ExecuteScalar();

                    if (exists > 0)
                    {
                        string updateSql = "UPDATE Major SET 专业名称=@name, 学院编号=@cid WHERE 专业编号=@mid";
                        SqlCommand cmd = new SqlCommand(updateSql, conn);
                        cmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@cid", string.IsNullOrWhiteSpace(cid) ? DBNull.Value : (object)cid);
                        cmd.Parameters.AddWithValue("@mid", mid);
                        cmd.ExecuteNonQuery();
                    }
                    else
                    {
                        string insertSql = "INSERT INTO Major (专业编号, 专业名称, 学院编号) VALUES (@mid, @name, @cid)";
                        SqlCommand cmd = new SqlCommand(insertSql, conn);
                        cmd.Parameters.AddWithValue("@mid", mid);
                        cmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@cid", string.IsNullOrWhiteSpace(cid) ? DBNull.Value : (object)cid);
                        cmd.ExecuteNonQuery();
                    }
                }

                MessageBox.Show("保存成功！");
                LoadMajors();
            }
        }


        // 删除
        private void button3_Click(object sender, EventArgs e)
        {
            if (dataGridView1.CurrentRow != null)
            {
                string mid = dataGridView1.CurrentRow.Cells["专业编号"].Value?.ToString();
                DialogResult dr = MessageBox.Show($"是否删除专业编号 {mid}？", "确认删除", MessageBoxButtons.YesNo);
                if (dr == DialogResult.Yes)
                {
                    using (SqlConnection conn = new SqlConnection(connStr))
                    {
                        conn.Open();

                        SqlCommand check = new SqlCommand("SELECT COUNT(*) FROM Graduate WHERE 专业编号 = @mid", conn);
                        check.Parameters.AddWithValue("@mid", mid);
                        int count = (int)check.ExecuteScalar();

                        if (count > 0)
                        {
                            MessageBox.Show($"专业编号 {mid} 已被学生使用，无法删除！");
                            return;
                        }

                        SqlCommand delete = new SqlCommand("DELETE FROM Major WHERE 专业编号 = @mid", conn);
                        delete.Parameters.AddWithValue("@mid", mid);
                        delete.ExecuteNonQuery();

                        MessageBox.Show("删除成功！");
                        LoadMajors();
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
    }
}
