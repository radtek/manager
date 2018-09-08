LOAD DATA INFILE 'E:/programa/BD_Avicola_2.0/comprobante_has_producto.csv' 
INTO TABLE comprobante_has_producto
FIELDS TERMINATED BY ',' 
ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 ROWS