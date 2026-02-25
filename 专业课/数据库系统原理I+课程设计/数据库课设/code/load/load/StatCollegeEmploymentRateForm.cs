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
using System.Windows.Forms.DataVisualization.Charting;

namespace load
{
    public partial class StatCollegeEmploymentRateForm : Form
    {
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";
        private Form parentForm;
        public StatCollegeEmploymentRateForm(Form parent = null)
        {
            InitializeComponent();
            parentForm = parent;
            this.Text = "各学院就业率";
        }
        private void StatCollegeEmploymentRateForm_Load(object sender, EventArgs e)
        {
            LoadEmploymentStats();
        }

        private void LoadEmploymentStats()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = @"
                    SELECT 
                        D.学院名称,
                        COUNT(G.学号) AS 总人数,
                        SUM(CASE WHEN G.单位编号 IS NOT NULL THEN 1 ELSE 0 END) AS 就业人数,
                        CAST(
                            100.0 * SUM(CASE WHEN G.单位编号 IS NOT NULL THEN 1 ELSE 0 END) / COUNT(G.学号) 
                            AS DECIMAL(5,2)
                        ) AS 就业率百分比
                    FROM Graduate G
                    JOIN Major M ON G.专业编号 = M.专业编号
                    JOIN College D ON M.学院编号 = D.学院编号
                    GROUP BY D.学院名称
                    ORDER BY 就业率百分比 DESC";

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
