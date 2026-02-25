using System;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace load
{
    public partial class StatMajorEmploymentRateForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;

        public StatMajorEmploymentRateForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "各专业就业率";
        }

        private void StatMajorEmploymentRateForm_Load(object sender, EventArgs e)
        {
            LoadStats();
        }

        private void LoadStats()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = @"
                    SELECT 
                        M.专业名称,
                        COUNT(G.学号) AS 总人数,
                        SUM(CASE WHEN G.单位编号 IS NOT NULL THEN 1 ELSE 0 END) AS 就业人数,
                        CAST(
                            100.0 * SUM(CASE WHEN G.单位编号 IS NOT NULL THEN 1 ELSE 0 END) / COUNT(G.学号) 
                            AS DECIMAL(5,2)
                        ) AS 就业率百分比
                    FROM Graduate G
                    JOIN Major M ON G.专业编号 = M.专业编号
                    GROUP BY M.专业名称
                    ORDER BY 就业率百分比 DESC";

                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;
                dataGridView1.RowHeadersVisible = false;
                // 样式优化
                dataGridView1.ReadOnly = true;
                dataGridView1.AllowUserToAddRows = false;
                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
                dataGridView1.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView1.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            }
        }

        private void buttonBack_Click(object sender, EventArgs e)
        {
            parentForm?.Show();
            this.Close();
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
    }
}
