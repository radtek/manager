namespace Restaurante
{
    partial class Home
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Home));
            this.button_insumos = new System.Windows.Forms.Button();
            this.button_vender = new System.Windows.Forms.Button();
            this.label_comprar = new System.Windows.Forms.Label();
            this.label_vender = new System.Windows.Forms.Label();
            this.button_comprar = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.button_platillos = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // button_insumos
            // 
            this.button_insumos.Image = ((System.Drawing.Image)(resources.GetObject("button_insumos.Image")));
            this.button_insumos.Location = new System.Drawing.Point(286, 24);
            this.button_insumos.Name = "button_insumos";
            this.button_insumos.Size = new System.Drawing.Size(90, 86);
            this.button_insumos.TabIndex = 2;
            this.button_insumos.UseVisualStyleBackColor = true;
            this.button_insumos.Click += new System.EventHandler(this.button_insumos_Click);
            // 
            // button_vender
            // 
            this.button_vender.Image = ((System.Drawing.Image)(resources.GetObject("button_vender.Image")));
            this.button_vender.Location = new System.Drawing.Point(428, 6);
            this.button_vender.Name = "button_vender";
            this.button_vender.Size = new System.Drawing.Size(226, 130);
            this.button_vender.TabIndex = 0;
            this.button_vender.UseVisualStyleBackColor = true;
            // 
            // label_comprar
            // 
            this.label_comprar.AutoSize = true;
            this.label_comprar.Location = new System.Drawing.Point(132, 139);
            this.label_comprar.Name = "label_comprar";
            this.label_comprar.Size = new System.Drawing.Size(46, 13);
            this.label_comprar.TabIndex = 3;
            this.label_comprar.Text = "Comprar";
            // 
            // label_vender
            // 
            this.label_vender.AutoSize = true;
            this.label_vender.Location = new System.Drawing.Point(513, 139);
            this.label_vender.Name = "label_vender";
            this.label_vender.Size = new System.Drawing.Size(41, 13);
            this.label_vender.TabIndex = 4;
            this.label_vender.Text = "Vender";
            // 
            // button_comprar
            // 
            this.button_comprar.Image = ((System.Drawing.Image)(resources.GetObject("button_comprar.Image")));
            this.button_comprar.Location = new System.Drawing.Point(87, 6);
            this.button_comprar.Name = "button_comprar";
            this.button_comprar.Size = new System.Drawing.Size(143, 130);
            this.button_comprar.TabIndex = 5;
            this.button_comprar.UseVisualStyleBackColor = true;
            this.button_comprar.Click += new System.EventHandler(this.button_comprar_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(304, 113);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(46, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Insumos";
            // 
            // button_platillos
            // 
            this.button_platillos.Image = ((System.Drawing.Image)(resources.GetObject("button_platillos.Image")));
            this.button_platillos.Location = new System.Drawing.Point(194, 148);
            this.button_platillos.Name = "button_platillos";
            this.button_platillos.Size = new System.Drawing.Size(313, 213);
            this.button_platillos.TabIndex = 7;
            this.button_platillos.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(304, 364);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Carta de comidas";
            // 
            // Home
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(681, 384);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.button_platillos);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button_comprar);
            this.Controls.Add(this.label_vender);
            this.Controls.Add(this.label_comprar);
            this.Controls.Add(this.button_insumos);
            this.Controls.Add(this.button_vender);
            this.Name = "Home";
            this.Text = "Home";
            this.Load += new System.EventHandler(this.Home_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_vender;
        private System.Windows.Forms.Button button_insumos;
        private System.Windows.Forms.Label label_comprar;
        private System.Windows.Forms.Label label_vender;
        private System.Windows.Forms.Button button_comprar;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button_platillos;
        private System.Windows.Forms.Label label2;
    }
}