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
    public partial class AdminStudentManageForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;

        public AdminStudentManageForm(Form parent)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "学生信息管理";
        }

        private void AdminStudentManageForm_Load(object sender, EventArgs e)
        {
            LoadStudents();
        }
        private void LoadStudents(string sid = "", string name = "", string majorName = "", string collegeName = "")
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();

                // ===== 查询专业编号（通过专业名称） =====
                string majorId = "";
                if (!string.IsNullOrWhiteSpace(majorName))
                {
                    SqlCommand cmd = new SqlCommand("SELECT TOP 1 专业编号 FROM Major WHERE 专业名称 LIKE @major", conn);
                    cmd.Parameters.AddWithValue("@major", "%" + majorName + "%");
                    object result = cmd.ExecuteScalar();
                    if (result != null)
                    {
                        majorId = result.ToString();
                    }
                    else
                    {
                        // ❗查不到专业名称 → 返回空表
                        dataGridView1.DataSource = new DataTable();
                        return;
                    }
                }

                // ===== 查询学院名称对应的所有专业编号 =====
                List<string> majorIdsFromCollege = new List<string>();
                if (!string.IsNullOrWhiteSpace(collegeName))
                {
                    SqlCommand cmd = new SqlCommand(
                        @"SELECT M.专业编号 
                  FROM Major M 
                  JOIN College C ON M.学院编号 = C.学院编号 
                  WHERE C.学院名称 LIKE @college", conn);
                    cmd.Parameters.AddWithValue("@college", "%" + collegeName + "%");

                    using (SqlDataReader reader = cmd.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            majorIdsFromCollege.Add(reader.GetString(0));
                        }
                    }

                    // ❗查不到学院下的专业 → 返回空表
                    if (majorIdsFromCollege.Count == 0)
                    {
                        dataGridView1.DataSource = new DataTable();
                        return;
                    }
                }

                // ===== 构建主查询 SQL =====
                string baseSql = @"SELECT 学号, 姓名, 性别, 出生日期, 联系电话, 专业编号, 工号, 单位编号 
                           FROM Graduate 
                           WHERE 学号 LIKE @sid AND 姓名 LIKE @name";

                SqlCommand mainCmd = new SqlCommand();
                mainCmd.Connection = conn;
                mainCmd.CommandText = baseSql;

                // 基础参数
                mainCmd.Parameters.AddWithValue("@sid", "%" + sid + "%");
                mainCmd.Parameters.AddWithValue("@name", "%" + name + "%");

                // 按专业名称过滤
                if (!string.IsNullOrWhiteSpace(majorId))
                {
                    mainCmd.CommandText += " AND 专业编号 LIKE @majorId";
                    mainCmd.Parameters.AddWithValue("@majorId", "%" + majorId + "%");
                }

                // 按学院名称过滤（IN 多个专业编号）
                if (majorIdsFromCollege.Count > 0)
                {
                    string inClause = string.Join(",", majorIdsFromCollege.Select((id, idx) => $"@major_in_{idx}"));
                    mainCmd.CommandText += $" AND 专业编号 IN ({inClause})";

                    for (int i = 0; i < majorIdsFromCollege.Count; i++)
                    {
                        mainCmd.Parameters.AddWithValue($"@major_in_{i}", majorIdsFromCollege[i]);
                    }
                }

                // ===== 执行查询并绑定 =====
                SqlDataAdapter adapter = new SqlDataAdapter(mainCmd);
                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                // ===== 表格样式设置 =====
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

                // 锁定学号列
                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (!row.IsNewRow)
                        row.Cells["学号"].ReadOnly = true;
                }
            }
        }


        // 学号
        private void label1_Click(object sender, EventArgs e)
        {

        }
        // 姓名
        private void label2_Click(object sender, EventArgs e)
        {

        }
        // 学院
        private void label4_Click(object sender, EventArgs e)
        {

        }
        // 查询
        private void button1_Click(object sender, EventArgs e)
        {
            string sid = textBox1.Text.Trim();
            string name = textBox2.Text.Trim();
            string major = textBox3.Text.Trim();
            string college = textBox4.Text.Trim();
            LoadStudents(sid, name, major, college);
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

                    string id = row.Cells["学号"].Value?.ToString()?.Trim();
                    if (string.IsNullOrWhiteSpace(id)) continue;

                    string name = row.Cells["姓名"].Value?.ToString()?.Trim();
                    string gender = row.Cells["性别"].Value?.ToString()?.Trim();
                    string phone = row.Cells["联系电话"].Value?.ToString()?.Trim();
                    string unit = row.Cells["单位编号"].Value?.ToString()?.Trim();
                    string majorId = row.Cells["专业编号"].Value?.ToString()?.Trim();
                    string teacherId = row.Cells["工号"].Value?.ToString()?.Trim();

                    DateTime? birth = row.Cells["出生日期"].Value == DBNull.Value || row.Cells["出生日期"].Value == null
                        ? (DateTime?)null
                        : Convert.ToDateTime(row.Cells["出生日期"].Value);

                    // ==== 单位编号检查 ====
                    if (!string.IsNullOrWhiteSpace(unit))
                    {
                        SqlCommand unitCheck = new SqlCommand("SELECT COUNT(*) FROM Employer WHERE 单位编号 = @uid", conn);
                        unitCheck.Parameters.AddWithValue("@uid", unit);
                        if ((int)unitCheck.ExecuteScalar() == 0)
                        {
                            MessageBox.Show($"单位编号 {unit} 不存在，已置空！");
                            unit = null;
                        }
                    }

                    // ==== 专业编号检查 ====
                    bool majorValid = true;
                    if (!string.IsNullOrWhiteSpace(majorId))
                    {
                        SqlCommand majorCheck = new SqlCommand("SELECT COUNT(*) FROM Major WHERE 专业编号 = @mid", conn);
                        majorCheck.Parameters.AddWithValue("@mid", majorId);
                        if ((int)majorCheck.ExecuteScalar() == 0)
                        {
                            MessageBox.Show($"专业编号 {majorId} 不存在，已置空！");
                            majorId = null;
                            majorValid = false;
                        }
                    }

                    // ==== 教师工号检查 ====
                    bool teacherValid = true;
                    if (!string.IsNullOrWhiteSpace(teacherId))
                    {
                        SqlCommand teacherCheck = new SqlCommand("SELECT COUNT(*) FROM Teacher WHERE 工号 = @tid", conn);
                        teacherCheck.Parameters.AddWithValue("@tid", teacherId);
                        if ((int)teacherCheck.ExecuteScalar() == 0)
                        {
                            MessageBox.Show($"教师工号 {teacherId} 不存在，已置空！");
                            teacherId = null;
                            teacherValid = false;
                        }
                    }

                    // ==== 专业与教师所属学院一致性校验 ====
                    if (!string.IsNullOrWhiteSpace(majorId) && !string.IsNullOrWhiteSpace(teacherId))
                    {
                        string getMajorCollege = "SELECT 学院编号 FROM Major WHERE 专业编号 = @mid";
                        string getTeacherCollege = "SELECT 学院编号 FROM Teacher WHERE 工号 = @tid";

                        SqlCommand majorCmd = new SqlCommand(getMajorCollege, conn);
                        majorCmd.Parameters.AddWithValue("@mid", majorId);
                        string majorCollegeId = majorCmd.ExecuteScalar()?.ToString();

                        SqlCommand teacherCmd = new SqlCommand(getTeacherCollege, conn);
                        teacherCmd.Parameters.AddWithValue("@tid", teacherId);
                        string teacherCollegeId = teacherCmd.ExecuteScalar()?.ToString();

                        if (majorCollegeId != null && teacherCollegeId != null && majorCollegeId != teacherCollegeId)
                        {
                            MessageBox.Show($"教师 {teacherId} 与专业 {majorId} 不属于同一学院，两者已置空！");
                            majorId = null;
                            teacherId = null;
                        }
                    }

                    // ==== 判断记录是否存在 ====
                    SqlCommand checkCmd = new SqlCommand("SELECT COUNT(*) FROM Graduate WHERE 学号 = @id", conn);
                    checkCmd.Parameters.AddWithValue("@id", id);
                    int exists = (int)checkCmd.ExecuteScalar();

                    if (exists > 0)
                    {
                        // ==== 更新 ====
                        string updateSql = @"UPDATE Graduate 
                                     SET 姓名=@name, 性别=@gender, 出生日期=@birth, 联系电话=@phone, 
                                         专业编号=@major, 工号=@teacher, 单位编号=@unit 
                                     WHERE 学号=@id";
                        SqlCommand updateCmd = new SqlCommand(updateSql, conn);
                        updateCmd.Parameters.AddWithValue("@id", id);
                        updateCmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        updateCmd.Parameters.AddWithValue("@gender", gender ?? (object)DBNull.Value);
                        updateCmd.Parameters.AddWithValue("@birth", birth.HasValue ? (object)birth.Value : DBNull.Value);
                        updateCmd.Parameters.AddWithValue("@phone", phone ?? (object)DBNull.Value);
                        updateCmd.Parameters.AddWithValue("@major", string.IsNullOrWhiteSpace(majorId) ? DBNull.Value : (object)majorId);
                        updateCmd.Parameters.AddWithValue("@teacher", string.IsNullOrWhiteSpace(teacherId) ? DBNull.Value : (object)teacherId);
                        updateCmd.Parameters.AddWithValue("@unit", string.IsNullOrWhiteSpace(unit) ? DBNull.Value : (object)unit);
                        updateCmd.ExecuteNonQuery();
                    }
                    else
                    {
                        // ==== 插入 ====
                        string insertSql = @"INSERT INTO Graduate 
                                     (学号, 姓名, 性别, 出生日期, 联系电话, 专业编号, 工号, 单位编号) 
                                     VALUES (@id, @name, @gender, @birth, @phone, @major, @teacher, @unit)";
                        SqlCommand insertCmd = new SqlCommand(insertSql, conn);
                        insertCmd.Parameters.AddWithValue("@id", id);
                        insertCmd.Parameters.AddWithValue("@name", name ?? (object)DBNull.Value);
                        insertCmd.Parameters.AddWithValue("@gender", gender ?? (object)DBNull.Value);
                        insertCmd.Parameters.AddWithValue("@birth", birth.HasValue ? (object)birth.Value : DBNull.Value);
                        insertCmd.Parameters.AddWithValue("@phone", phone ?? (object)DBNull.Value);
                        insertCmd.Parameters.AddWithValue("@major", string.IsNullOrWhiteSpace(majorId) ? DBNull.Value : (object)majorId);
                        insertCmd.Parameters.AddWithValue("@teacher", string.IsNullOrWhiteSpace(teacherId) ? DBNull.Value : (object)teacherId);
                        insertCmd.Parameters.AddWithValue("@unit", string.IsNullOrWhiteSpace(unit) ? DBNull.Value : (object)unit);
                        insertCmd.ExecuteNonQuery();
                    }
                }

                MessageBox.Show("保存成功！");
                LoadStudents(); // 立即刷新显示
            }
        }

        // 删除
        private void button4_Click(object sender, EventArgs e)
        {
            if (dataGridView1.CurrentRow != null)
            {
                string id = dataGridView1.CurrentRow.Cells["学号"].Value.ToString();
                DialogResult dr = MessageBox.Show($"是否删除学号为 {id} 的学生？", "确认删除", MessageBoxButtons.YesNo);
                if (dr == DialogResult.Yes)
                {
                    using (SqlConnection conn = new SqlConnection(connStr))
                    {
                        conn.Open();
                        string sql = "DELETE FROM Graduate WHERE 学号=@id";
                        SqlCommand cmd = new SqlCommand(sql, conn);
                        cmd.Parameters.AddWithValue("@id", id);
                        cmd.ExecuteNonQuery();
                    }
                    LoadStudents();
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
