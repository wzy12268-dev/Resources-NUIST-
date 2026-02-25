using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Linq;

namespace load
{
    public partial class StudentManageForm : Form
    {
        private string teacherID;   // 教师工号
        private Form parentForm;    // 上级窗体
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";

        // 【新增】专业字典，用于编辑时同步名称
        private Dictionary<string, string> majorDict = new Dictionary<string, string>();

        public StudentManageForm(string tid, Form parent)
        {
            InitializeComponent();
            teacherID = tid;
            parentForm = parent;
            this.Text = "学生信息管理";

            // 【新增】确保单元格值改变能触发事件
            dataGridView1.EditMode = DataGridViewEditMode.EditOnEnter;
            dataGridView1.CellEndEdit += DataGridView1_CellEndEdit;
            dataGridView1.CellValueChanged += DataGridView1_CellValueChanged;
            dataGridView1.CellFormatting += DataGridView1_CellFormatting;
        }

        private void StudentManageForm_Load(object sender, EventArgs e)
        {
            LoadStudentData();

            // 列只读 & 重命名
            if (dataGridView1.Columns.Contains("学号"))
                dataGridView1.Columns["学号"].ReadOnly = true;
            if (dataGridView1.Columns.Contains("密码"))
                dataGridView1.Columns["密码"].ReadOnly = true;   // 教师不可修改密码
            if (dataGridView1.Columns.Contains("工号"))
                dataGridView1.Columns["工号"].HeaderText = "指导老师工号";

            // 外观设置
            dataGridView1.AllowUserToAddRows = false;
            dataGridView1.RowHeadersVisible = false;
            dataGridView1.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            dataGridView1.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            //dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
        }

        private void LoadStudentData()
        {
            using (var conn = new SqlConnection(connStr))
            {
                conn.Open();

                // 【新增】预加载所有专业编号→名称的映射
                majorDict.Clear();
                using (var cmd = new SqlCommand("SELECT 专业编号, 专业名称 FROM Major", conn))
                using (var reader = cmd.ExecuteReader())
                    while (reader.Read())
                        majorDict[reader.GetString(0)] = reader.GetString(1);

                // 拉学生数据并附带专业名称
                string sql = @"
SELECT
    G.*,
    U.密码,
    M.专业名称
FROM Graduate G
LEFT JOIN UserAccount U
    ON G.学号 = U.用户编号
LEFT JOIN Major M
    ON G.专业编号 = M.专业编号
WHERE G.工号 = @tid";

                var adapter = new SqlDataAdapter(sql, conn);
                adapter.SelectCommand.Parameters.AddWithValue("@tid", teacherID);

                var dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;

                // 确保已有行的“学号”“工号”只读
                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (!row.IsNewRow)
                    {
                        row.Cells["学号"].ReadOnly = true;
                        row.Cells["工号"].ReadOnly = true;
                    }
                }

                // 把“专业名称”列放到“专业编号”之后
                if (dataGridView1.Columns.Contains("专业名称") &&
                    dataGridView1.Columns.Contains("专业编号"))
                {
                    int idx = dataGridView1.Columns["专业编号"].DisplayIndex;
                    dataGridView1.Columns["专业名称"].DisplayIndex = idx + 1;
                }
            }
        }

        // —— 密码掩码显示 —— 修复了参数名冲突（用 args 代替 e）
        private void DataGridView1_CellFormatting(object sender, DataGridViewCellFormattingEventArgs args)
        {
            var col = dataGridView1.Columns[args.ColumnIndex];
            if (col.Name == "密码" && args.Value != null)
            {
                string real = args.Value.ToString();
                args.Value = new string('*', real.Length);
                args.FormattingApplied = true;
            }
        }

        // —— 确保编辑后的值能提交触发 CellValueChanged
        private void DataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            dataGridView1.CommitEdit(DataGridViewDataErrorContexts.Commit);
        }

        // —— 专业编号变动时，同步更新专业名称列
        private void DataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (dataGridView1.Columns[e.ColumnIndex].Name == "专业编号" && e.RowIndex >= 0)
            {
                var row = dataGridView1.Rows[e.RowIndex];
                var code = row.Cells["专业编号"].Value?.ToString();
                string name = null;
                if (!string.IsNullOrEmpty(code) && majorDict.TryGetValue(code, out name))
                    row.Cells["专业名称"].Value = name;
                else
                    row.Cells["专业名称"].Value = "";  // 未找到则清空
            }
        }

        // 保存按钮
        // —— 保存逻辑（全部用 col.Name 而非 HeaderText） —— 
        private void button3_Click(object sender, EventArgs e)
        {
            if (dataGridView1.DataSource == null) return;
            var table = (DataTable)dataGridView1.DataSource;

            using (var conn = new SqlConnection(connStr))
            {
                conn.Open();

                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (row.IsNewRow) continue;
                    string id = row.Cells["学号"].Value?.ToString();
                    if (string.IsNullOrWhiteSpace(id)) continue;

                    var fieldNames = new List<string>();
                    var paramNames = new List<string>();
                    var cmd = new SqlCommand { Connection = conn };

                    foreach (DataGridViewColumn col in dataGridView1.Columns)
                    {
                        string key = col.Name;
                        // 跳过这些非 Graduate 字段
                        if (key == "学号"
                         || key == "密码"
                         || key == "专业名称")    // ← 新增
                            continue;

                        object val = row.Cells[key].Value ?? DBNull.Value;
                        fieldNames.Add(key);
                        paramNames.Add("@" + key);
                        cmd.Parameters.AddWithValue("@" + key, val);
                    }


                    cmd.CommandText = "UPDATE Graduate SET "
                        + string.Join(", ",
                            fieldNames.Zip(paramNames, (f, p) => $"{f} = {p}"))
                        + " WHERE 学号 = @id";
                    cmd.Parameters.AddWithValue("@id", id);

                    try
                    {
                        cmd.ExecuteNonQuery();

                        // 密码写回（虽然老师看不到真实值）
                        string pwd = row.Cells["密码"].Value?.ToString() ?? "";
                        using (var up = new SqlCommand(
                            "UPDATE UserAccount SET 密码 = @pwd WHERE 用户编号 = @id", conn))
                        {
                            up.Parameters.AddWithValue("@pwd", pwd);
                            up.Parameters.AddWithValue("@id", id);
                            up.ExecuteNonQuery();
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"保存失败（学号 {id}）：{ex.Message}");
                    }
                }

                MessageBox.Show("保存成功！");
                LoadStudentData();
            }
        }




        // 返回
        private void button4_Click(object sender, EventArgs e)
        {
            parentForm?.Show();
            this.Close();
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            // 留空或后续扩展
        }
    }
}
