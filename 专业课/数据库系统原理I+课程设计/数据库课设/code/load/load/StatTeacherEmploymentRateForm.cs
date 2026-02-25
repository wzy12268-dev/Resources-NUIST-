using System;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace load
{
    public partial class StatTeacherEmploymentRateForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private string teacherId;
        private Form parentForm;

        public StatTeacherEmploymentRateForm(string teacherId, Form parent = null)
        {
            InitializeComponent();
            this.teacherId = teacherId;
            this.parentForm = parent;
            this.Text = "教师管理学生就业率";
        }

        private void StatTeacherEmploymentRateForm_Load(object sender, EventArgs e)
        {
            LoadTeacherEmploymentStats();
        }

        private void LoadTeacherEmploymentStats()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = @"
                    SELECT 
                        T.姓名 AS 教师姓名,
                        COUNT(G.学号) AS 指导总人数,
                        SUM(CASE WHEN G.单位编号 IS NOT NULL THEN 1 ELSE 0 END) AS 就业人数,
                        CASE 
                            WHEN COUNT(G.学号) = 0 THEN NULL
                            ELSE CAST(100.0 * SUM(CASE WHEN G.单位编号 IS NOT NULL THEN 1 ELSE 0 END) / COUNT(G.学号) AS DECIMAL(5,2))
                        END AS 就业率百分比
                    FROM Teacher T
                    LEFT JOIN Graduate G ON T.工号 = G.工号
                    WHERE T.工号 = @tid
                    GROUP BY T.姓名";

                SqlCommand cmd = new SqlCommand(sql, conn);
                cmd.Parameters.AddWithValue("@tid", teacherId);

                SqlDataAdapter adapter = new SqlDataAdapter(cmd);
                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;
                dataGridView1.RowHeadersVisible = false;
                dataGridView1.ReadOnly = true;
                dataGridView1.AllowUserToAddRows = false;
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
                dataGridView1.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView1.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            parentForm?.Show();
            this.Close();
        }
    }
}
