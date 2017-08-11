-- MySQL dump 10.13  Distrib 5.7.19, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: bmkperu
-- ------------------------------------------------------
-- Server version	5.7.19-log
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `bmkperu`
--

USE `bmkperu`;

--
-- Dumping data for table `comprobante`
--

INSERT INTO `comprobante` VALUES (1,'','0000',0,'2017-08-04 22:49:14',1,'Efectivo','Activo','Venta','Ticket'),(2,'Alex','0000',0,'2017-08-04 22:54:55',1,'Efectivo','Activo','Venta','Ticket'),(3,'ALEXANDER GABRIEL LUNA CHOQUECOTA','0000',0,'2017-08-04 22:55:39',1,'Efectivo','Activo','Venta','Ticket'),(4,'cesar','0000',0,'2017-08-04 23:11:16',1,'Efectivo','Activo','Venta','Ticket');

--
-- Dumping data for table `comprobante_has_persona`
--

INSERT INTO `comprobante_has_persona` VALUES (3,'71200685');

--
-- Dumping data for table `comprobante_has_plato`
--

INSERT INTO `comprobante_has_plato` VALUES (1,'Arroz a la Cubana',1),(1,'Chuleta de Cerdo Junior',1),(1,'Taper',1),(2,'Agua San Luis 1 LT',1),(2,'Combo Bill',1),(3,'Alitas Picantes',1),(3,'Frap de Vainilla',1),(4,'Salchicono Bill',1);

--
-- Dumping data for table `datos`
--

INSERT INTO `datos` VALUES (174,591,582,2,8,7,3);

--
-- Dumping data for table `direccion`
--


--
-- Dumping data for table `divisa`
--


--
-- Dumping data for table `familia`
--

