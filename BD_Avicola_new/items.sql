delete from comprobante_has_persona;
ALTER TABLE comprobante_has_persona AUTO_INCREMENT = 1;
delete from comprobante_has_producto;
ALTER TABLE comprobante_has_producto AUTO_INCREMENT = 1;
delete from comprobante;
ALTER TABLE comprobante AUTO_INCREMENT = 1;
delete from comprobante;
delete from categoria;
#ALTER TABLE categoria AUTO_INCREMENT = 1;
delete from usuario;
ALTER TABLE usuario AUTO_INCREMENT = 1;
delete from persona;
#ALTER TABLE persona AUTO_INCREMENT = 1;
delete from grupo;
ALTER TABLE grupo AUTO_INCREMENT = 1;
delete from item;
ALTER TABLE item AUTO_INCREMENT = 1;
delete from direccion;
ALTER TABLE direccion AUTO_INCREMENT = 1;
delete from producto;
#ALTER TABLE producto AUTO_INCREMENT = 1;


INSERT INTO grupo(nombre)VALUES
('establecimiento')
, ('transaccion')
, ('operacion')
, ('moneda')
, ('regimen')
, ('estado')
, ('pago')
, ('marca')
, ('categoria')
, ('unidad')
, ('persona_tipo')
, ('persona')
, ('producto_tipo')
, ('producto')
, ('comprobante_tipo')
, ('comprobante')
;

INSERT INTO item(nombre, grupo_nombre)VALUES('Casa', 'Establecimiento');
INSERT INTO item(nombre, grupo_nombre)VALUES('Principal', 'Establecimiento');
INSERT INTO item(nombre, grupo_nombre)VALUES('Sucursal', 'Establecimiento');

INSERT INTO item(nombre, grupo_nombre)VALUES('Ingresar', 'Transaccion');
INSERT INTO item(nombre, grupo_nombre)VALUES('Modificar', 'Transaccion');
INSERT INTO item(nombre, grupo_nombre)VALUES('Eliminar', 'Transaccion');

INSERT INTO item(nombre, grupo_nombre)VALUES('Sol', 'Moneda');
INSERT INTO item(nombre, grupo_nombre)VALUES('Dolar', 'Moneda');
INSERT INTO item(nombre, grupo_nombre)VALUES('Euro', 'Moneda');

INSERT INTO item(nombre, grupo_nombre)VALUES('Percepcion', 'Regimen');
INSERT INTO item(nombre, grupo_nombre)VALUES('Detraccion', 'Regimen');
INSERT INTO item(nombre, grupo_nombre)VALUES('Retencion', 'Regimen');

INSERT INTO item(nombre, grupo_nombre)VALUES('Activo', 'Estado');
INSERT INTO item(nombre, grupo_nombre)VALUES('Suspendido', 'Estado');
INSERT INTO item(nombre, grupo_nombre)VALUES('Inactivo', 'Estado');
INSERT INTO item(nombre, grupo_nombre)VALUES('Anulado', 'Estado');

INSERT INTO item(nombre, grupo_nombre)VALUES('Efectivo', 'Pago');
INSERT INTO item(nombre, grupo_nombre)VALUES('Master Card', 'Pago');
INSERT INTO item(nombre, grupo_nombre)VALUES('Visa', 'Pago');

INSERT INTO item(nombre, grupo_nombre)VALUES('General', 'Marca');

#INSERT INTO item(nombre, grupo_nombre)VALUES('Almuerzo', 'Familia');
#INSERT INTO item(nombre, grupo_nombre)VALUES('Comida_Rapida', 'Familia');
#INSERT INTO item(nombre, grupo_nombre)VALUES('Postre', 'Familia');

INSERT INTO item(nombre, grupo_nombre)VALUES('JABAS', 'Unidad');
INSERT INTO item(nombre, grupo_nombre)VALUES('UNID', 'Unidad');
INSERT INTO item(nombre, grupo_nombre)VALUES('POLLOS', 'Unidad');
INSERT INTO item(nombre, grupo_nombre)VALUES('ONZ', 'Unidad');
INSERT INTO item(nombre, grupo_nombre)VALUES('KG', 'Unidad');

INSERT INTO item(nombre, grupo_nombre)VALUES('Administrador', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Empleado', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Almacenero', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Proveedor', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Transportista', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cliente_DNI', 'persona_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Cliente_RUC', 'persona_tipo');

INSERT INTO item(nombre, grupo_nombre)VALUES('Insumo', 'producto_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Producto', 'producto_tipo');

INSERT INTO item(nombre, grupo_nombre)VALUES('Compra', 'operacion');
INSERT INTO item(nombre, grupo_nombre)VALUES('Venta', 'operacion');

INSERT INTO item(nombre, grupo_nombre)VALUES('Salida', 'comprobante_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('NPedido', 'comprobante_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Ticket', 'comprobante_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Boleta', 'comprobante_tipo');
INSERT INTO item(nombre, grupo_nombre)VALUES('Factura', 'comprobante_tipo');

INSERT INTO usuario(nombre, pass)VALUES
('alex'
, '1234');
INSERT INTO usuario(nombre, pass)VALUES
('ivan'
, '2148');

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
, 'Ivan'
, 'Ivan'
, '2000-01-01'
, '000000000'
, '000000000'
, '2000-01-01'
, '2000-01-01'
, 'Activo'
, 'ivan'
, 'Empleado');
INSERT INTO datos(splitter_1, splitter_2, splitter_3
, categoria_filas, categoria_columnas, producto_filas, producto_columnas, clave_wifi)
VALUES
(174, 591, 582
, 2, 8, 7, 3, "ALASPERUANAS");

