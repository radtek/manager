-- MySQL Script generated by MySQL Workbench
-- Tue May  1 01:08:14 2018
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema bd_avicola
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `bd_avicola` ;

-- -----------------------------------------------------
-- Schema bd_avicola
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `bd_avicola` DEFAULT CHARACTER SET utf8 ;
SHOW WARNINGS;
USE `bd_avicola` ;

-- -----------------------------------------------------
-- Table `bd_avicola`.`grupo`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`grupo` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`grupo` (
  `nombre` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`nombre`))
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`item`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`item` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`item` (
  `nombre` VARCHAR(45) NOT NULL,
  `grupo_nombre` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`nombre`),
  CONSTRAINT `fk_item_grupo1`
    FOREIGN KEY (`grupo_nombre`)
    REFERENCES `bd_avicola`.`grupo` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_item_grupo1_idx` ON `bd_avicola`.`item` (`grupo_nombre` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`usuario`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`usuario` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`usuario` (
  `nombre` VARCHAR(145) NOT NULL,
  `pass` VARCHAR(145) NULL,
  PRIMARY KEY (`nombre`))
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`persona`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`persona` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`persona` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `cod` VARCHAR(11) NULL,
  `nombre` VARCHAR(145) NULL,
  `nombre_corto` VARCHAR(45) NULL,
  `fecha_nacimiento` DATE NULL,
  `telefono` VARCHAR(90) NULL,
  `celular` VARCHAR(90) NULL,
  `fecha_inicio` DATE NULL,
  `fecha_fin` DATE NULL,
  `estado_item_nombre` VARCHAR(45) NOT NULL,
  `usuario_nombre` VARCHAR(145) NULL,
  `tipo_item_nombre` VARCHAR(45) NOT NULL,
  `direccion` VARCHAR(245) NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_persona_item1`
    FOREIGN KEY (`estado_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_persona_usuario1`
    FOREIGN KEY (`usuario_nombre`)
    REFERENCES `bd_avicola`.`usuario` (`nombre`)
    ON DELETE SET NULL
    ON UPDATE CASCADE,
  CONSTRAINT `fk_persona_item2`
    FOREIGN KEY (`tipo_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_persona_item1_idx` ON `bd_avicola`.`persona` (`estado_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_persona_usuario1_idx` ON `bd_avicola`.`persona` (`usuario_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_persona_item2_idx` ON `bd_avicola`.`persona` (`tipo_item_nombre` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`direccion`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`direccion` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`direccion` (
  `domicilio_fiscal` VARCHAR(255) NOT NULL,
  PRIMARY KEY (`domicilio_fiscal`))
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`divisa`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`divisa` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`divisa` (
  `fecha` FLOAT NOT NULL,
  `valor` FLOAT NULL,
  `moneda_item_nombre` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`fecha`),
  CONSTRAINT `fk_divisa_item1`
    FOREIGN KEY (`moneda_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_divisa_item1_idx` ON `bd_avicola`.`divisa` (`moneda_item_nombre` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`impuesto`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`impuesto` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`impuesto` (
  `fecha` DATE NOT NULL,
  `valor` FLOAT NULL,
  `moneda_item_nombre` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`fecha`),
  CONSTRAINT `fk_impuesto_item1`
    FOREIGN KEY (`moneda_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_impuesto_item1_idx` ON `bd_avicola`.`impuesto` (`moneda_item_nombre` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`comprobante`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`comprobante` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`comprobante` (
  `id` INT NOT NULL,
  `nombre` TEXT NULL,
  `serie` VARCHAR(4) NULL,
  `numero` INT NULL,
  `fecha_emision` DATETIME NULL,
  `anulado` TINYINT NULL,
  `pago_item_nombre` VARCHAR(45) NOT NULL,
  `estado_item_nombre` VARCHAR(45) NOT NULL,
  `operacion_item_nombre` VARCHAR(45) NOT NULL,
  `tipo_item_nombre` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_comprobante_item1`
    FOREIGN KEY (`pago_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_comprobante_item2`
    FOREIGN KEY (`estado_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_comprobante_item3`
    FOREIGN KEY (`operacion_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_comprobante_item4`
    FOREIGN KEY (`tipo_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_comprobante_item1_idx` ON `bd_avicola`.`comprobante` (`pago_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_comprobante_item2_idx` ON `bd_avicola`.`comprobante` (`estado_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_comprobante_item3_idx` ON `bd_avicola`.`comprobante` (`operacion_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_comprobante_item4_idx` ON `bd_avicola`.`comprobante` (`tipo_item_nombre` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`regimen`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`regimen` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`regimen` (
  `fecha` DATE NOT NULL,
  `valor` FLOAT NULL,
  `regimen_item_nombre` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`fecha`),
  CONSTRAINT `fk_regimen_item1`
    FOREIGN KEY (`regimen_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_regimen_de_pago_item1_idx` ON `bd_avicola`.`regimen` (`regimen_item_nombre` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`per_dir`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`per_dir` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`per_dir` (
  `persona_cod` VARCHAR(11) NOT NULL,
  `direccion_domicilio_fiscal` VARCHAR(255) NOT NULL,
  `establecimiento_item_nombre` VARCHAR(45) NOT NULL,
  CONSTRAINT `fk_per_dir_direccion1`
    FOREIGN KEY (`direccion_domicilio_fiscal`)
    REFERENCES `bd_avicola`.`direccion` (`domicilio_fiscal`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_per_dir_item1`
    FOREIGN KEY (`establecimiento_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_per_dir_persona1`
    FOREIGN KEY (`persona_cod`)
    REFERENCES `bd_avicola`.`persona` (`cod`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_per_dir_direccion1_idx` ON `bd_avicola`.`per_dir` (`direccion_domicilio_fiscal` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_per_dir_item1_idx` ON `bd_avicola`.`per_dir` (`establecimiento_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_per_dir_persona1_idx` ON `bd_avicola`.`per_dir` (`persona_cod` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`comprobante_has_persona`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`comprobante_has_persona` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`comprobante_has_persona` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `comprobante_id` INT NOT NULL,
  `persona_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_comprobante_has_persona_comprobante1`
    FOREIGN KEY (`comprobante_id`)
    REFERENCES `bd_avicola`.`comprobante` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_comprobante_has_persona_persona1`
    FOREIGN KEY (`persona_id`)
    REFERENCES `bd_avicola`.`persona` (`id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_comprobante_has_persona_comprobante1_idx` ON `bd_avicola`.`comprobante_has_persona` (`comprobante_id` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_comprobante_has_persona_persona1_idx` ON `bd_avicola`.`comprobante_has_persona` (`persona_id` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`categoria`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`categoria` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`categoria` (
  `item_nombre` VARCHAR(45) NOT NULL,
  `x` INT NULL,
  `y` INT NULL,
  `font` VARCHAR(145) NULL,
  `color` VARCHAR(45) NULL,
  `background_color` VARCHAR(45) NULL,
  PRIMARY KEY (`item_nombre`),
  CONSTRAINT `fk_familia_item1`
    FOREIGN KEY (`item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`datos`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`datos` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`datos` (
  `splitter_1` INT NULL,
  `splitter_2` INT NULL,
  `splitter_3` INT NULL,
  `categoria_filas` INT NULL,
  `categoria_columnas` INT NULL,
  `producto_filas` INT NULL,
  `producto_columnas` INT NULL,
  `clave_wifi` VARCHAR(45) NULL,
  `fondo_tapiz` VARCHAR(55) NULL)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`producto`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`producto` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`producto` (
  `item_nombre` VARCHAR(45) NOT NULL,
  `tipo_item_nombre` VARCHAR(45) NOT NULL,
  `categoria_item_nombre` VARCHAR(45) NOT NULL,
  `marca_item_nombre` VARCHAR(45) NOT NULL,
  `unidad_item_nombre` VARCHAR(45) NOT NULL,
  `x` INT NULL,
  `y` INT NULL,
  `font` VARCHAR(145) NULL,
  `color` VARCHAR(45) NULL,
  `background_color` VARCHAR(45) NULL,
  `descripcion` TEXT NULL,
  PRIMARY KEY (`item_nombre`),
  CONSTRAINT `fk_producto_item4`
    FOREIGN KEY (`item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_producto_item6`
    FOREIGN KEY (`marca_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_producto_item7`
    FOREIGN KEY (`unidad_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_producto_item1`
    FOREIGN KEY (`tipo_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_producto_familia1`
    FOREIGN KEY (`categoria_item_nombre`)
    REFERENCES `bd_avicola`.`categoria` (`item_nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_producto_item4_idx` ON `bd_avicola`.`producto` (`item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_producto_item6_idx` ON `bd_avicola`.`producto` (`marca_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_producto_item7_idx` ON `bd_avicola`.`producto` (`unidad_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_producto_item1_idx` ON `bd_avicola`.`producto` (`tipo_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_producto_familia1_idx` ON `bd_avicola`.`producto` (`categoria_item_nombre` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`comprobante_has_producto`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`comprobante_has_producto` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`comprobante_has_producto` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `producto_item_nombre` VARCHAR(45) NOT NULL,
  `comprobante_id` INT NOT NULL,
  `unidad_item_nombre` VARCHAR(45) NULL,
  `cantidad` FLOAT NULL,
  `peso` FLOAT NULL,
  `precio` FLOAT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_producto_has_comprobante_producto1`
    FOREIGN KEY (`producto_item_nombre`)
    REFERENCES `bd_avicola`.`producto` (`item_nombre`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_producto_has_comprobante_comprobante1`
    FOREIGN KEY (`comprobante_id`)
    REFERENCES `bd_avicola`.`comprobante` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_comprobante_has_producto_item1`
    FOREIGN KEY (`unidad_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE SET NULL
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_producto_has_comprobante_comprobante1_idx` ON `bd_avicola`.`comprobante_has_producto` (`comprobante_id` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_producto_has_comprobante_producto1_idx` ON `bd_avicola`.`comprobante_has_producto` (`producto_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_comprobante_has_producto_item1_idx` ON `bd_avicola`.`comprobante_has_producto` (`unidad_item_nombre` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`conversion`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`conversion` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`conversion` (
  `unidad_item_nombre` VARCHAR(45) NOT NULL,
  `unidad_item_nombre1` VARCHAR(45) NOT NULL,
  `value` FLOAT NULL,
  CONSTRAINT `fk_conversion_item1`
    FOREIGN KEY (`unidad_item_nombre`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_conversion_item2`
    FOREIGN KEY (`unidad_item_nombre1`)
    REFERENCES `bd_avicola`.`item` (`nombre`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_conversion_item1_idx` ON `bd_avicola`.`conversion` (`unidad_item_nombre` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_conversion_item2_idx` ON `bd_avicola`.`conversion` (`unidad_item_nombre1` ASC);

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `bd_avicola`.`pagos`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `bd_avicola`.`pagos` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `bd_avicola`.`pagos` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `comprobante_id` INT NOT NULL,
  `persona_id` INT NOT NULL,
  `fecha_pago` DATE NULL,
  `pago` FLOAT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_pagos_persona1`
    FOREIGN KEY (`persona_id`)
    REFERENCES `bd_avicola`.`persona` (`id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_pagos_comprobante1`
    FOREIGN KEY (`comprobante_id`)
    REFERENCES `bd_avicola`.`comprobante` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

SHOW WARNINGS;
CREATE INDEX `fk_pagos_persona1_idx` ON `bd_avicola`.`pagos` (`persona_id` ASC);

SHOW WARNINGS;
CREATE INDEX `fk_pagos_comprobante1_idx` ON `bd_avicola`.`pagos` (`comprobante_id` ASC);

SHOW WARNINGS;
USE `bd_avicola` ;

-- -----------------------------------------------------
-- procedure multiple_query
-- -----------------------------------------------------

USE `bd_avicola`;
DROP procedure IF EXISTS `bd_avicola`.`multiple_query`;
SHOW WARNINGS;

DELIMITER $$
USE `bd_avicola`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `multiple_query`(m_query TEXT)
BEGIN
	/* INICIALIZACION*/
    SET @m_query:= m_query;
 
	label1: LOOP
		SET @pos:= LOCATE('&&END_QUERY&&', @m_query);
        SET @query_:= SUBSTRING(@m_query,1, @pos-1);
                
		
        PREPARE stmt1 FROM @query_;	
		EXECUTE stmt1;

		DEALLOCATE PREPARE stmt1;
        
        SET @m_query = SUBSTRING(@m_query, @pos+13);

		IF @m_query = '' THEN
		  LEAVE label1;
		END IF;			
	END LOOP label1;    
END$$

DELIMITER ;
SHOW WARNINGS;

-- -----------------------------------------------------
-- procedure puntos
-- -----------------------------------------------------

USE `bd_avicola`;
DROP procedure IF EXISTS `bd_avicola`.`puntos`;
SHOW WARNINGS;

DELIMITER $$
USE `bd_avicola`$$
CREATE PROCEDURE `puntos` (per_cod TEXT, prod_item_nombre TEXT)
BEGIN
SET @size := (SELECT COUNT(*)
FROM (SELECT com.fecha_emision AS fecha_emision, ROUND(SUM(com_h_prod.precio) / SUM(com_h_prod.peso), 1) AS p_unit
FROM producto prod
JOIN comprobante_has_producto com_h_prod ON prod.item_nombre = com_h_prod.producto_item_nombre
JOIN comprobante com ON com.cod = com_h_prod.comprobante_cod
JOIN comprobante_has_persona com_h_per ON com_h_per.comprobante_cod = com_h_prod.comprobante_cod
JOIN persona per ON per.cod = com_h_per.persona_cod
WHERE per.cod = '7' AND prod.item_nombre = 'POLLO C/M' GROUP BY com.fecha_emision) AS t
GROUP BY t.fecha_emision, t.p_unit ORDER BY t.fecha_emision DESC);

SET @counter := 0;

label1: LOOP
	IF @counter < @size THEN
		LEAVE label1;
	END IF;
    
    
    SET @counter := @counter+1;
END LOOP label1;

END$$

DELIMITER ;
SHOW WARNINGS;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
