delete from comprobante;
ALTER TABLE comprobante AUTO_INCREMENT = 1;
ALTER TABLE comprobante_has_producto AUTO_INCREMENT = 1;
ALTER TABLE comprobante_has_persona AUTO_INCREMENT = 1;
delete from producto;
ALTER TABLE producto AUTO_INCREMENT = 1;
delete from familia;
ALTER TABLE familia AUTO_INCREMENT = 1;
delete from usuario;
ALTER TABLE usuario AUTO_INCREMENT = 1;
delete from persona;
delete from grupo;
delete from item;
delete from direccion;
delete from ingrediente;
ALTER TABLE ingrediente AUTO_INCREMENT = 1;
delete from datos;


INSERT INTO grupo(nombre)VALUES
('establecimiento')
, ('operacion')
, ('moneda')
, ('regimen')
, ('estado')
, ('pago')
, ('marca')
, ('familia')
, ('producto')
, ('unidad')
, ('medida')
, ('persona_tipo')
, ('producto_tipo')
, ('comprobante_operacion')
, ('comprobante_tipo')
;

INSERT INTO item(nombre, grupo_nombre)VALUES('UND', 'Unidad');

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
INSERT INTO item(nombre, grupo_nombre)VALUES('Anulado', 'Estado');
INSERT INTO item(nombre, grupo_nombre)VALUES('Suspendido', 'Estado');
INSERT INTO item(nombre, grupo_nombre)VALUES('Inactivo', 'Estado');

INSERT INTO item(nombre, grupo_nombre)VALUES('Efectivo', 'Pago');
INSERT INTO item(nombre, grupo_nombre)VALUES('Master Card', 'Pago');
INSERT INTO item(nombre, grupo_nombre)VALUES('Visa', 'Pago');

#INSERT INTO item(nombre, grupo_nombre)VALUES('General', 'Marca');

#INSERT INTO item(nombre, grupo_nombre)VALUES('Almuerzo', 'Familia');
#INSERT INTO item(nombre, grupo_nombre)VALUES('Comida_Rapida', 'Familia');
#INSERT INTO item(nombre, grupo_nombre)VALUES('Postre', 'Familia');


INSERT INTO item(nombre, grupo_nombre)VALUES('Master', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Administrador', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Empleado', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cajero', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Proveedor', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Transportista', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cliente_DNI', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cliente_RUC', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cliente_Eventual', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cliente_Delivery', 'persona_tipo');

INSERT INTO item(nombre, grupo_nombre)VALUES('Producto_Final', 'producto_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Servicio', 'persona_tipo');

INSERT INTO item(nombre, grupo_nombre)VALUES('Compra', 'comprobante_operacion');
INSERT INTO item(nombre, grupo_nombre)VALUES('Venta', 'comprobante_operacion');
INSERT INTO item(nombre, grupo_nombre)VALUES('Egresos', 'comprobante_operacion');

INSERT INTO item(nombre, grupo_nombre)VALUES('Ticket', 'comprobante_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Boleta', 'comprobante_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Factura', 'comprobante_tipo');

INSERT INTO usuario(nombre, pass)VALUES
('jorge'
, '2148');
INSERT INTO usuario(nombre, pass)VALUES
('alex'
, '1213');
INSERT INTO usuario(nombre, pass)VALUES
('leal'
, '1234');

INSERT INTO persona(cod, nombre, nombre_corto, fecha_nacimiento
, telefono, celular, fecha_inicio, fecha_fin, email
, estado_item_nombre
, tipo_item_nombre)VALUES
('00000000'
, 'Jorge'
, 'jorge'
, '2000-01-01'
, '000000000'
, '000000000'
, '2000-01-01'
, '2000-01-01'
, 'billmcquack.peru@gmail.com'
, 'Activo'
, 'Master');
INSERT INTO persona_has_usuario(persona_cod, usuario_id)VALUES(
'00000000', 1);

INSERT INTO persona(cod, nombre, nombre_corto, fecha_nacimiento
, telefono, celular, fecha_inicio, fecha_fin, email
, estado_item_nombre
, tipo_item_nombre)VALUES
('71200000'
, 'Alexander Gabriel, Luna Choquecota'
, 'alex'
, '2000-01-01'
, '053483120'
, '953627684'
, '2000-01-01'
, '2000-01-01'
, 'lordalex27@gmail.com'
, 'Activo'
, 'Administrador');
INSERT INTO persona_has_usuario(persona_cod, usuario_id)VALUES(
'71200000', 2);

INSERT INTO persona(cod, nombre, nombre_corto, fecha_nacimiento
, telefono, celular, fecha_inicio, fecha_fin, estado_item_nombre
, tipo_item_nombre)VALUES
('00000001'
, 'Leal'
, 'leal'
, '2000-01-01'
, '000000000'
, '000000000'
, '2000-01-01'
, '2000-01-01'
, 'Activo'
, 'Cajero');
INSERT INTO persona_has_usuario(persona_cod, usuario_id)VALUES(
'00000001', 3);

INSERT INTO datos(splitter_1, splitter_2, splitter_3
, familia_filas, familia_columnas, plato_filas, plato_columnas, clave_wifi)
VALUES
(174, 591, 582
, 2, 8, 7, 3, "SIN INTERNET");

