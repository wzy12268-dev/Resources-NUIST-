using System;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace load
{
    public partial class StatEmployerEmploymentCountForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;

        public StatEmployerEmploymentCountForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "用人单位就业人数";
        }

        private void StatEmployerEmploymentCountForm_Load(object sender, EventArgs e)
        {
            LoadEmploymentCount();
        }

        private void LoadEmploymentCount()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = @"
                    SELECT 
                        E.单位名称 AS 用人单位, 
                        COUNT(G.学号) AS 就业人数
                    FROM Graduate G
                    JOIN Employer E ON G.单位编号 = E.单位编号
                    WHERE G.单位编号 IS NOT NULL
                    GROUP BY E.单位名称
                    ORDER BY 就业人数 DESC";

                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;
                dataGridView1.RowHeadersVisible = false;
                // 表格优化
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
