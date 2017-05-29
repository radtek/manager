using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Data.SqlClient;

namespace Restaurante
{
    public partial class CompraConsultaBayes : Form
    {
        public CompraConsultaBayes()
        {
            InitializeComponent();
        }

        private List<String> annios()
        {
            List<String> list_annios = new List<String>();
            bool check = this.checkBox_2012.Checked;
            if (check == true)
            {
                list_annios.Add("2012");
            }
            check = this.checkBox_2013.Checked;
            if (check == true)
            {
                list_annios.Add("2013");
            }
            check = this.checkBox_2014.Checked;
            if (check == true)
            {
                list_annios.Add("2014");
            }
            check = this.checkBox_2015.Checked;
            if (check == true)
            {
                list_annios.Add("2015");
            }
            check = this.checkBox_2016.Checked;
            if (check == true)
            {
                list_annios.Add("2016");
            }
            check = this.checkBox_2017.Checked;
            if (check == true)
            {
                list_annios.Add("2017");
            }
            return list_annios;
        }
        private List<int> meses()
        {
            List<int> list_meses = new List<int>();
            bool check = this.checkBox_enero.Checked;
            if (check == true)
            {
                list_meses.Add(1);
            }
            check = this.checkBox_febrero.Checked;
            if (check == true)
            {
                list_meses.Add(2);
            }
            check = this.checkBox_marzo.Checked;
            if (check == true)
            {
                list_meses.Add(3);
            }
            check = this.checkBox_abril.Checked;
            if (check == true)
            {
                list_meses.Add(4);
            }
            check = this.checkBox_mayo.Checked;
            if (check == true)
            {
                list_meses.Add(5);
            }
            check = this.checkBox_junio.Checked;
            if (check == true)
            {
                list_meses.Add(6);
            }
            check = this.checkBox_julio.Checked;
            if (check == true)
            {
                list_meses.Add(7);
            }
            check = this.checkBox_agosto.Checked;
            if (check == true)
            {
                list_meses.Add(8);
            }
            check = this.checkBox_septiembre.Checked;
            if (check == true)
            {
                list_meses.Add(9);
            }
            check = this.checkBox_octubre.Checked;
            if (check == true)
            {
                list_meses.Add(10);
            }
            check = this.checkBox_noviembre.Checked;
            if (check == true)
            {
                list_meses.Add(11);
            }
            check = this.checkBox_diciembre.Checked;
            if (check == true)
            {
                list_meses.Add(12);
            }
            return list_meses;
        }
        private List<int> semanas()
        {
            List<int> list_semanas = new List<int>();
            bool check = this.checkBox_semana_1.Checked;
            if (check == true)
            {
                list_semanas.Add(1);
            }
            check = this.checkBox_semana_2.Checked;
            if (check == true)
            {
                list_semanas.Add(2);
            }
            check = this.checkBox_semana_3.Checked;
            if (check == true)
            {
                list_semanas.Add(3);
            }
            check = this.checkBox_semana_4.Checked;
            if (check == true)
            {
                list_semanas.Add(4);
            }
            check = this.checkBox_semana_5.Checked;
            if (check == true)
            {
                list_semanas.Add(5);
            }
            check = this.checkBox_semana_6.Checked;
            if (check == true)
            {
                list_semanas.Add(6);
            }
            return list_semanas;
        }
        private List<int> dias()
        {
            List<int> list_dias = new List<int>();
            bool check = this.checkBox_lunes.Checked;
            if (check == true)
            {
                list_dias.Add(1);
            }
            check = this.checkBox_martes.Checked;
            if (check == true)
            {
                list_dias.Add(2);
            }
            check = this.checkBox_miercoles.Checked;
            if (check == true)
            {
                list_dias.Add(3);
            }
            check = this.checkBox_jueves.Checked;
            if (check == true)
            {
                list_dias.Add(4);
            }
            check = this.checkBox_viernes.Checked;
            if (check == true)
            {
                list_dias.Add(5);
            }
            check = this.checkBox_sabado.Checked;
            if (check == true)
            {
                list_dias.Add(6);
            }
            check = this.checkBox_domingo.Checked;
            if (check == true)
            {
                list_dias.Add(7);
            }

            return list_dias;
        }
        private void button_bayes_Click(object sender, EventArgs e)
        {
            string connetionString = null;
            SqlConnection connection;
            SqlCommand command;
            string sql = null;
            //SqlDataReader dataReader;
            connetionString = "Data Source=DESKTOP-TJMNUQT;Initial Catalog=restaurante;Integrated Security=True;";
            connection = new SqlConnection(connetionString);

            try
            {
                connection.Open();
            }
            catch (SqlException ex)
            {
                MessageBox.Show("Can not open connection ! ");
            }

            sql = "DELETE FROM busqueda_annio";
            command = new SqlCommand(sql, connection);

            command.ExecuteNonQuery();
            command.Dispose();

            sql = "DELETE FROM busqueda_mes";
            command = new SqlCommand(sql, connection);

            command.ExecuteNonQuery();
            command.Dispose();

            sql = "DELETE FROM busqueda_dia";
            command = new SqlCommand(sql, connection);

            command.ExecuteNonQuery();
            command.Dispose();

            List<String> list_annios = annios();
            for (int i = 0; i < list_annios.Count; i++)
            {
                sql = "INSERT INTO busqueda_annio(annio)VALUES('" + list_annios[i] + "')";
                command = new SqlCommand(sql, connection);

                command.ExecuteNonQuery();
                command.Dispose();
            }
            List<int> list_meses = meses();
            for (int i = 0; i < list_meses.Count; i++)
            {
                sql = "INSERT INTO busqueda_mes(mes)VALUES('" + list_meses[i].ToString() + "')";
                command = new SqlCommand(sql, connection);

                command.ExecuteNonQuery();
                command.Dispose();
            }
            List<int> list_semana = semanas();
            for (int i = 0; i < list_semana.Count; i++)
            {
                sql = "INSERT INTO busqueda_semana(semana)VALUES('" + list_semana[i].ToString() + "')";
                command = new SqlCommand(sql, connection);

                command.ExecuteNonQuery();
                command.Dispose();
            }
            List<int> list_dias = dias();
            for (int i = 0; i < list_dias.Count; i++)
            {
                sql = "INSERT INTO busqueda_dia(dia)VALUES('" + list_dias[i].ToString() + "')";
                command = new SqlCommand(sql, connection);

                command.ExecuteNonQuery();
                command.Dispose();
            }
            connection.Close();


            string startupPath = System.IO.Directory.GetCurrentDirectory();

            ProcessStartInfo startInfo = new ProcessStartInfo();
            //startInfo.WindowStyle = ProcessWindowStyle.Hidden;
            startInfo.FileName = startupPath + "/BayesianNetwork";
            //startInfo.WindowStyle = ProcessWindowStyle.Hidden;
            this.resultado.Text = startupPath;

            try
            {
                // Start the process with the info we specified.
                // Call WaitForExit and then the using statement will close.
                using (Process exeProcess = Process.Start(startInfo))
                {
                    exeProcess.WaitForExit();
                    if (exeProcess.ExitCode == 0)
                    {
                        MessageBox.Show("exit code 0");
                    }
                    else
                    {
                        MessageBox.Show("exit code " + exeProcess.ExitCode.ToString());
                    }
                }
            }
            catch
            {
                // Log error.
            }
        }
    }
}