INSERT INTO `familia` VALUES ('Almuerzos',1,4,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#00007f','#ffff7f'),('Bebidas',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff7f','#0000ff'),('Cafeteria',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#934900','#55aa00'),('Combos',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ff0000','#000000'),('Comida Rapida',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#aa0000','#aaff00'),('Helados',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffffff','#55ffff'),('Postres',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ff55ff',NULL);

--
-- Dumping data for table `grupo`
--

INSERT INTO `grupo` VALUES ('comprobante_operacion'),('comprobante_tipo'),('establecimiento'),('estado'),('familia'),('marca'),('moneda'),('operacion'),('pago'),('persona_tipo'),('producto_tipo'),('regimen'),('unidad');

--
-- Dumping data for table `impuesto`
--


--
-- Dumping data for table `item`
--

INSERT INTO `item` VALUES ('Compra','comprobante_operacion'),('Venta','comprobante_operacion'),('Boleta','comprobante_tipo'),('Factura','comprobante_tipo'),('Ticket','comprobante_tipo'),('Casa','Establecimiento'),('Principal','Establecimiento'),('Sucursal','Establecimiento'),('Activo','Estado'),('Suspendido','Estado'),('Agua San Luis 1 LT','familia'),('Alitas Picantes','familia'),('Alitas SBBQ','familia'),('Almuerzos','familia'),('Arroz','familia'),('Arroz a la Cubana','familia'),('Arroz a la Cubana S/.9','familia'),('Bebidas','familia'),('Cafe','familia'),('Cafe / Leche','familia'),('Cafeteria','familia'),('Chicha Jarra','familia'),('Chicha Morada Grande','familia'),('Chicha Morada Regular','familia'),('Chicha S/0','familia'),('Chicha S/1','familia'),('Chocolate','familia'),('Choripan','familia'),('Choripan / Bebida','familia'),('Chuleta de Cerdo Junior','familia'),('Chuleta de Cerdo S/.9','familia'),('Cocacola 1.5','familia'),('Cocacola 1.5 familiar','familia'),('Cocacola 2.5 Familiar Promo','familia'),('Cocacola 500 ml','familia'),('Cocacola Prom S/1','familia'),('Combo Bill','familia'),('Combo Bill Personal','familia'),('Combo Delivery','familia'),('Combo Nuggets','familia'),('Combo Para 2','familia'),('Combo Para 2 Delivery','familia'),('Combo Personal 1','familia'),('Combo Personal Bill','familia'),('Combo Picante','familia'),('Combos','familia'),('Comida Rapida','familia'),('Cono Bill Chocolate','familia'),('Cono Bill Combinado','familia'),('Cono Bill Vainilla','familia'),('Cono Nugget + Chicha','familia'),('Cono Nugget + Maracuya','familia'),('Cono Nuggets','familia'),('Crepe Jamon y Queso','familia'),('Crepe Platano','familia'),('Delivery S/.2','familia'),('Delivery S/.4','familia'),('Delivery S/.5','familia'),('Delivery S/.6','familia'),('Delivery S/.7','familia'),('Delivery S/.8','familia'),('Delivery S/3','familia'),('Ensalada Cesar','familia'),('Ensalada Cesar S/.9','familia'),('Ensalada de frutas','familia'),('Ensalada Regular','familia'),('Familiar 1','familia'),('Familiar 2','familia'),('Familiar 3','familia'),('Filete de Pollo','familia'),('Filete de Pollo Grill ','familia'),('Filete de Pollo Grill S/.9','familia'),('Frap de Fresa','familia'),('Frap de Lucuma','familia'),('Frap de Moka','familia'),('Frap de Oreo','familia'),('Frap de Vainilla','familia'),('Frozen de limon','familia'),('Helados','familia'),('Huevo','familia'),('Inca Kola 1.5 Familiar','familia'),('Inca Kola 1.5 L','familia'),('Inca Kola 2.5 Familiar Promo','familia'),('Inca Kola 500 ml','familia'),('Inka Prom S/1','familia'),('Jugo Grande de Papaya','familia'),('Jugo Grande de Piña','familia'),('Jugo Grande de Platano','familia'),('Jugo Grande Surtido','familia'),('Jugo Regular Papaya','familia'),('Jugo Regular Piña','familia'),('Jugo Regular Surtido','familia'),('Leche','familia'),('Maracuya Grande','familia'),('Maracuya Regular','familia'),('Maracuya S/0','familia'),('Maracuya S/1','familia'),('Milanesa de pollo','familia'),('Milanesa S/.9','familia'),('Paneton','familia'),('Paneton / Chocolate','familia'),('Pollo Broaster','familia'),('Pollo Broaster S/.9','familia'),('Porcion de arroz','familia'),('Postres','familia'),('Promo 2 X 10 00','familia'),('Promo 2 X 10 0F','familia'),('Promo 2 X 10 0M','familia'),('Promo 2 X 10 FM','familia'),('Promo 2 X 10 L0','familia'),('Promo 2 X 10 LF','familia'),('Promo 2 X 10 LL','familia'),('Promo 2 X 10 LM','familia'),('Promo 2 X 10 LV','familia'),('Promo 2 X 10 M0','familia'),('Promo 2 X 10 MF','familia'),('Promo 2 X 10 MM','familia'),('Promo 2 X 10 MV','familia'),('Promo 2 X 10 VV','familia'),('Promo 3 Cono Bill x 5','familia'),('Salchi arroz','familia'),('Salchi arroz S/.9','familia'),('Salchi Pollo Bill','familia'),('Salchicono + Chicha','familia'),('Salchicono + Maracuya','familia'),('Salchicono Bill','familia'),('Salchipapa Bill','familia'),('Salchipapa Familiar','familia'),('Salchipapa Familiar Delivery','familia'),('Sanwich de Filete de Pollo','familia'),('Sundae 3 x 2','familia'),('Sundae Cua Cua','familia'),('Sundae de Chocolate','familia'),('Sundae de Fresa','familia'),('Sundae Doña Pepa','familia'),('Taper','familia'),('Te / Mate','familia'),('General','Marca'),('Dolar','Moneda'),('Euro','Moneda'),('Sol','Moneda'),('Efectivo','Pago'),('Master Card','Pago'),('Visa','Pago'),('Cliente_DNI','persona_tipo'),('Cliente_RUC','persona_tipo'),('Empleado','persona_tipo'),('Proveedor','persona_tipo'),('Servicio','persona_tipo'),('Transportista','persona_tipo'),('Producto_Final','producto_tipo'),('Detraccion','Regimen'),('Percepcion','Regimen'),('Retencion','Regimen'),('ONZ','Unidad');

--
-- Dumping data for table `per_dir`
--


--
-- Dumping data for table `persona`
--

INSERT INTO `persona` VALUES ('04600000','Milagros','Mili','2000-01-01','000000000','000000000','2000-01-01','2000-01-01','Activo','mili','Empleado',NULL),('71200000','Alexander Gabriel, Luna Choquecota','alex','2000-01-01','053483120','953627684','2000-01-01','2000-01-01','Activo','alex','Empleado',NULL),('71200685','ALEXANDER GABRIEL LUNA CHOQUECOTA',NULL,NULL,NULL,NULL,NULL,NULL,'Activo',NULL,'Cliente_DNI','');

--
-- Dumping data for table `plato`
--

INSERT INTO `plato` VALUES ('Agua San Luis 1 LT','Bebidas','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2),('Alitas Picantes','Comida Rapida','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff00','#c7c7c7',5.9),('Alitas SBBQ','Comida Rapida','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff00','#bcbcbc',8.9),('Arroz','Almuerzos','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3),('Arroz a la Cubana','Almuerzos','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9),('Arroz a la Cubana S/.9','Almuerzos','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9),('Cafe','Cafeteria','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3.5),('Cafe / Leche','Cafeteria','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4.5),('Chicha Jarra','Bebidas','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Chicha Morada Grande','Bebidas','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3.5),('Chicha Morada Regular','Bebidas','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2),('Chicha S/0','Bebidas','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Chicha S/1','Bebidas','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1),('Chocolate','Cafeteria','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4),('Choripan','Comida Rapida','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#aa0000',NULL,3.6),('Choripan / Bebida','Comida Rapida','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#aa0000','#d4d4d4',5),('Chuleta de Cerdo Junior','Almuerzos','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6),('Chuleta de Cerdo S/.9','Almuerzos','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9),('Cocacola 1.5','Bebidas','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1.5),('Cocacola 1.5 familiar','Bebidas','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Cocacola 2.5 Familiar Promo','Bebidas','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Cocacola 500 ml','Bebidas','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3),('Cocacola Prom S/1','Bebidas','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1),('Combo Bill','Postres','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Combo Bill Personal','Combos','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,10),('Combo Delivery','Combos','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,49),('Combo Nuggets','Combos','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,37),('Combo Para 2','Combos','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,11.9),('Combo Para 2 Delivery','Combos','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,14.9),('Combo Personal 1','Combos','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,21.9),('Combo Personal Bill','Combos','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,12),('Combo Picante','Combos','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,16),('Cono Bill Chocolate','Helados','General','ONZ',1,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Cono Bill Combinado','Helados','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Cono Bill Vainilla','Helados','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Cono Nugget + Chicha','Comida Rapida','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff7f','#55007f',5.8),('Cono Nugget + Maracuya','Comida Rapida','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff7f','#595959',5.8),('Cono Nuggets','Comida Rapida','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#ffff7f','#8f8f8f',4.5),('Crepe Jamon y Queso','Cafeteria','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Crepe Platano','Cafeteria','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Delivery S/.2','Combos','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2),('Delivery S/.4','Combos','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4),('Delivery S/.5','Combos','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Delivery S/.6','Combos','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6),('Delivery S/.7','Combos','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7),('Delivery S/.8','Combos','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,8),('Delivery S/3','Combos','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3),('Ensalada Cesar','Almuerzos','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6),('Ensalada Cesar S/.9','Almuerzos','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9),('Ensalada de frutas','Cafeteria','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Ensalada Regular','Comida Rapida','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#aaff00','#efefef',3.5),('Familiar 1','Combos','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Familiar 2','Combos','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Familiar 3','Combos','General','ONZ',6,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Filete de Pollo','Comida Rapida','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,'#ffffff',6.7),('Filete de Pollo Grill ','Almuerzos','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9),('Filete de Pollo Grill S/.9','Almuerzos','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9),('Frap de Fresa','Postres','General','ONZ',1,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Frap de Lucuma','Postres','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Frap de Moka','Postres','General','ONZ',1,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Frap de Oreo','Postres','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Frap de Vainilla','Postres','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Frozen de limon','Postres','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Huevo','Almuerzos','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1),('Inca Kola 1.5 Familiar','Bebidas','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Inca Kola 1.5 L','Bebidas','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1),('Inca Kola 2.5 Familiar Promo','Bebidas','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Inca Kola 500 ml','Bebidas','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3),('Inka Prom S/1','Bebidas','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1),('Jugo Grande de Papaya','Cafeteria','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Jugo Grande de Piña','Cafeteria','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Jugo Grande de Platano','Cafeteria','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Jugo Grande Surtido','Cafeteria','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Jugo Regular Papaya','Cafeteria','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Jugo Regular Piña','Cafeteria','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Jugo Regular Surtido','Cafeteria','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Leche','Cafeteria','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2.5),('Maracuya Grande','Bebidas','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3.5),('Maracuya Regular','Bebidas','General','ONZ',6,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2),('Maracuya S/0','Bebidas','General','ONZ',7,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0),('Maracuya S/1','Bebidas','General','ONZ',7,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,1),('Milanesa de pollo','Almuerzos','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9),('Milanesa S/.9','Almuerzos','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9),('Paneton','Cafeteria','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3.5),('Paneton / Chocolate','Cafeteria','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.5),('Pollo Broaster','Almuerzos','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9),('Pollo Broaster S/.9','Almuerzos','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9),('Porcion de arroz','Almuerzos','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,3),('Promo 2 X 10 00','Postres','General','ONZ',7,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 0F','Postres','General','ONZ',6,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 0M','Postres','General','ONZ',7,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 FM','Postres','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 L0','Postres','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 LF','Postres','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 LL','Postres','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 LM','Postres','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 LV','Postres','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 M0','Postres','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 MF','Postres','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 MM','Postres','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 MV','Postres','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 2 X 10 VV','Postres','General','ONZ',7,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Promo 3 Cono Bill x 5','Helados','General','ONZ',2,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Salchi arroz','Almuerzos','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,7.9),('Salchi arroz S/.9','Almuerzos','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,9),('Salchi Pollo Bill','Comida Rapida','General','ONZ',6,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6.7),('Salchicono + Chicha','Comida Rapida','General','ONZ',4,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4.9),('Salchicono + Maracuya','Comida Rapida','General','ONZ',4,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4.9),('Salchicono Bill','Comida Rapida','General','ONZ',4,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita','#000000',NULL,3.5),('Salchipapa Bill','Comida Rapida','General','ONZ',5,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,4.5),('Salchipapa Familiar','Comida Rapida','General','ONZ',5,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,13.9),('Salchipapa Familiar Delivery','Comida Rapida','General','ONZ',5,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,18.9),('Sanwich de Filete de Pollo','Comida Rapida','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,6.3),('Sundae 3 x 2','Helados','General','ONZ',2,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Sundae Cua Cua','Helados','General','ONZ',2,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Sundae de Chocolate','Helados','General','ONZ',3,1,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Sundae de Fresa','Helados','General','ONZ',3,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Sundae Doña Pepa','Helados','General','ONZ',3,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,5),('Taper','Almuerzos','General','ONZ',6,3,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,0.5),('Te / Mate','Cafeteria','General','ONZ',6,2,'MS Shell Dlg 2,12,-1,5,75,0,0,0,0,0,Negrita',NULL,NULL,2);

--
-- Dumping data for table `regimen`
--


--
-- Dumping data for table `usuario`
--

INSERT INTO `usuario` VALUES ('alex','1234'),('mili','1234');
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-08-05 13:43:34
