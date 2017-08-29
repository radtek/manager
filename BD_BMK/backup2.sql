USE `bmkperu`;

delete from familia;
delete from comprobante;
ALTER TABLE comprobante AUTO_INCREMENT = 1;
delete from usuario;
delete from persona;
delete from grupo;
delete from item;
delete from direccion;
delete from plato;


INSERT INTO `datos` VALUES (398,921,582,2,8,7,3,'ALASPERUANAS');

INSERT INTO `grupo` VALUES ('comprobante_operacion'),('comprobante_tipo'),('establecimiento'),('estado'),('familia'),('marca'),('moneda'),('operacion'),('pago'),('persona_tipo'),('producto_tipo'),('regimen'),('unidad');

INSERT INTO `item` VALUES ('Compra','comprobante_operacion'),('Venta','comprobante_operacion'),('Boleta','comprobante_tipo'),('Factura','comprobante_tipo'),('Ticket','comprobante_tipo'),('Casa','Establecimiento'),('Principal','Establecimiento'),('Sucursal','Establecimiento'),('Activo','Estado'),('Suspendido','Estado'),('Agua San Luis 1 LT','familia'),('Alitas Picantes','familia'),('Alitas SBBQ','familia'),('Almuerzos','familia'),('Arroz','familia'),('Arroz a la Cubana','familia'),('Arroz a la Cubana S/.9','familia'),('Bebidas','familia'),('Cafe','familia'),('Cafe / Leche','familia'),('Cafeteria','familia'),('Chicha Jarra','familia'),('Chicha Morada Grande','familia'),('Chicha Morada Regular','familia'),('Chicha S/0','familia'),('Chicha S/1','familia'),('Chocolate','familia'),('Choripan','familia'),('Choripan / Bebida','familia'),('Chuleta de Cerdo Junior','familia'),('Chuleta de Cerdo S/.9','familia'),('Cocacola 1.5','familia'),('Cocacola 1.5 familiar','familia'),('Cocacola 2.5 Familiar Promo','familia'),('Cocacola 500 ml','familia'),('Cocacola Prom S/1','familia'),('Combo Bill','familia'),('Combo Bill Personal','familia'),('Combo Delivery','familia'),('Combo Nuggets','familia'),('Combo Para 2','familia'),('Combo Para 2 Delivery','familia'),('Combo Personal 1','familia'),('Combo Personal Bill','familia'),('Combo Picante','familia'),('Combos','familia'),('Comida Rapida','familia'),('Cono Bill Chocolate','familia'),('Cono Bill Combinado','familia'),('Cono Bill Vainilla','familia'),('Cono Nugget + Chicha','familia'),('Cono Nugget + Maracuya','familia'),('Cono Nuggets','familia'),('Crepe Jamon y Queso','familia'),('Crepe Platano','familia'),('Delivery S/.2','familia'),('Delivery S/.4','familia'),('Delivery S/.5','familia'),('Delivery S/.6','familia'),('Delivery S/.7','familia'),('Delivery S/.8','familia'),('Delivery S/3','familia'),('Ensalada Cesar','familia'),('Ensalada Cesar S/.9','familia'),('Ensalada de frutas','familia'),('Ensalada Regular','familia'),('Familiar 1','familia'),('Familiar 2','familia'),('Familiar 3','familia'),('Filete de Pollo','familia'),('Filete de Pollo Grill ','familia'),('Filete de Pollo Grill S/.9','familia'),('Frap de Fresa','familia'),('Frap de Lucuma','familia'),('Frap de Moka','familia'),('Frap de Oreo','familia'),('Frap de Vainilla','familia'),('Frozen de limon','familia'),('Helados','familia'),('Huevo','familia'),('Inca Kola 1.5 Familiar','familia'),('Inca Kola 1.5 L','familia'),('Inca Kola 2.5 Familiar Promo','familia'),('Inca Kola 500 ml','familia'),('Inka Prom S/1','familia'),('Jugo Grande de Papaya','familia'),('Jugo Grande de PiÃ±a','familia'),('Jugo Grande de Platano','familia'),('Jugo Grande Surtido','familia'),('Jugo Regular Papaya','familia'),('Jugo Regular PiÃ±a','familia'),('Jugo Regular Surtido','familia'),('Leche','familia'),('Maracuya Grande','familia'),('Maracuya Regular','familia'),('Maracuya S/0','familia'),('Maracuya S/1','familia'),('Milanesa de pollo','familia'),('Milanesa S/.9','familia'),('Paneton','familia'),('Paneton / Chocolate','familia'),('Pollo Broaster','familia'),('Pollo Broaster S/.9','familia'),('Porcion de arroz','familia'),('Postres','familia'),('Promo 2 X 10 00','familia'),('Promo 2 X 10 0F','familia'),('Promo 2 X 10 0M','familia'),('Promo 2 X 10 FM','familia'),('Promo 2 X 10 L0','familia'),('Promo 2 X 10 LF','familia'),('Promo 2 X 10 LL','familia'),('Promo 2 X 10 LM','familia'),('Promo 2 X 10 LV','familia'),('Promo 2 X 10 M0','familia'),('Promo 2 X 10 MF','familia'),('Promo 2 X 10 MM','familia'),('Promo 2 X 10 MV','familia'),('Promo 2 X 10 VV','familia'),('Promo 3 Cono Bill x 5','familia'),('Salchi arroz','familia'),('Salchi arroz S/.9','familia'),('Salchi Pollo Bill','familia'),('Salchicono + Chicha','familia'),('Salchicono + Maracuya','familia'),('Salchicono Bill','familia'),('Salchipapa Bill','familia'),('Salchipapa Familiar','familia'),('Salchipapa Familiar Delivery','familia'),('Sanwich de Filete de Pollo','familia'),('Sundae 3 x 2','familia'),('Sundae Cua Cua','familia'),('Sundae de Chocolate','familia'),('Sundae de Fresa','familia'),('Sundae DoÃ±a Pepa','familia'),('Taper','familia'),('Te / Mate','familia'),('General','Marca'),('Dolar','Moneda'),('Euro','Moneda'),('Sol','Moneda'),('Efectivo','Pago'),('Master Card','Pago'),('Visa','Pago'),('Administrador','persona_tipo'),('Cliente_DNI','persona_tipo'),('Cliente_RUC','persona_tipo'),('Empleado','persona_tipo'),('Proveedor','persona_tipo'),('Servicio','persona_tipo'),('Transportista','persona_tipo'),('Producto_Final','producto_tipo'),('Detraccion','Regimen'),('Percepcion','Regimen'),('Retencion','Regimen'),('ONZ','Unidad');

