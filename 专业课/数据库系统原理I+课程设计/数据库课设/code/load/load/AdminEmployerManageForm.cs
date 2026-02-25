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
    public partial class AdminEmployerManageForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;

        public AdminEmployerManageForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "用人单位管理";
        }

        private void AdminEmployerManageForm_Load(object sender, EventArgs e)
        {
            LoadEmployers();
        }

        private void LoadEmployers(string eid = "", string name = "", string address = "")
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                string sql = @"SELECT 单位编号, 单位名称, 单位地址, 联系电话, 行业编号 
                               FROM Employer
                               WHERE 单位编号 LIKE @eid AND 单位名称 LIKE @name AND 单位地址 LIKE @addr";

                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                adapter.SelectCommand.Parameters.AddWithValue("@eid", "%" + eid + "%");
                adapter.SelectCommand.Parameters.AddWithValue("@name", "%" + name + "%");
                adapter.SelectCommand.Parameters.AddWithValue("@addr", "%" + address + "%");

                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                dataGridView1.ReadOnly = false;
                dataGridView1.AllowUserToAddRows = false;
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
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
                        row.Cells["单位编号"].ReadOnly = true;
                }
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
        // 查询
        private void button1_Click(object sender, EventArgs e)
        {
            string eid = textBox1.Text.Trim();  // 单位编号
            string name = textBox2.Text.Trim(); // 单位名称
            string addr = textBox3.Text.Trim(); // 单位地址
            LoadEmployers(eid, name, addr);
        }
        // 删除
        private void button2_Click(object sender, EventArgs e)
        {
            if (dataGridView1.CurrentRow != null)
            {
                string eid = dataGridView1.CurrentRow.Cells["单位编号"].Value?.ToString();
                if (string.IsNullOrWhiteSpace(eid)) return;

                DialogResult dr = MessageBox.Show($"是否删除单位编号 {eid}？", "确认删除", MessageBoxButtons.YesNo);
                if (dr == DialogResult.Yes)
                {
                    using (SqlConnection conn = new SqlConnection(connStr))
                    {
                        conn.Open();

                        SqlCommand checkGraduate = new SqlCommand("SELECT COUNT(*) FROM Graduate WHERE 单位编号 = @eid", conn);
                        checkGraduate.Parameters.AddWithValue("@eid", eid);
                        int count = (int)checkGraduate.ExecuteScalar();

                        if (count > 0)
                        {
                            MessageBox.Show($"单位编号 {eid} 已被毕业生使用，无法删除！");
                            return;
                        }

                        SqlCommand delete = new SqlCommand("DELETE FROM Employer WHERE 单位编号 = @eid", conn);
                        delete.Parameters.AddWithValue("@eid", eid);
                        delete.ExecuteNonQuery();

                        MessageBox.Show("删除成功！");
                        LoadEmployers();
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
                bool hasSaved = false; // ✅ 用于追踪是否真的保存了数据

                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (row.IsNewRow) continue;

                    string eid = row.Cells["单位编号"].Value?.ToString()?.Trim();
                    string name = row.Cells["单位名称"].Value?.ToString()?.Trim();
                    string addr = row.Cells["单位地址"].Value?.ToString()?.Trim();
                    string phone = row.Cells["联系电话"].Value?.ToString()?.Trim();
                    string industryId = row.Cells["行业编号"].Value?.ToString()?.Trim();

                    if (string.IsNullOrWhiteSpace(eid)) continue;

                    // ✅ 行业编号校验
                    if (!string.IsNullOrWhiteSpace(industryId))
                    {
                        SqlCommand checkIndustry = new SqlCommand("SELECT COUNT(*) FROM Industry WHERE 行业编号 = @iid", conn);
                        checkIndustry.Parameters.AddWithValue("@iid", industryId);
                        int valid = (int)checkIndustry.ExecuteScalar();

                        if (valid == 0)
                        {
                            MessageBox.Show($"行业编号 {industryId} 不存在，已置空！");
                            industryId = null; // ✅ 清空行业编号字段
                        }
                    }

                    // 是否已存在
                    SqlCommand check = new SqlCommand("SELECT COUNT(*) FROM Employer WHERE 单位编号 = @eid", conn);
                    check.Parameters.AddWithValue("@eid", eid);
                    int exists = (int)check.ExecuteScalar();

                    if (exists > 0)
                    {
                        // 更新
                        string updateSql = @"UPDATE Employer 
                                     SET 单位名称=@name, 单位地址=@addr, 联系电话=@phone, 行业编号=@iid 
                                     WHERE 单位编号=@eid";
                        SqlCommand cmd = new SqlCommand(updateSql, conn);
                        cmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@addr", addr ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@phone", phone ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@iid", string.IsNullOrWhiteSpace(industryId) ? DBNull.Value : (object)industryId);
                        cmd.Parameters.AddWithValue("@eid", eid);
                        cmd.ExecuteNonQuery();
                        hasSaved = true;
                    }
                    else
                    {
                        // 插入
                        string insertSql = @"INSERT INTO Employer (单位编号, 单位名称, 单位地址, 联系电话, 行业编号) 
                                     VALUES (@eid, @name, @addr, @phone, @iid)";
                        SqlCommand cmd = new SqlCommand(insertSql, conn);
                        cmd.Parameters.AddWithValue("@eid", eid);
                        cmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@addr", addr ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@phone", phone ?? (object)DBNull.Value);
                        cmd.Parameters.AddWithValue("@iid", string.IsNullOrWhiteSpace(industryId) ? DBNull.Value : (object)industryId);
                        cmd.ExecuteNonQuery();
                        hasSaved = true;
                    }
                }

                if (hasSaved)
                {
                    MessageBox.Show("保存成功！");
                    LoadEmployers(); // 刷新
                }
            }
        }


        // 退出
        private void button4_Click(object sender, EventArgs e)
        {
            parentForm?.Show();
            this.Close();
        }
        // 添加
        private void button5_Click(object sender, EventArgs e)
        {
            DataTable dt = (DataTable)dataGridView1.DataSource;
            DataRow newRow = dt.NewRow();

            foreach (DataColumn col in dt.Columns)
            {
                newRow[col.ColumnName] = col.DataType == typeof(DateTime) ? (object)DBNull.Value : (object)"";
            }

            dt.Rows.Add(newRow);
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
    }
}
