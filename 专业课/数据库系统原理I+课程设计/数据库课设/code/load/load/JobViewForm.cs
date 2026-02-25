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
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;

namespace load
{
    public partial class JobViewForm : Form
    {
        private Form parentForm;
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";

        public JobViewForm(Form parent)
        {
           
            InitializeComponent();
            parentForm = parent;
            LoadJobData();
            this.Text = "岗位招聘查询";
        }
        private void JobViewFor_Load(object sender, EventArgs e)
        {
            LoadJobData();
            dataGridView1.AllowUserToAddRows = false;

        }
        private void LoadJobData()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = "SELECT * FROM JobPosting";
                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                DataTable dt = new DataTable();
                adapter.Fill(dt);
                dataGridView1.DataSource = dt;
                dataGridView1.ReadOnly = true;

                dataGridView1.AllowUserToAddRows = false;
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

        private void button1_Click(object sender, EventArgs e)
        {
            parentForm.Show();
            this.Close();
        }
    }
}
