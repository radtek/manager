delete from familia;
delete from comprobante;
ALTER TABLE comprobante AUTO_INCREMENT = 1;
delete from usuario;
delete from persona;
delete from grupo;
delete from item;
delete from direccion;
delete from plato;


INSERT INTO grupo(nombre)VALUES
('establecimiento')
, ('operacion')
, ('moneda')
, ('regimen')
, ('estado')
, ('pago')
, ('marca')
, ('familia')
, ('unidad')
, ('persona_tipo')
, ('producto_tipo')
, ('comprobante_operacion')
, ('comprobante_tipo')
;

INSERT INTO item(nombre, grupo_nombre)VALUES('Casa', 'Establecimiento');
INSERT INTO item(nombre, grupo_nombre)VALUES('Principal', 'Establecimiento');
INSERT INTO item(nombre, grupo_nombre)VALUES('Sucursal', 'Establecimiento');

INSERT INTO item(nombre, grupo_nombre)VALUES('Sol', 'Moneda');
INSERT INTO item(nombre, grupo_nombre)VALUES('Dolar', 'Moneda');
INSERT INTO item(nombre, grupo_nombre)VALUES('Euro', 'Moneda');

INSERT INTO item(nombre, grupo_nombre)VALUES('Percepcion', 'Regimen');
INSERT INTO item(nombre, grupo_nombre)VALUES('Detraccion', 'Regimen');
INSERT INTO item(nombre, grupo_nombre)VALUES('Retencion', 'Regimen');

INSERT INTO item(nombre, grupo_nombre)VALUES('Activo', 'Estado');
INSERT INTO item(nombre, grupo_nombre)VALUES('Suspendido', 'Estado');

INSERT INTO item(nombre, grupo_nombre)VALUES('Efectivo', 'Pago');
INSERT INTO item(nombre, grupo_nombre)VALUES('Master Card', 'Pago');
INSERT INTO item(nombre, grupo_nombre)VALUES('Visa', 'Pago');

INSERT INTO item(nombre, grupo_nombre)VALUES('General', 'Marca');

#INSERT INTO item(nombre, grupo_nombre)VALUES('Almuerzo', 'Familia');
#INSERT INTO item(nombre, grupo_nombre)VALUES('Comida_Rapida', 'Familia');
#INSERT INTO item(nombre, grupo_nombre)VALUES('Postre', 'Familia');

INSERT INTO item(nombre, grupo_nombre)VALUES('ONZ', 'Unidad');

INSERT INTO item(nombre, grupo_nombre)VALUES('Administrador', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Empleado', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Proveedor', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Transportista', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cliente_DNI', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cliente_RUC', 'persona_tipo');

INSERT INTO item(nombre, grupo_nombre)VALUES('Producto_Final', 'producto_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Servicio', 'persona_tipo');

INSERT INTO item(nombre, grupo_nombre)VALUES('Compra', 'comprobante_operacion');
INSERT INTO item(nombre, grupo_nombre)VALUES('Venta', 'comprobante_operacion');

INSERT INTO item(nombre, grupo_nombre)VALUES('Ticket', 'comprobante_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Boleta', 'comprobante_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Factura', 'comprobante_tipo');

INSERT INTO usuario(nombre, pass)VALUES
('alex'
, '1234');
INSERT INTO usuario(nombre, pass)VALUES
('mili'
, '1234');

INSERT INTO persona(cod, nombre, nombre_corto, fecha_nacimiento
, telefono, celular, fecha_inicio, fecha_fin, estado_item_nombre, usuario_nombre
, tipo_item_nombre)VALUES
('71200000'
, 'Alexander Gabriel, Luna Choquecota'
, 'alex'
, '2000-01-01'
, '053483120'
, '953627684'
, '2000-01-01'
, '2000-01-01'
, 'Activo'
, 'alex'
, 'Administrador');
INSERT INTO persona(cod, nombre, nombre_corto, fecha_nacimiento
, telefono, celular, fecha_inicio, fecha_fin, estado_item_nombre, usuario_nombre
, tipo_item_nombre)VALUES
('04600000'
, 'Milagros'
, 'Mili'
, '2000-01-01'
, '000000000'
, '000000000'
, '2000-01-01'
, '2000-01-01'
, 'Activo'
, 'mili'
, 'Empleado');
INSERT INTO datos(splitter_1, splitter_2, splitter_3
, familia_filas, familia_columnas, plato_filas, plato_columnas)
VALUES
(174, 591, 582
, 2, 8, 7, 3);

