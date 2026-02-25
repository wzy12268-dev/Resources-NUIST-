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
    public partial class AdminIndustryManageForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;
        public AdminIndustryManageForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "行业信息管理";
        }
        private void AdminIndustryManageForm_Load(object sender, EventArgs e)
        {
            LoadIndustries();
        }

        private void LoadIndustries()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                string sql = "SELECT 行业编号, 行业名称 FROM Industry";
                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                dataGridView1.ReadOnly = false;
                dataGridView1.AllowUserToAddRows = false;
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
                        row.Cells["行业编号"].ReadOnly = true;
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
        // 删除
        private void button2_Click(object sender, EventArgs e)
        {
            if (dataGridView1.CurrentRow != null)
            {
                string iid = dataGridView1.CurrentRow.Cells["行业编号"].Value?.ToString();
                if (string.IsNullOrWhiteSpace(iid)) return;

                DialogResult dr = MessageBox.Show($"是否删除行业编号 {iid}？", "确认删除", MessageBoxButtons.YesNo);
                if (dr == DialogResult.Yes)
                {
                    using (SqlConnection conn = new SqlConnection(connStr))
                    {
                        conn.Open();

                        SqlCommand checkEmployer = new SqlCommand("SELECT COUNT(*) FROM Employer WHERE 行业编号 = @iid", conn);
                        checkEmployer.Parameters.AddWithValue("@iid", iid);
                        int count = (int)checkEmployer.ExecuteScalar();

                        if (count > 0)
                        {
                            MessageBox.Show($"行业编号 {iid} 已被用人单位使用，无法删除！");
                            return;
                        }

                        SqlCommand delete = new SqlCommand("DELETE FROM Industry WHERE 行业编号 = @iid", conn);
                        delete.Parameters.AddWithValue("@iid", iid);
                        delete.ExecuteNonQuery();

                        MessageBox.Show("删除成功！");
                        LoadIndustries();
                    }
                }
            }
        }
        // 保存 
        private void button3_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (row.IsNewRow) continue;

                    string iid = row.Cells["行业编号"].Value?.ToString()?.Trim();
                    string name = row.Cells["行业名称"].Value?.ToString()?.Trim();

                    if (string.IsNullOrWhiteSpace(iid)) continue;

                    SqlCommand checkCmd = new SqlCommand("SELECT COUNT(*) FROM Industry WHERE 行业编号 = @iid", conn);
                    checkCmd.Parameters.AddWithValue("@iid", iid);
                    int exists = (int)checkCmd.ExecuteScalar();

                    if (exists > 0)
                    {
                        string updateSql = "UPDATE Industry SET 行业名称=@name WHERE 行业编号=@iid";
                        SqlCommand cmd = new SqlCommand(updateSql, conn);
                        cmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@iid", iid);
                        cmd.ExecuteNonQuery();
                    }
                    else
                    {
                        string insertSql = "INSERT INTO Industry (行业编号, 行业名称) VALUES (@iid, @name)";
                        SqlCommand cmd = new SqlCommand(insertSql, conn);
                        cmd.Parameters.AddWithValue("@iid", iid);
                        cmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        cmd.ExecuteNonQuery();
                    }
                }

                MessageBox.Show("保存成功！");
                LoadIndustries();
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
