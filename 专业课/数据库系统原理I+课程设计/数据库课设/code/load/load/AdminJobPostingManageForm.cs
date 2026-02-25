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
    public partial class AdminJobPostingManageForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;

        public AdminJobPostingManageForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "招聘信息管理";
        }

        private void AdminJobPostingManageForm_Load(object sender, EventArgs e)
        {
            LoadJobPostings(); // 加载全部岗位信息
        }
        private void LoadJobPostings()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = "SELECT * FROM JobPosting ORDER BY 发布时间 DESC"; // 🔽 最近的在最上
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
            }
        }


        // 查询
        private void button1_Click(object sender, EventArgs e)
        {
            string unitId = textBox1.Text.Trim();
            string majorId = textBox2.Text.Trim();

            string sql = "SELECT * FROM JobPosting WHERE 1=1";
            if (!string.IsNullOrEmpty(unitId))
                sql += " AND 单位编号 = @unitId";
            if (!string.IsNullOrEmpty(majorId))
                sql += " AND 专业编号 = @majorId";

            using (SqlConnection conn = new SqlConnection(connStr))
            {
                SqlCommand cmd = new SqlCommand(sql, conn);
                if (!string.IsNullOrEmpty(unitId))
                    cmd.Parameters.AddWithValue("@unitId", unitId);
                if (!string.IsNullOrEmpty(majorId))
                    cmd.Parameters.AddWithValue("@majorId", majorId);

                SqlDataAdapter da = new SqlDataAdapter(cmd);
                DataTable dt = new DataTable();
                da.Fill(dt);
                dataGridView1.DataSource = dt;
            }
        }
        // 添加
        private void button2_Click(object sender, EventArgs e)
        {
            DataTable dt = (DataTable)dataGridView1.DataSource;
            DataRow newRow = dt.NewRow();
            dt.Rows.Add(newRow);
        }
        // 删除
        private void button3_Click(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count > 0)
            {
                DataGridViewRow selectedRow = dataGridView1.SelectedRows[0];

                string unitId = selectedRow.Cells["单位编号"].Value?.ToString()?.Trim();
                string majorId = selectedRow.Cells["专业编号"].Value?.ToString()?.Trim();
                string releaseDateStr = selectedRow.Cells["发布时间"].Value?.ToString()?.Trim();
                string deadlineStr = selectedRow.Cells["截止时间"].Value?.ToString()?.Trim();

                if (string.IsNullOrWhiteSpace(unitId) || string.IsNullOrWhiteSpace(majorId) ||
                    string.IsNullOrWhiteSpace(releaseDateStr) || string.IsNullOrWhiteSpace(deadlineStr))
                {
                    MessageBox.Show("无法确定主键信息，无法删除！");
                    return;
                }

                if (!DateTime.TryParse(releaseDateStr, out DateTime releaseDate) ||
                    !DateTime.TryParse(deadlineStr, out DateTime deadline))
                {
                    MessageBox.Show("日期格式错误，无法删除！");
                    return;
                }

                DialogResult dr = MessageBox.Show("确认删除该岗位信息？", "确认", MessageBoxButtons.YesNo);
                if (dr == DialogResult.Yes)
                {
                    using (SqlConnection conn = new SqlConnection(connStr))
                    {
                        conn.Open();
                        string deleteSql = @"DELETE FROM JobPosting 
                                     WHERE 单位编号 = @uid AND 专业编号 = @mid 
                                       AND 发布时间 = @release AND 截止时间 = @deadline";

                        SqlCommand cmd = new SqlCommand(deleteSql, conn);
                        cmd.Parameters.AddWithValue("@uid", unitId);
                        cmd.Parameters.AddWithValue("@mid", majorId);
                        cmd.Parameters.AddWithValue("@release", releaseDate);
                        cmd.Parameters.AddWithValue("@deadline", deadline);
                        int affected = cmd.ExecuteNonQuery();

                        if (affected > 0)
                            MessageBox.Show("删除成功！");
                        else
                            MessageBox.Show("删除失败，记录可能不存在！");
                    }

                    // 最后从 DataGridView 中移除行（可选）
                    dataGridView1.Rows.Remove(selectedRow);

                    // 推荐：刷新表格数据更稳妥
                    LoadJobPostings();
                }
            }
        }

        // 保存
        private void button4_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                bool hasSaved = false;

                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (row.IsNewRow) continue;

                    string unitId = row.Cells["单位编号"].Value?.ToString()?.Trim();
                    string majorId = row.Cells["专业编号"].Value?.ToString()?.Trim();
                    string releaseDateStr = row.Cells["发布时间"].Value?.ToString()?.Trim();
                    string deadlineStr = row.Cells["截止时间"].Value?.ToString()?.Trim();
                    string peopleNeededStr = row.Cells["招聘人数"].Value?.ToString()?.Trim();

                    // 校验：非空字段
                    if (string.IsNullOrWhiteSpace(unitId) || string.IsNullOrWhiteSpace(majorId) ||
                        string.IsNullOrWhiteSpace(releaseDateStr) || string.IsNullOrWhiteSpace(deadlineStr))
                    {
                        MessageBox.Show("单位编号、专业编号、发布时间、截止时间不能为空！");
                        continue;
                    }

                    // 校验单位编号是否存在
                    SqlCommand checkUnit = new SqlCommand("SELECT COUNT(*) FROM Employer WHERE 单位编号 = @uid", conn);
                    checkUnit.Parameters.AddWithValue("@uid", unitId);
                    if ((int)checkUnit.ExecuteScalar() == 0)
                    {
                        MessageBox.Show($"单位编号 {unitId} 不存在！");
                        continue;
                    }

                    // 校验专业编号是否存在
                    SqlCommand checkMajor = new SqlCommand("SELECT COUNT(*) FROM Major WHERE 专业编号 = @mid", conn);
                    checkMajor.Parameters.AddWithValue("@mid", majorId);
                    if ((int)checkMajor.ExecuteScalar() == 0)
                    {
                        MessageBox.Show($"专业编号 {majorId} 不存在！");
                        continue;
                    }

                    // 校验时间格式
                    if (!DateTime.TryParse(releaseDateStr, out DateTime releaseDate) ||
                        !DateTime.TryParse(deadlineStr, out DateTime deadline))
                    {
                        MessageBox.Show("请确保发布时间和截止时间为合法日期格式！");
                        continue;
                    }

                    if (releaseDate > deadline)
                    {
                        MessageBox.Show($"发布时间 {releaseDate:yyyy-MM-dd} 晚于截止时间 {deadline:yyyy-MM-dd}！");
                        continue;
                    }

                    // 校验招聘人数
                    if (!int.TryParse(peopleNeededStr, out int peopleNeeded) || peopleNeeded < 0)
                    {
                        MessageBox.Show("招聘人数必须是非负整数！");
                        continue;
                    }

                    // ✅ DELETE（防止主键冲突）
                    string deleteSql = @"DELETE FROM JobPosting 
                                 WHERE 单位编号 = @uid AND 专业编号 = @mid 
                                   AND 发布时间 = @release AND 截止时间 = @deadline";
                    SqlCommand deleteCmd = new SqlCommand(deleteSql, conn);
                    deleteCmd.Parameters.AddWithValue("@uid", unitId);
                    deleteCmd.Parameters.AddWithValue("@mid", majorId);
                    deleteCmd.Parameters.AddWithValue("@release", releaseDate);
                    deleteCmd.Parameters.AddWithValue("@deadline", deadline);
                    deleteCmd.ExecuteNonQuery();

                    // ✅ INSERT（插入新数据）
                    string insertSql = @"INSERT INTO JobPosting 
                                (单位编号, 专业编号, 发布时间, 截止时间, 招聘人数)
                                VALUES (@uid, @mid, @release, @deadline, @count)";
                    SqlCommand insertCmd = new SqlCommand(insertSql, conn);
                    insertCmd.Parameters.AddWithValue("@uid", unitId);
                    insertCmd.Parameters.AddWithValue("@mid", majorId);
                    insertCmd.Parameters.AddWithValue("@release", releaseDate);
                    insertCmd.Parameters.AddWithValue("@deadline", deadline);
                    insertCmd.Parameters.AddWithValue("@count", peopleNeeded);
                    insertCmd.ExecuteNonQuery();

                    hasSaved = true;
                }

                if (hasSaved)
                {
                    MessageBox.Show("保存成功！");
                    LoadJobPostings(); // 刷新表格
                }
            }
        }



        // 退出
        private void button5_Click(object sender, EventArgs e)
        {
            parentForm?.Show();
            this.Close();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
