LOAD DATA INFILE "E:/familia.csv"
INTO TABLE familia
COLUMNS TERMINATED BY ','
ENCLOSED BY '"'
ESCAPED BY '"'
LINES TERMINATED BY '\n';

LOAD DATA INFILE "E:/plato.csv"
INTO TABLE plato
COLUMNS TERMINATED BY ','
ENCLOSED BY '"'
ESCAPED BY '"'
LINES TERMINATED BY '\n';

LOAD DATA INFILE "E:/comprobante.csv"
INTO TABLE comprobante
COLUMNS TERMINATED BY ','
OPTIONALLY ENCLOSED BY '"'
ESCAPED BY '"'
LINES TERMINATED BY '\n';

LOAD DATA INFILE "E:/comprobante_has_plato.csv"
INTO TABLE comprobante_has_plato
COLUMNS TERMINATED BY ','
OPTIONALLY ENCLOSED BY '"'
ESCAPED BY '"'
LINES TERMINATED BY '\n';