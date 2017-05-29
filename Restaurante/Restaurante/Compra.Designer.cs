namespace Restaurante
{
    partial class Compra
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
            this.tableLayoutPanel_resultado = new System.Windows.Forms.TableLayoutPanel();
            this.button_ingresar = new System.Windows.Forms.Button();
            this.dateTimePicker1 = new System.Windows.Forms.DateTimePicker();
            this.label1 = new System.Windows.Forms.Label();
            this.button_arriba = new System.Windows.Forms.Button();
            this.button_abajo = new System.Windows.Forms.Button();
            this.button_borrar = new System.Windows.Forms.Button();
            this.button_guardar = new System.Windows.Forms.Button();
            this.button_eliminar = new System.Windows.Forms.Button();
            this.button_salir = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // tableLayoutPanel_resultado
            // 
            this.tableLayoutPanel_resultado.AutoScroll = true;
            this.tableLayoutPanel_resultado.ColumnCount = 1;
            this.tableLayoutPanel_resultado.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_resultado.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel_resultado.Location = new System.Drawing.Point(25, 165);
            this.tableLayoutPanel_resultado.Name = "tableLayoutPanel_resultado";
            this.tableLayoutPanel_resultado.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.tableLayoutPanel_resultado.RowCount = 1;
            this.tableLayoutPanel_resultado.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 500F));
            this.tableLayoutPanel_resultado.Size = new System.Drawing.Size(656, 244);
            this.tableLayoutPanel_resultado.TabIndex = 0;
            // 
            // button_ingresar
            // 
            this.button_ingresar.Location = new System.Drawing.Point(25, 121);
            this.button_ingresar.Name = "button_ingresar";
            this.button_ingresar.Size = new System.Drawing.Size(75, 23);
            this.button_ingresar.TabIndex = 1;
            this.button_ingresar.Text = "INGRESAR";
            this.button_ingresar.UseVisualStyleBackColor = true;
            // 
            // dateTimePicker1
            // 
            this.dateTimePicker1.CustomFormat = "dddd,  MM - yyyy";
            this.dateTimePicker1.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePicker1.Location = new System.Drawing.Point(77, 69);
            this.dateTimePicker1.Name = "dateTimePicker1";
            this.dateTimePicker1.Size = new System.Drawing.Size(152, 20);
            this.dateTimePicker1.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(22, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(139, 31);
            this.label1.TabIndex = 3;
            this.label1.Text = "COMPRA";
            // 
            // button_arriba
            // 
            this.button_arriba.Location = new System.Drawing.Point(444, 121);
            this.button_arriba.Name = "button_arriba";
            this.button_arriba.Size = new System.Drawing.Size(75, 23);
            this.button_arriba.TabIndex = 4;
            this.button_arriba.Text = "Arriba";
            this.button_arriba.UseVisualStyleBackColor = true;
            // 
            // button_abajo
            // 
            this.button_abajo.Location = new System.Drawing.Point(525, 121);
            this.button_abajo.Name = "button_abajo";
            this.button_abajo.Size = new System.Drawing.Size(75, 23);
            this.button_abajo.TabIndex = 5;
            this.button_abajo.Text = "Abajo";
            this.button_abajo.UseVisualStyleBackColor = true;
            // 
            // button_borrar
            // 
            this.button_borrar.Location = new System.Drawing.Point(606, 121);
            this.button_borrar.Name = "button_borrar";
            this.button_borrar.Size = new System.Drawing.Size(75, 23);
            this.button_borrar.TabIndex = 6;
            this.button_borrar.Text = "Borrar";
            this.button_borrar.UseVisualStyleBackColor = true;
            // 
            // button_guardar
            // 
            this.button_guardar.Location = new System.Drawing.Point(281, 430);
            this.button_guardar.Name = "button_guardar";
            this.button_guardar.Size = new System.Drawing.Size(75, 23);
            this.button_guardar.TabIndex = 7;
            this.button_guardar.Text = "Guardar";
            this.button_guardar.UseVisualStyleBackColor = true;
            // 
            // button_eliminar
            // 
            this.button_eliminar.Location = new System.Drawing.Point(372, 430);
            this.button_eliminar.Name = "button_eliminar";
            this.button_eliminar.Size = new System.Drawing.Size(75, 23);
            this.button_eliminar.TabIndex = 8;
            this.button_eliminar.Text = "Eliminar";
            this.button_eliminar.UseVisualStyleBackColor = true;
            // 
            // button_salir
            // 
            this.button_salir.Location = new System.Drawing.Point(525, 430);
            this.button_salir.Name = "button_salir";
            this.button_salir.Size = new System.Drawing.Size(75, 23);
            this.button_salir.TabIndex = 9;
            this.button_salir.Text = "Salir";
            this.button_salir.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(25, 69);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(46, 16);
            this.label2.TabIndex = 10;
            this.label2.Text = "Fecha";
            // 
            // Compra
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(690, 499);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.button_salir);
            this.Controls.Add(this.button_eliminar);
            this.Controls.Add(this.button_guardar);
            this.Controls.Add(this.button_borrar);
            this.Controls.Add(this.button_abajo);
            this.Controls.Add(this.button_arriba);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.dateTimePicker1);
            this.Controls.Add(this.button_ingresar);
            this.Controls.Add(this.tableLayoutPanel_resultado);
            this.Name = "Compra";
            this.Text = "Compra";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel_resultado;
        private System.Windows.Forms.Button button_ingresar;
        private System.Windows.Forms.DateTimePicker dateTimePicker1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button_arriba;
        private System.Windows.Forms.Button button_abajo;
        private System.Windows.Forms.Button button_borrar;
        private System.Windows.Forms.Button button_guardar;
        private System.Windows.Forms.Button button_eliminar;
        private System.Windows.Forms.Button button_salir;
        private System.Windows.Forms.Label label2;
    }
}