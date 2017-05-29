using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Restaurante
{
    public partial class Home : Form
    {
        Compra compra;
        public Home()
        {
            InitializeComponent();
        }

        private void Home_Load(object sender, EventArgs e)
        {

        }

        private void button_insumos_Click(object sender, EventArgs e)
        {
            
        }

        private void button_comprar_Click(object sender, EventArgs e)
        {
            if (compra == null) { 
                compra = new Compra();
                compra.FormClosed += compra_FormClosed;
            }
            compra.Show(this);
            Hide();
        }
        void compra_FormClosed(object sender, FormClosedEventArgs e)
        {
            compra = null;
            Show();
        }
    }
}
