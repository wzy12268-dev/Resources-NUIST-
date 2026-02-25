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
    public partial class AdminTeacherManageForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;
        public AdminTeacherManageForm(Form parent)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "教师信息管理";
        }
        private void AdminTeacherManageForm_Load(object sender, EventArgs e)
        {
            LoadTeachers();
        }
        private void LoadTeachers(string tid = "", string name = "", string collegeName = "")
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();

                // 查询学院编号（如果输入了学院名称）
                string collegeId = null;
                if (!string.IsNullOrWhiteSpace(collegeName))
                {
                    SqlCommand cmd = new SqlCommand("SELECT TOP 1 学院编号 FROM College WHERE 学院名称 LIKE @college", conn);
                    cmd.Parameters.AddWithValue("@college", "%" + collegeName + "%");
                    object result = cmd.ExecuteScalar();
                    if (result != null)
                    {
                        collegeId = result.ToString();
                    }
                    else
                    {
                        // ❗查询不到学院编号时，直接返回空结果
                        dataGridView1.DataSource = new DataTable();
                        return;
                    }
                }

                // 构造 SQL 查询语句
                string sql = @"SELECT 工号, 姓名, 性别, 出生日期, 联系电话, 学院编号 
                       FROM Teacher 
                       WHERE 工号 LIKE @tid AND 姓名 LIKE @name";

                if (!string.IsNullOrWhiteSpace(collegeId))
                {
                    sql += " AND 学院编号 LIKE @collegeId";
                }

                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                adapter.SelectCommand.Parameters.AddWithValue("@tid", "%" + tid + "%");
                adapter.SelectCommand.Parameters.AddWithValue("@name", "%" + name + "%");

                if (!string.IsNullOrWhiteSpace(collegeId))
                {
                    adapter.SelectCommand.Parameters.AddWithValue("@collegeId", "%" + collegeId + "%");
                }

                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                // 表格美化设置
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
                // 锁定工号列
                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (!row.IsNewRow)
                        row.Cells["工号"].ReadOnly = true;
                }
            }
        }



        // 查询
        private void button1_Click(object sender, EventArgs e)
        {
            string tid = textBox1.Text.Trim();
            string name = textBox2.Text.Trim();
            string college = textBox3.Text.Trim(); // 学院名称
            LoadTeachers(tid, name, college);
        }
        // 添加
        private void button2_Click(object sender, EventArgs e)
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
        private void button3_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();

                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (row.IsNewRow) continue;

                    string tid = row.Cells["工号"].Value?.ToString()?.Trim();
                    if (string.IsNullOrWhiteSpace(tid)) continue;

                    string name = row.Cells["姓名"].Value?.ToString()?.Trim();
                    string gender = row.Cells["性别"].Value?.ToString()?.Trim();
                    string phone = row.Cells["联系电话"].Value?.ToString()?.Trim();
                    string collegeId = row.Cells["学院编号"].Value?.ToString()?.Trim();

                    DateTime? birth = row.Cells["出生日期"].Value == DBNull.Value || row.Cells["出生日期"].Value == null
                        ? (DateTime?)null
                        : Convert.ToDateTime(row.Cells["出生日期"].Value);

                    // 学院编号检查（可空，但如填写需存在）
                    if (!string.IsNullOrWhiteSpace(collegeId))
                    {
                        SqlCommand checkCollege = new SqlCommand("SELECT COUNT(*) FROM College WHERE 学院编号 = @cid", conn);
                        checkCollege.Parameters.AddWithValue("@cid", collegeId);
                        if ((int)checkCollege.ExecuteScalar() == 0)
                        {
                            MessageBox.Show($"学院编号 {collegeId} 不存在，已置空！");
                            collegeId = null;
                        }
                    }

                    // 判断教师是否存在
                    SqlCommand checkCmd = new SqlCommand("SELECT COUNT(*) FROM Teacher WHERE 工号 = @tid", conn);
                    checkCmd.Parameters.AddWithValue("@tid", tid);
                    int exists = (int)checkCmd.ExecuteScalar();

                    if (exists > 0)
                    {
                        // 更新
                        string updateSql = @"UPDATE Teacher 
                                     SET 姓名=@name, 性别=@gender, 出生日期=@birth, 联系电话=@phone, 学院编号=@college 
                                     WHERE 工号=@tid";
                        SqlCommand updateCmd = new SqlCommand(updateSql, conn);
                        updateCmd.Parameters.AddWithValue("@tid", tid);
                        updateCmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        updateCmd.Parameters.AddWithValue("@gender", gender ?? (object)DBNull.Value);
                        updateCmd.Parameters.AddWithValue("@birth", birth.HasValue ? (object)birth.Value : DBNull.Value);
                        updateCmd.Parameters.AddWithValue("@phone", phone ?? (object)DBNull.Value);
                        updateCmd.Parameters.AddWithValue("@college", string.IsNullOrWhiteSpace(collegeId) ? DBNull.Value : (object)collegeId);
                        updateCmd.ExecuteNonQuery();
                    }
                    else
                    {
                        // 插入
                        string insertSql = @"INSERT INTO Teacher 
                                     (工号, 姓名, 性别, 出生日期, 联系电话, 学院编号) 
                                     VALUES (@tid, @name, @gender, @birth, @phone, @college)";
                        SqlCommand insertCmd = new SqlCommand(insertSql, conn);
                        insertCmd.Parameters.AddWithValue("@tid", tid);
                        insertCmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        insertCmd.Parameters.AddWithValue("@gender", gender ?? (object)DBNull.Value);
                        insertCmd.Parameters.AddWithValue("@birth", birth.HasValue ? (object)birth.Value : DBNull.Value);
                        insertCmd.Parameters.AddWithValue("@phone", phone ?? (object)DBNull.Value);
                        insertCmd.Parameters.AddWithValue("@college", string.IsNullOrWhiteSpace(collegeId) ? DBNull.Value : (object)collegeId);
                        insertCmd.ExecuteNonQuery();
                    }
                }

                MessageBox.Show("保存成功！");
                LoadTeachers(); // 实时刷新
            }
        }

        // 删除
        private void button4_Click(object sender, EventArgs e)
        {
            if (dataGridView1.CurrentRow != null)
            {
                string tid = dataGridView1.CurrentRow.Cells["工号"].Value?.ToString();
                if (!string.IsNullOrWhiteSpace(tid))
                {
                    DialogResult dr = MessageBox.Show($"是否删除工号为 {tid} 的教师？", "确认删除", MessageBoxButtons.YesNo);
                    if (dr == DialogResult.Yes)
                    {
                        using (SqlConnection conn = new SqlConnection(connStr))
                        {
                            conn.Open();
                            string sql = "DELETE FROM Teacher WHERE 工号 = @tid";
                            SqlCommand cmd = new SqlCommand(sql, conn);
                            cmd.Parameters.AddWithValue("@tid", tid);
                            cmd.ExecuteNonQuery();
                        }
                        LoadTeachers(); // 删除后刷新
                    }
                }
            }
        }
        // 退出
        private void button5_Click(object sender, EventArgs e)
        {
            parentForm?.Show();
            this.Close();
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
    }
}
