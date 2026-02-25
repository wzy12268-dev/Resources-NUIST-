using System;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Windows.Forms;

namespace load
{
    public partial class AdminUserAccountManageForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;

        public AdminUserAccountManageForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "用户信息管理";
        }

        private void AdminUserAccountManageForm_Load(object sender, EventArgs e)
        {
            LoadUsers();
        }

        private void LoadUsers(string uid = "")
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                conn.Open();
                string sql = "SELECT * FROM UserAccount WHERE 用户编号 LIKE @uid";
                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                adapter.SelectCommand.Parameters.AddWithValue("@uid", "%" + uid + "%");
                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                dataGridView1.ReadOnly = false;
               
                dataGridView1.RowHeadersVisible = false;

                dataGridView1.AllowUserToAddRows = false;
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
                        row.Cells["用户编号"].ReadOnly = true;
                }
            }
        }

        // 查询
        private void button1_Click(object sender, EventArgs e)
        {
            string uid = textBox1.Text.Trim();
            LoadUsers(uid);
        }

        // 添加
        private void button4_Click(object sender, EventArgs e)
        {
            DataTable dt = (DataTable)dataGridView1.DataSource;
            DataRow newRow = dt.NewRow();
            dt.Rows.Add(newRow);
        }

        // 删除
        private void button2_Click(object sender, EventArgs e)
        {
            if (dataGridView1.CurrentRow != null)
            {
                string uid = dataGridView1.CurrentRow.Cells["用户编号"].Value?.ToString();
                if (!string.IsNullOrWhiteSpace(uid))
                {
                    DialogResult dr = MessageBox.Show($"确认删除用户 {uid}？", "确认", MessageBoxButtons.YesNo);
                    if (dr == DialogResult.Yes)
                    {
                        using (SqlConnection conn = new SqlConnection(connStr))
                        {
                            conn.Open();
                            SqlCommand cmd = new SqlCommand("DELETE FROM UserAccount WHERE 用户编号=@uid", conn);
                            cmd.Parameters.AddWithValue("@uid", uid);
                            cmd.ExecuteNonQuery();
                        }
                        LoadUsers();
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

                    string uid = row.Cells["用户编号"].Value?.ToString()?.Trim();
                    string pwd = row.Cells["密码"].Value?.ToString()?.Trim();
                    string type = row.Cells["用户类型"].Value?.ToString()?.Trim();

                    if (string.IsNullOrWhiteSpace(uid)) continue;

                    // —— 前缀与类型对应校验 —— 
                    char prefix = uid[0];
                    bool ok =
                        (prefix == 'S' && type == "S") ||  // 学生
                        (prefix == 'T' && type == "T") ||  // 老师
                        (prefix == 'A' && type == "A");    // 管理员
                    if (!ok)
                    {
                        MessageBox.Show(
                            $"保存失败（用户 {uid}）：编号前缀 “{prefix}” 与用户类型 “{type}” 不匹配。\n" +
                            "学生编号须以 S 开头，教师以 T，管理员以 A。",
                            "校验错误",
                            MessageBoxButtons.OK,
                            MessageBoxIcon.Warning
                        );
                        LoadUsers();    // ← 刷新
                        return;  // 或者 continue，跳过这一行看你的业务需求
                    }

                    SqlCommand checkCmd = new SqlCommand(
               "SELECT COUNT(*) FROM UserAccount WHERE 用户编号 = @uid", conn);
                    checkCmd.Parameters.AddWithValue("@uid", uid);
                    int exists = (int)checkCmd.ExecuteScalar();


                    try
                    {
                        if (exists > 0)
                        {
                            var update = new SqlCommand(
                                "UPDATE UserAccount SET 密码 = @pwd, 用户类型 = @type WHERE 用户编号 = @uid", conn);
                            update.Parameters.AddWithValue("@pwd", pwd ?? (object)DBNull.Value);
                            update.Parameters.AddWithValue("@type", type);
                            update.Parameters.AddWithValue("@uid", uid);
                            update.ExecuteNonQuery();
                        }
                        else
                        {
                            var insert = new SqlCommand(
                                "INSERT INTO UserAccount (用户编号, 密码, 用户类型) VALUES (@uid, @pwd, @type)", conn);
                            insert.Parameters.AddWithValue("@uid", uid);
                            insert.Parameters.AddWithValue("@pwd", pwd ?? (object)DBNull.Value);
                            insert.Parameters.AddWithValue("@type", type);
                            insert.ExecuteNonQuery();
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"保存失败（用户 {uid}）：{ex.Message}", "异常", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        LoadUsers();    // ← 保存异常也刷新
                        return;
                    }
                }

                MessageBox.Show("保存成功！", "完成", MessageBoxButtons.OK, MessageBoxIcon.Information);
                LoadUsers();  // 最终成功后刷新
            }
        }

        // 退出
        private void button5_Click(object sender, EventArgs e)
        {
            parentForm?.Show();
            this.Close();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