INSERT INTO `familia` VALUES ('Almuerzos',1,4,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#00007f','#ffff7f'),('Bebidas',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff7f','#0000ff'),('Cafeteria',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#934900','#55aa00'),('Combos',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ff0000','#000000'),('Comida Rapida',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#aa0000','#aaff00'),('Helados',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffffff','#55ffff'),('Postres',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ff55ff',NULL);


INSERT INTO `plato` VALUES ('Agua San Luis 1 LT','Bebidas','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2,NULL),('Alitas Picantes','Comida Rapida','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff00','#c7c7c7',5.9,NULL),('Alitas SBBQ','Comida Rapida','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff00','#bcbcbc',8.9,NULL),('Arroz','Almuerzos','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3,NULL),('Arroz a la Cubana','Almuerzos','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9,NULL),('Arroz a la Cubana S/.9','Almuerzos','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9,NULL),('Cafe','Cafeteria','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3.5,NULL),('Cafe / Leche','Cafeteria','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4.5,NULL),('Chicha Jarra','Bebidas','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Chicha Morada Grande','Bebidas','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3.5,NULL),('Chicha Morada Regular','Bebidas','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2,NULL),('Chicha S/0','Bebidas','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Chicha S/1','Bebidas','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1,NULL),('Chocolate','Cafeteria','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4,NULL),('Choripan','Comida Rapida','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#aa0000',NULL,3.6,NULL),('Choripan / Bebida','Comida Rapida','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#aa0000','#d4d4d4',5,NULL),('Chuleta de Cerdo Junior','Almuerzos','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6,NULL),('Chuleta de Cerdo S/.9','Almuerzos','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9,NULL),('Cocacola 1.5','Bebidas','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1.5,NULL),('Cocacola 1.5 familiar','Bebidas','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Cocacola 2.5 Familiar Promo','Bebidas','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Cocacola 500 ml','Bebidas','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3,NULL),('Cocacola Prom S/1','Bebidas','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1,NULL),('Combo Bill','Postres','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Combo Bill Personal','Combos','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,10,NULL),('Combo Delivery','Combos','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,49,NULL),('Combo Nuggets','Combos','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,37,NULL),('Combo Para 2','Combos','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,11.9,NULL),('Combo Para 2 Delivery','Combos','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,14.9,NULL),('Combo Personal 1','Combos','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,21.9,NULL),('Combo Personal Bill','Combos','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,12,NULL),('Combo Picante','Combos','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,16,NULL),('Cono Bill Chocolate','Helados','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Cono Bill Combinado','Helados','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Cono Bill Vainilla','Helados','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Cono Nugget + Chicha','Comida Rapida','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff7f','#55007f',5.8,NULL),('Cono Nugget + Maracuya','Comida Rapida','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff7f','#595959',5.8,NULL),('Cono Nuggets','Comida Rapida','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff7f','#8f8f8f',4.5,NULL),('Crepe Jamon y Queso','Cafeteria','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Crepe Platano','Cafeteria','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Delivery S/.2','Combos','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2,NULL),('Delivery S/.4','Combos','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4,NULL),('Delivery S/.5','Combos','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Delivery S/.6','Combos','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6,NULL),('Delivery S/.7','Combos','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7,NULL),('Delivery S/.8','Combos','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,8,NULL),('Delivery S/3','Combos','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3,NULL),('Ensalada Cesar','Almuerzos','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6,NULL),('Ensalada Cesar S/.9','Almuerzos','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9,NULL),('Ensalada de frutas','Cafeteria','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Ensalada Regular','Comida Rapida','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#aaff00','#efefef',3.5,NULL),('Familiar 1','Combos','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Familiar 2','Combos','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Familiar 3','Combos','General','ONZ',6,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Filete de Pollo','Comida Rapida','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,'#ffffff',6.7,NULL),('Filete de Pollo Grill ','Almuerzos','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9,NULL),('Filete de Pollo Grill S/.9','Almuerzos','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9,NULL),('Frap de Fresa','Postres','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Frap de Lucuma','Postres','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Frap de Moka','Postres','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Frap de Oreo','Postres','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Frap de Vainilla','Postres','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Frozen de limon','Postres','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Huevo','Almuerzos','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1,NULL),('Inca Kola 1.5 Familiar','Bebidas','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Inca Kola 1.5 L','Bebidas','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1,NULL),('Inca Kola 2.5 Familiar Promo','Bebidas','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Inca Kola 500 ml','Bebidas','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3,NULL),('Inka Prom S/1','Bebidas','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1,NULL),('Jugo Grande de Papaya','Cafeteria','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Jugo Grande de PiÃ±a','Cafeteria','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Jugo Grande de Platano','Cafeteria','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Jugo Grande Surtido','Cafeteria','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Jugo Regular Papaya','Cafeteria','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Jugo Regular PiÃ±a','Cafeteria','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Jugo Regular Surtido','Cafeteria','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Leche','Cafeteria','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2.5,NULL),('Maracuya Grande','Bebidas','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3.5,NULL),('Maracuya Regular','Bebidas','General','ONZ',6,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2,NULL),('Maracuya S/0','Bebidas','General','ONZ',7,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0,NULL),('Maracuya S/1','Bebidas','General','ONZ',7,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1,NULL),('Milanesa de pollo','Almuerzos','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9,NULL),('Milanesa S/.9','Almuerzos','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9,NULL),('Paneton','Cafeteria','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3.5,NULL),('Paneton / Chocolate','Cafeteria','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.5,NULL),('Pollo Broaster','Almuerzos','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9,NULL),('Pollo Broaster S/.9','Almuerzos','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9,NULL),('Porcion de arroz','Almuerzos','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3,NULL),('Promo 2 X 10 00','Postres','General','ONZ',7,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 0F','Postres','General','ONZ',6,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 0M','Postres','General','ONZ',7,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 FM','Postres','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 L0','Postres','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 LF','Postres','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 LL','Postres','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 LM','Postres','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 LV','Postres','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 M0','Postres','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 MF','Postres','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 MM','Postres','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 MV','Postres','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 2 X 10 VV','Postres','General','ONZ',7,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Promo 3 Cono Bill x 5','Helados','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Salchi arroz','Almuerzos','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9,NULL),('Salchi arroz S/.9','Almuerzos','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9,NULL),('Salchi Pollo Bill','Comida Rapida','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6.7,NULL),('Salchicono + Chicha','Comida Rapida','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4.9,NULL),('Salchicono + Maracuya','Comida Rapida','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4.9,NULL),('Salchicono Bill','Comida Rapida','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#000000',NULL,3.5,NULL),('Salchipapa Bill','Comida Rapida','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4.5,NULL),('Salchipapa Familiar','Comida Rapida','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,13.9,NULL),('Salchipapa Familiar Delivery','Comida Rapida','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,18.9,NULL),('Sanwich de Filete de Pollo','Comida Rapida','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6.3,NULL),('Sundae 3 x 2','Helados','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Sundae Cua Cua','Helados','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Sundae de Chocolate','Helados','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Sundae de Fresa','Helados','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Sundae DoÃ±a Pepa','Helados','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5,NULL),('Taper','Almuerzos','General','ONZ',6,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0.5,NULL),('Te / Mate','Cafeteria','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2,NULL);

INSERT INTO `usuario` VALUES ('alex','1234'),('mili','123456');

INSERT INTO `persona` VALUES ('04600000','Milagros','Mili','2000-01-01','000000000','000000000','2000-01-01','2000-01-01','Activo','mili','Empleado',NULL),('71200000','Alexander Gabriel, Luna Choquecota','alex','2000-01-01','053483120','953627684','2000-01-01','2000-01-01','Activo','alex','Administrador',NULL),('71200685','ALEXANDER GABRIEL LUNA CHOQUECOTA',NULL,NULL,NULL,NULL,NULL,NULL,'Activo',NULL,'Cliente_DNI','');

INSERT INTO `comprobante` VALUES (1,'','0000',0,'2017-08-04 22:49:14',1,'Efectivo','Activo','Venta','Ticket'),(2,'Alex','0000',0,'2017-08-04 22:54:55',1,'Efectivo','Activo','Venta','Ticket'),(3,'ALEXANDER GABRIEL LUNA CHOQUECOTA','0000',0,'2017-08-04 22:55:39',1,'Efectivo','Activo','Venta','Ticket'),(4,'cesar','0000',0,'2017-08-04 23:11:16',1,'Efectivo','Activo','Venta','Ticket'),(5,'X','0000',0,'2017-08-09 11:33:36',1,'Efectivo','Activo','Venta','Ticket'),(6,'MILAGROS','0000',0,'2017-08-09 11:37:26',1,'Efectivo','Activo','Venta','Ticket'),(7,'ALEX','0000',0,'2017-08-09 11:45:37',1,'Efectivo','Activo','Venta','Ticket'),(8,'cabb','0000',0,'2017-08-10 11:17:10',1,'Efectivo','Activo','Venta','Ticket'),(9,'alex','0000',0,'2017-08-10 11:19:28',1,'Efectivo','Activo','Venta','Ticket'),(10,'adssad','0000',0,'2017-08-10 11:22:58',1,'Efectivo','Activo','Venta','Ticket'),(11,'vitor','0000',0,'2017-08-10 11:25:35',1,'Efectivo','Activo','Venta','Ticket'),(12,'mili','0000',0,'2017-08-10 11:27:03',1,'Efectivo','Activo','Venta','Ticket'),(13,'alex','0000',0,'2017-08-10 11:29:05',1,'Efectivo','Activo','Venta','Ticket'),(14,'vbb','0000',0,'2017-08-10 11:36:11',1,'Efectivo','Activo','Venta','Ticket'),(15,'cacg','0000',0,'2017-08-10 11:51:06',1,'Efectivo','Activo','Venta','Ticket'),(16,'cacac','0000',0,'2017-08-10 11:57:50',1,'Efectivo','Activo','Venta','Ticket'),(17,'ageqr','0000',0,'2017-08-10 11:59:37',1,'Efectivo','Activo','Venta','Ticket'),(18,'casc','0000',0,'2017-08-10 12:03:42',1,'Efectivo','Activo','Venta','Ticket'),(19,'cascsa','0000',0,'2017-08-10 12:04:35',1,'Efectivo','Activo','Venta','Ticket'),(20,'','0000',0,'2017-08-10 12:05:55',1,'Efectivo','Activo','Venta','Ticket'),(21,'','0000',0,'2017-08-10 12:08:11',1,'Efectivo','Activo','Venta','Ticket'),(22,'','0000',0,'2017-08-10 12:16:57',1,'Efectivo','Activo','Venta','Ticket'),(23,'','0000',0,'2017-08-10 12:19:16',1,'Efectivo','Activo','Venta','Ticket'),(24,'','0000',0,'2017-08-10 12:20:41',1,'Efectivo','Activo','Venta','Ticket'),(25,'','0000',0,'2017-08-10 12:29:16',1,'Efectivo','Activo','Venta','Ticket'),(26,'','0000',0,'2017-08-10 12:53:40',1,'Efectivo','Activo','Venta','Ticket'),(27,'','0000',0,'2017-08-10 13:00:53',1,'Efectivo','Activo','Venta','Ticket'),(28,'','0000',0,'2017-08-10 13:07:02',1,'Efectivo','Activo','Venta','Ticket'),(29,'','0000',0,'2017-08-10 13:08:43',1,'Efectivo','Activo','Venta','Ticket'),(30,'','0000',0,'2017-08-10 13:10:04',1,'Efectivo','Activo','Venta','Ticket'),(31,'','0000',0,'2017-08-10 13:12:14',1,'Efectivo','Activo','Venta','Ticket'),(32,'','0000',0,'2017-08-10 13:14:58',1,'Efectivo','Activo','Venta','Ticket'),(33,'','0000',0,'2017-08-10 13:16:29',1,'Efectivo','Activo','Venta','Ticket'),(34,'','0000',0,'2017-08-10 13:17:45',1,'Efectivo','Activo','Venta','Ticket'),(35,'JAN CA','0000',0,'2017-08-10 14:19:38',1,'Efectivo','Activo','Venta','Ticket'),(36,'asvsasa','0000',0,'2017-08-10 14:29:46',1,'Efectivo','Activo','Venta','Ticket'),(37,'asddsa','0000',0,'2017-08-10 14:35:35',1,'Efectivo','Activo','Venta','Ticket'),(38,'qfqewq','0000',0,'2017-08-10 14:37:56',1,'Efectivo','Activo','Venta','Ticket'),(39,'VAAS','0000',0,'2017-08-10 14:44:36',1,'Efectivo','Activo','Venta','Ticket'),(40,'bassas','0000',0,'2017-08-10 14:46:40',1,'Efectivo','Activo','Venta','Ticket'),(41,'bassas','0000',0,'2017-08-10 14:48:26',1,'Efectivo','Activo','Venta','Ticket'),(42,'asdsad','0000',0,'2017-08-10 14:49:38',1,'Efectivo','Activo','Venta','Ticket'),(43,'gqwgwq','0000',0,'2017-08-10 14:51:21',1,'Efectivo','Activo','Venta','Ticket'),(44,'','0000',0,'2017-08-10 14:53:07',1,'Efectivo','Activo','Venta','Ticket'),(45,'JAN CA','0000',0,'2017-08-10 14:55:49',1,'Efectivo','Activo','Venta','Ticket'),(46,'basba','0000',0,'2017-08-10 15:00:34',1,'Efectivo','Activo','Venta','Ticket'),(47,'vasvsasa','0000',0,'2017-08-10 15:04:04',1,'Efectivo','Activo','Venta','Ticket'),(48,'','0000',0,'2017-08-10 15:15:16',1,'Efectivo','Activo','Venta','Ticket'),(49,'assafsaf','0000',0,'2017-08-10 16:12:56',1,'Efectivo','Activo','Venta','Ticket'),(50,'','0000',0,'2017-08-10 16:16:46',1,'Efectivo','Activo','Venta','Ticket'),(51,'','0000',0,'2017-08-10 16:17:24',1,'Efectivo','Activo','Venta','Ticket'),(52,'asdasd','0000',0,'2017-08-10 16:32:31',1,'Efectivo','Activo','Venta','Ticket'),(53,'','0000',0,'2017-08-10 16:34:06',1,'Efectivo','Activo','Venta','Ticket'),(54,'','0000',0,'2017-08-10 16:35:01',1,'Efectivo','Activo','Venta','Ticket'),(55,'','0000',0,'2017-08-10 16:48:00',1,'Efectivo','Activo','Venta','Ticket'),(56,'','0000',0,'2017-08-10 16:51:45',1,'Efectivo','Activo','Venta','Ticket'),(57,'','0000',0,'2017-08-10 16:58:48',1,'Efectivo','Activo','Venta','Ticket'),(58,'','0000',0,'2017-08-10 17:05:49',1,'Efectivo','Activo','Venta','Ticket'),(59,'','0000',0,'2017-08-10 17:13:01',1,'Efectivo','Activo','Venta','Ticket'),(60,'','0000',0,'2017-08-10 17:15:05',1,'Efectivo','Activo','Venta','Ticket'),(61,'','0000',0,'2017-08-10 17:18:01',1,'Efectivo','Activo','Venta','Ticket'),(62,'','0000',0,'2017-08-10 17:21:05',1,'Efectivo','Activo','Venta','Ticket'),(63,'','0000',0,'2017-08-10 17:23:49',1,'Efectivo','Activo','Venta','Ticket'),(64,'','0000',0,'2017-08-10 17:26:28',1,'Efectivo','Activo','Venta','Ticket'),(65,'','0000',0,'2017-08-10 17:30:05',1,'Efectivo','Activo','Venta','Ticket'),(66,'','0000',0,'2017-08-10 17:42:40',1,'Efectivo','Activo','Venta','Ticket'),(67,'','0000',0,'2017-08-10 17:44:39',1,'Efectivo','Activo','Venta','Ticket'),(68,'','0000',0,'2017-08-10 17:50:20',1,'Efectivo','Activo','Venta','Ticket'),(69,'','0000',0,'2017-08-10 17:52:27',1,'Efectivo','Activo','Venta','Ticket'),(70,'','0000',0,'2017-08-10 17:54:35',1,'Efectivo','Activo','Venta','Ticket'),(71,'','0000',0,'2017-08-10 17:56:19',1,'Efectivo','Activo','Venta','Ticket'),(72,'','0000',0,'2017-08-10 18:05:24',1,'Efectivo','Activo','Venta','Ticket'),(73,'','0000',0,'2017-08-10 18:09:00',1,'Efectivo','Activo','Venta','Ticket'),(74,'','0000',0,'2017-08-10 19:26:26',1,'Efectivo','Activo','Venta','Ticket'),(75,'','0000',0,'2017-08-10 19:31:35',1,'Efectivo','Activo','Venta','Ticket'),(76,'','0000',0,'2017-08-10 19:32:13',1,'Efectivo','Activo','Venta','Ticket'),(77,'csaassa','0000',0,'2017-08-10 19:38:45',1,'Efectivo','Activo','Venta','Ticket'),(78,'ALEX','0000',0,'2017-08-19 17:45:05',1,'Efectivo','Activo','Venta','Ticket'),(79,'BBB','0000',0,'2017-08-19 17:49:33',1,'Master Card','Activo','Venta','Ticket'),(80,'','0000',0,'2017-08-19 17:50:18',1,'Visa','Activo','Venta','Ticket');

INSERT INTO `comprobante_has_persona` VALUES (3,'71200685');

INSERT INTO `comprobante_has_plato` VALUES (1,'Arroz a la Cubana',1),(1,'Chuleta de Cerdo Junior',1),(1,'Taper',1),(2,'Agua San Luis 1 LT',1),(2,'Combo Bill',1),(3,'Alitas Picantes',1),(3,'Frap de Vainilla',1),(4,'Salchicono Bill',1),(5,'Alitas Picantes',1),(5,'Cono Nuggets',1),(6,'Alitas SBBQ',1),(6,'Agua San Luis 1 LT',1),(6,'Frap de Oreo',1),(7,'Salchicono + Maracuya',1),(7,'Sundae Cua Cua',1),(8,'Alitas SBBQ',1),(9,'Agua San Luis 1 LT',1),(9,'Chicha Morada Regular',1),(10,'Chicha Morada Regular',1),(10,'Agua San Luis 1 LT',1),(11,'Choripan / Bebida',1),(11,'Cono Nuggets',1),(12,'Alitas Picantes',1),(12,'Choripan / Bebida',1),(13,'Alitas Picantes',1),(13,'Choripan / Bebida',1),(14,'Chuleta de Cerdo S/.9',1),(14,'Ensalada Cesar S/.9',1),(15,'Combo Personal Bill',1),(15,'Combo Para 2',1),(16,'Salchicono Bill',1),(17,'Agua San Luis 1 LT',1),(17,'Chicha Morada Regular',1),(18,'Agua San Luis 1 LT',1),(18,'Chicha Morada Regular',1),(19,'Cono Nuggets',1),(20,'Chuleta de Cerdo S/.9',1),(20,'Arroz',1),(21,'Chuleta de Cerdo S/.9',1),(21,'Ensalada Cesar S/.9',1),(22,'Choripan / Bebida',1),(22,'Cono Nuggets',1),(23,'Cono Nugget + Maracuya',1),(23,'Alitas SBBQ',1),(24,'Chuleta de Cerdo S/.9',1),(24,'Ensalada Cesar S/.9',1),(25,'Cocacola 2.5 Familiar Promo',1),(25,'Cocacola 1.5 familiar',1),(26,'Agua San Luis 1 LT',1),(27,'Frozen de limon',1),(27,'Promo 2 X 10 LL',1),(28,'Chuleta de Cerdo S/.9',1),(28,'Ensalada Cesar S/.9',1),(29,'Promo 2 X 10 LL',1),(29,'Promo 2 X 10 LV',1),(30,'Promo 2 X 10 M0',1),(31,'Combo Para 2',1),(31,'Combo Personal Bill',1),(32,'Alitas Picantes',1),(32,'Alitas SBBQ',1),(33,'Choripan / Bebida',1),(33,'Cono Nuggets',1),(34,'Chuleta de Cerdo S/.9',1),(34,'Ensalada Cesar S/.9',1),(35,'Salchipapa Familiar',1),(36,'Filete de Pollo Grill ',1),(37,'Delivery S/.2',1),(37,'Delivery S/.6',1),(38,'Chuleta de Cerdo S/.9',1),(39,'Agua San Luis 1 LT',1),(39,'Chicha Morada Regular',1),(40,'Alitas Picantes',1),(41,'Choripan / Bebida',1),(41,'Salchicono Bill',1),(41,'Salchipapa Familiar',1),(42,'Chuleta de Cerdo Junior',1),(42,'Filete de Pollo Grill ',1),(43,'Chuleta de Cerdo S/.9',1),(43,'Ensalada Cesar S/.9',1),(44,'Arroz a la Cubana',1),(44,'Chuleta de Cerdo Junior',1),(45,'Cono Nuggets',1),(45,'Salchicono Bill',1),(46,'Choripan / Bebida',1),(46,'Alitas Picantes',1),(47,'Combo Bill Personal',1),(47,'Combo Personal Bill',1),(47,'Chuleta de Cerdo S/.9',1),(47,'Chuleta de Cerdo Junior',1),(48,'Combo Para 2',1),(48,'Combo Bill Personal',1),(49,'Arroz a la Cubana',1),(49,'Chuleta de Cerdo Junior',1),(50,'Chuleta de Cerdo S/.9',1),(50,'Ensalada Cesar S/.9',1),(51,'Chuleta de Cerdo S/.9',1),(51,'Ensalada Cesar S/.9',1),(52,'Arroz',1),(52,'Chuleta de Cerdo S/.9',1),(53,'Chuleta de Cerdo S/.9',1),(54,'Chuleta de Cerdo Junior',1),(54,'Arroz a la Cubana',1),(55,'Chuleta de Cerdo S/.9',1),(55,'Ensalada Cesar S/.9',1),(56,'Chuleta de Cerdo S/.9',1),(56,'Ensalada Cesar S/.9',1),(57,'Chuleta de Cerdo S/.9',1),(57,'Ensalada Cesar S/.9',1),(58,'Sundae de Chocolate',1),(58,'Promo 3 Cono Bill x 5',1),(59,'Arroz',1),(59,'Chuleta de Cerdo S/.9',1),(59,'Ensalada Cesar S/.9',1),(60,'Chuleta de Cerdo S/.9',1),(60,'Ensalada Cesar S/.9',1),(61,'Arroz',1),(61,'Chuleta de Cerdo S/.9',1),(62,'Combo Para 2 Delivery',1),(63,'Chuleta de Cerdo S/.9',1),(64,'Promo 3 Cono Bill x 5',1),(65,'Arroz',1),(66,'Combo Para 2',1),(67,'Ensalada Cesar S/.9',1),(68,'Cono Bill Chocolate',1),(69,'Combo Para 2',1),(70,'Arroz',1),(71,'Agua San Luis 1 LT',1),(72,'Arroz',1),(73,'Frap de Oreo',1),(74,'Combo Bill Personal',1),(75,'Pollo Broaster',1),(76,'Chuleta de Cerdo S/.9',1),(76,'Ensalada Cesar S/.9',1),(76,'Huevo',1),(77,'Delivery S/.2',1),(77,'Delivery S/.6',1),(78,'Alitas Picantes',1),(79,'Choripan / Bebida',1),(80,'Alitas SBBQ',2),(80,'Alitas SBBQ',1);
