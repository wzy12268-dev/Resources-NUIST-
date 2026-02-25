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
    public partial class EmployerViewForm : Form
    {
        private Form parentForm;
        private string connStr = "server=localhost;database=database;uid=sa;pwd=122122;";

        public EmployerViewForm(Form parent)
        {
            InitializeComponent();
            parentForm = parent;
            LoadEmployerData();
            this.Text = "用人单位查看";
        }
        private void LoadEmployerData()
        {
            using (SqlConnection conn = new SqlConnection(connStr))
            {
                string sql = @"
            SELECT 
                E.单位编号,
                E.单位名称,
                E.单位地址,
                E.联系电话,
                I.行业名称
            FROM Employer E
            JOIN Industry I ON E.行业编号 = I.行业编号";

                SqlDataAdapter adapter = new SqlDataAdapter(sql, conn);
                DataTable dt = new DataTable();
                adapter.Fill(dt);

                dataGridView1.DataSource = dt;
                // 去除索引
                dataGridView1.RowHeadersVisible = false;

                // 样式配置
                dataGridView1.ReadOnly = true;
                dataGridView1.AllowUserToAddRows = false;
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

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
    }
}
