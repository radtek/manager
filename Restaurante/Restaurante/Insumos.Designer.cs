namespace Restaurante
{
    partial class Insumos
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanel_buscar = new System.Windows.Forms.TableLayoutPanel();
            this.label_ingredientes = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label_buscar = new System.Windows.Forms.Label();
            this.NUEVO = new System.Windows.Forms.Button();
            this.EDITAR = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // tableLayoutPanel_buscar
            // 
            this.tableLayoutPanel_buscar.AutoScroll = true;
            this.tableLayoutPanel_buscar.ColumnCount = 1;
            this.tableLayoutPanel_buscar.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_buscar.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_buscar.Location = new System.Drawing.Point(16, 60);
            this.tableLayoutPanel_buscar.Name = "tableLayoutPanel_buscar";
            this.tableLayoutPanel_buscar.RowCount = 1;
            this.tableLayoutPanel_buscar.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 200F));
            this.tableLayoutPanel_buscar.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 200F));
            this.tableLayoutPanel_buscar.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 200F));
            this.tableLayoutPanel_buscar.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 200F));
            this.tableLayoutPanel_buscar.Size = new System.Drawing.Size(641, 274);
            this.tableLayoutPanel_buscar.TabIndex = 0;
            // 
            // label_ingredientes
            // 
            this.label_ingredientes.AutoSize = true;
            this.label_ingredientes.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_ingredientes.Location = new System.Drawing.Point(12, 9);
            this.label_ingredientes.Name = "label_ingredientes";
            this.label_ingredientes.Size = new System.Drawing.Size(232, 31);
            this.label_ingredientes.TabIndex = 1;
            this.label_ingredientes.Text = "INGREDIENTES";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(106, 355);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(551, 20);
            this.textBox1.TabIndex = 2;
            // 
            // label_buscar
            // 
            this.label_buscar.AutoSize = true;
            this.label_buscar.Location = new System.Drawing.Point(13, 358);
            this.label_buscar.Name = "label_buscar";
            this.label_buscar.Size = new System.Drawing.Size(51, 13);
            this.label_buscar.TabIndex = 3;
            this.label_buscar.Text = "BUSCAR";
            // 
            // NUEVO
            // 
            this.NUEVO.Location = new System.Drawing.Point(487, 11);
            this.NUEVO.Name = "NUEVO";
            this.NUEVO.Size = new System.Drawing.Size(75, 23);
            this.NUEVO.TabIndex = 4;
            this.NUEVO.Text = "NUEVO";
            this.NUEVO.UseVisualStyleBackColor = true;
            // 
            // EDITAR
            // 
            this.EDITAR.Location = new System.Drawing.Point(582, 12);
            this.EDITAR.Name = "EDITAR";
            this.EDITAR.Size = new System.Drawing.Size(75, 23);
            this.EDITAR.TabIndex = 5;
            this.EDITAR.Text = "EDITAR";
            this.EDITAR.UseVisualStyleBackColor = true;
            // 
            // Insumos
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(669, 387);
            this.Controls.Add(this.EDITAR);
            this.Controls.Add(this.NUEVO);
            this.Controls.Add(this.label_buscar);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label_ingredientes);
            this.Controls.Add(this.tableLayoutPanel_buscar);
            this.Name = "Insumos";
            this.Text = "Insumos";
            this.Load += new System.EventHandler(this.Insumos_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel_buscar;
        private System.Windows.Forms.Label label_ingredientes;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label_buscar;
        private System.Windows.Forms.Button NUEVO;
        private System.Windows.Forms.Button EDITAR;
    }
}