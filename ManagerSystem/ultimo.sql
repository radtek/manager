-- MySQL dump 10.13  Distrib 5.7.17, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: managersystem
-- ------------------------------------------------------
-- Server version	5.7.17-log
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `managersystem`
--

USE `managersystem`;

--
-- Dumping data for table `amplitud`
--


--
-- Dumping data for table `anexo`
--


--
-- Dumping data for table `boleta`
--


--
-- Dumping data for table `cliente_dni`
--


--
-- Dumping data for table `cliente_ruc`
--


--
-- Dumping data for table `comprobante`
--


--
-- Dumping data for table `cotizacion`
--


--
-- Dumping data for table `delimitador`
--


--
-- Dumping data for table `documento`
--


--
-- Dumping data for table `documento_h_documento`
--


--
-- Dumping data for table `documento_h_persona`
--


--
-- Dumping data for table `documento_h_producto`
--


--
-- Dumping data for table `factura`
--


--
-- Dumping data for table `guia_remision_remitente`
--


--
-- Dumping data for table `igv`
--


--
-- Dumping data for table `juridica`
--


--
-- Dumping data for table `marca`
--


--
-- Dumping data for table `moneda`
--

INSERT INTO `moneda` VALUES (1,'soles'),(2,'dolares'),(3,'euros');

--
-- Dumping data for table `naturales`
--


--
-- Dumping data for table `nota_credito`
--


--
-- Dumping data for table `nota_debito`
--


--
-- Dumping data for table `nota_pedido`
--


--
-- Dumping data for table `persona`
--

INSERT INTO `persona` VALUES (1,1);

--
-- Dumping data for table `producto`
--


--
-- Dumping data for table `proforma`
--


--
-- Dumping data for table `proveedor`
--


--
-- Dumping data for table `rol`
--

INSERT INTO `rol` VALUES (2,'administrador'),(1,'programador'),(3,'vendedor');

--
-- Dumping data for table `tipo`
--


--
-- Dumping data for table `tipo_cambio`
--


--
-- Dumping data for table `transportista`
--


--
-- Dumping data for table `unidad`
--


--
-- Dumping data for table `usuario`
--

INSERT INTO `usuario` VALUES (1,1,1,'aaa','1234');
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-01-07 20:42:13
