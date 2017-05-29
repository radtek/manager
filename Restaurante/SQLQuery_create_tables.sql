USE restaurante;  
GO
DROP TABLE IF EXISTS busqueda_annio;
DROP TABLE IF EXISTS busqueda_mes;
DROP TABLE IF EXISTS busqueda_semana;
DROP TABLE IF EXISTS busqueda_dia;

DROP TABLE IF EXISTS venta_h_plato;
DROP TABLE IF EXISTS ingrediente_h_plato;
DROP TABLE IF EXISTS ingrediente_h_compra;
DROP TABLE IF EXISTS compra;
DROP TABLE IF EXISTS ingrediente;
DROP TABLE IF EXISTS plato;
DROP TABLE IF EXISTS venta;
DROP TABLE IF EXISTS medida;
DROP TABLE IF EXISTS unidad;

CREATE TABLE busqueda_annio (
	annio varchar(32)
);
CREATE TABLE busqueda_mes (
	mes int
);
CREATE TABLE busqueda_semana (
	semana int
);
CREATE TABLE busqueda_dia (
	dia int
);

CREATE TABLE medida (
    id int PRIMARY KEY NOT NULL IDENTITY(1,1),
	medida varchar(65)
);
CREATE TABLE ingrediente (
    id int PRIMARY KEY NOT NULL IDENTITY(1,1),
    nombre varchar(255),
    medida_id int NOT NULL,    
    cantidad float
	FOREIGN KEY ( medida_id ) REFERENCES [medida] ( id )
	ON UPDATE NO ACTION ON DELETE NO ACTION
);
CREATE TABLE compra (
	id int PRIMARY KEY NOT NULL IDENTITY(1,1),
	fecha DATE
);
CREATE TABLE ingrediente_h_compra (
    id int PRIMARY KEY NOT NULL IDENTITY(1,1),
    ingrediente_id int NOT NULL,
    compra_id int NOT NULL
	FOREIGN KEY ( ingrediente_id ) REFERENCES [ingrediente] ( id )
	ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ( compra_id ) REFERENCES [compra] ( id )
	ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE plato (
    id int PRIMARY KEY NOT NULL IDENTITY(1,1),
    nombre varchar(255),
    medida_id int NOT NULL   
	FOREIGN KEY ( medida_id ) REFERENCES [medida] ( id )
	ON UPDATE NO ACTION ON DELETE NO ACTION
);
CREATE TABLE ingrediente_h_plato (
    id int PRIMARY KEY NOT NULL IDENTITY(1,1),
    ingrediente_id int NOT NULL,
    plato_id int NOT NULL
	FOREIGN KEY ( ingrediente_id ) REFERENCES [ingrediente] ( id )
	ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ( plato_id ) REFERENCES [plato] ( id )
	ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE venta (
	id int PRIMARY KEY NOT NULL IDENTITY(1,1),
	fecha DATE
);
CREATE TABLE venta_h_plato (
    id int PRIMARY KEY NOT NULL IDENTITY(1,1),
    venta_id int NOT NULL,
    plato_id int NOT NULL,
	cantidad int
	FOREIGN KEY ( venta_id ) REFERENCES [venta] ( id )
	ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY ( plato_id ) REFERENCES [plato] ( id )
	ON UPDATE CASCADE ON DELETE CASCADE
);

INSERT INTO medida(medida)VALUES('1 KG');--1
INSERT INTO medida(medida)VALUES('1 CUCHARADA');
INSERT INTO medida(medida)VALUES('3 CUCHARADAS');
INSERT INTO medida(medida)VALUES('1/4 DE TAZA');
INSERT INTO medida(medida)VALUES('4 TAZAS');--5
INSERT INTO medida(medida)VALUES('1/2 TAZA');
INSERT INTO medida(medida)VALUES('3 CEBOLLAS');
INSERT INTO medida(medida)VALUES('4 GRANOS');
INSERT INTO medida(medida)VALUES('1 UND');
INSERT INTO medida(medida)VALUES('2 LITROS');--10

INSERT INTO medida(medida)VALUES('4 PRESAS');--1
INSERT INTO medida(medida)VALUES('1 CUCHARADITA');
INSERT INTO medida(medida)VALUES('2 UND');
INSERT INTO medida(medida)VALUES('1/2 UND');
INSERT INTO medida(medida)VALUES('1/2 PAQUETE');--5
INSERT INTO medida(medida)VALUES('AL GUSTO');
INSERT INTO medida(medida)VALUES('4 UND');
INSERT INTO medida(medida)VALUES('1 PECHUGA');
INSERT INTO medida(medida)VALUES('1 PAQUETE');
INSERT INTO medida(medida)VALUES('2 CUCHARADAS');--10

INSERT INTO medida(medida)VALUES('2 CUCHARADITAS');--1
INSERT INTO medida(medida)VALUES('1 TAZA');
INSERT INTO medida(medida)VALUES('1/4 BOTELLA');
INSERT INTO medida(medida)VALUES('5 UND');

INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('PALETILLA DE CERDO', 1, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('AJO MOLIDO', 2, 5.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('AJI PANCA MOLIDO', 3, 5.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('VINAGRE ROJO', 4, 5.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('CHICHA DE JORA', 5, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('CEBOLLA LICUADA', 6, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('CEBOLLA CORTADAS EN GAJOS(PLUMA GRUESA)', 7, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('PIMIENTA DE CHAMA TRITURADOS', 8, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('OREGANO SECO', 2, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('ROCOTO SIN VENAS', 9, 10.0);

INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('AGUA', 10, 20.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('POLLO', 11, 20.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('ZANAHORIA CORTADA EN BASTONES', 13, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('APIO PICADO', 6, 20.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('PAPA GRANDE CORTADA EN TROZOS', 9, 50.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('CEBOLLADA PICADA', 14, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('FIDEO CABELLO DE ANGEL', 15, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('SAL', 16, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('PIMIENTA', 16, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('OREGANO', 16, 10.0);

INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('HUEVOS', 17, 50.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('POLLO', 18, 20.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('HOT DOG(SALCHICHA)', 19, 60.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('KION PICADO', 20, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('AJO PICADO', 21, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('PIMIENTO PICADO', 6, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('ARROZ', 5, 50.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('SILLAO(SALSA DE SOYA)', 6, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('ACEITE DE AJONJOLI', 4, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('CEBOLLA CHINA PICADA', 22, 10.0);

INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('ACEITE', 23, 10.0);
INSERT INTO ingrediente(nombre, medida_id, cantidad)VALUES('SAL', 4, 10.0);

INSERT INTO plato(nombre, medida_id)VALUES('ADOBO DE CHANCHO', 17);
INSERT INTO plato(nombre, medida_id)VALUES('SOPA DE POLLO', 17);
INSERT INTO plato(nombre, medida_id)VALUES('ARROZ CHAUFA', 24);

INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(1, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(2, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(3, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(4, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(5, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(6, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(7, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(8, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(9, 1);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(10, 1);

INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(11, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(12, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(2, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(13, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(14, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(15, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(16, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(17, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(18, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(19, 2);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(20, 2);

INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(21, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(22, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(23, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(24, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(25, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(26, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(27, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(28, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(29, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(30, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(31, 3);
INSERT INTO ingrediente_h_plato(ingrediente_id, plato_id)VALUES(32, 3);

INSERT INTO venta(fecha)VALUES('2015-01-01');--1
INSERT INTO venta(fecha)VALUES('2015-01-01');
INSERT INTO venta(fecha)VALUES('2015-01-01');
INSERT INTO venta(fecha)VALUES('2015-01-02');
INSERT INTO venta(fecha)VALUES('2015-01-02');
INSERT INTO venta(fecha)VALUES('2015-01-02');
INSERT INTO venta(fecha)VALUES('2015-01-03');
INSERT INTO venta(fecha)VALUES('2015-01-03');
INSERT INTO venta(fecha)VALUES('2015-01-03');
INSERT INTO venta(fecha)VALUES('2015-01-03');--10

INSERT INTO venta(fecha)VALUES('2015-02-01');--11
INSERT INTO venta(fecha)VALUES('2015-02-01');
INSERT INTO venta(fecha)VALUES('2015-02-01');
INSERT INTO venta(fecha)VALUES('2015-02-02');
INSERT INTO venta(fecha)VALUES('2015-02-02');
INSERT INTO venta(fecha)VALUES('2015-02-02');
INSERT INTO venta(fecha)VALUES('2015-02-03');
INSERT INTO venta(fecha)VALUES('2015-02-03');
INSERT INTO venta(fecha)VALUES('2015-02-03');
INSERT INTO venta(fecha)VALUES('2015-02-03');--20

INSERT INTO venta(fecha)VALUES('2016-01-01');--21
INSERT INTO venta(fecha)VALUES('2016-01-01');
INSERT INTO venta(fecha)VALUES('2016-01-01');
INSERT INTO venta(fecha)VALUES('2016-01-02');
INSERT INTO venta(fecha)VALUES('2016-01-02');
INSERT INTO venta(fecha)VALUES('2016-01-02');
INSERT INTO venta(fecha)VALUES('2016-01-03');
INSERT INTO venta(fecha)VALUES('2016-01-03');
INSERT INTO venta(fecha)VALUES('2016-01-03');
INSERT INTO venta(fecha)VALUES('2016-01-03');--30

INSERT INTO venta(fecha)VALUES('2016-02-01');--31
INSERT INTO venta(fecha)VALUES('2016-02-01');
INSERT INTO venta(fecha)VALUES('2016-02-01');
INSERT INTO venta(fecha)VALUES('2016-02-02');
INSERT INTO venta(fecha)VALUES('2016-02-02');
INSERT INTO venta(fecha)VALUES('2016-02-02');
INSERT INTO venta(fecha)VALUES('2016-02-03');
INSERT INTO venta(fecha)VALUES('2016-02-03');
INSERT INTO venta(fecha)VALUES('2016-02-03');
INSERT INTO venta(fecha)VALUES('2016-02-03');--40

INSERT INTO venta(fecha)VALUES('2017-01-01');--41
INSERT INTO venta(fecha)VALUES('2017-01-01');
INSERT INTO venta(fecha)VALUES('2017-01-01');
INSERT INTO venta(fecha)VALUES('2017-01-02');
INSERT INTO venta(fecha)VALUES('2017-01-02');
INSERT INTO venta(fecha)VALUES('2017-01-02');
INSERT INTO venta(fecha)VALUES('2017-01-03');
INSERT INTO venta(fecha)VALUES('2017-01-03');
INSERT INTO venta(fecha)VALUES('2017-01-03');
INSERT INTO venta(fecha)VALUES('2017-01-03');--50

INSERT INTO venta(fecha)VALUES('2017-02-01');--51
INSERT INTO venta(fecha)VALUES('2017-02-01');
INSERT INTO venta(fecha)VALUES('2017-02-01');
INSERT INTO venta(fecha)VALUES('2017-02-02');
INSERT INTO venta(fecha)VALUES('2017-02-02');
INSERT INTO venta(fecha)VALUES('2017-02-02');
INSERT INTO venta(fecha)VALUES('2017-02-03');
INSERT INTO venta(fecha)VALUES('2017-02-03');
INSERT INTO venta(fecha)VALUES('2017-02-03');
INSERT INTO venta(fecha)VALUES('2017-02-03');--60

INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(1, 1, 1);--1
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(2, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(3, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(4, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(5, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(6, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(7, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(8, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(9, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(10, 3, 1);--10

INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(11, 1, 1);--1
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(12, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(13, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(14, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(15, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(16, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(17, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(18, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(19, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(20, 3, 1);--20

INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(21, 3, 1);--1
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(22, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(23, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(24, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(25, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(26, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(27, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(28, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(29, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(30, 3, 1);--30

INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(31, 3, 1);--1
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(32, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(33, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(34, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(35, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(36, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(37, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(38, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(39, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(40, 3, 1);--40

INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(41, 1, 1);--1
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(42, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(43, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(44, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(45, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(46, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(47, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(48, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(49, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(50, 3, 1);--50

INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(51, 3, 1);--1
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(52, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(53, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(54, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(55, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(56, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(57, 1, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(58, 2, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(59, 3, 1);
INSERT INTO venta_h_plato(venta_id, plato_id, cantidad)VALUES(60, 3, 1);--60

GO
