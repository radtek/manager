DELETE FROM rol where id <> 0;
ALTER TABLE rol AUTO_INCREMENT = 1;
DELETE FROM moneda where id <> 0;
ALTER TABLE moneda AUTO_INCREMENT = 1;

INSERT INTO rol(id, rol)VALUES(1, 'Programador');
INSERT INTO rol(id, rol)VALUES(2, 'Administrador');
INSERT INTO rol(id, rol)VALUES(3, 'Vendedor');

INSERT INTO moneda(id, moneda)VALUES(1, 'soles');
INSERT INTO moneda(id, moneda)VALUES(2, 'dolares');
INSERT INTO moneda(id, moneda)VALUES(3, 'euros');

INSERT INTO tipo_persona(id, numero, persona)VALUES(1, 0, 'proveedor');
INSERT INTO tipo_persona(id, numero, persona)VALUES(2, 1, 'transportista');
INSERT INTO tipo_persona(id, numero, persona)VALUES(3, 2, 'cliente_ruc');
INSERT INTO tipo_persona(id, numero, persona)VALUES(4, 3, 'cliente_dni');
INSERT INTO tipo_persona(id, numero, persona)VALUES(5, 4, 'usuario');
INSERT INTO tipo_persona(id, numero, persona)VALUES(6, 5, 'desconocido');

INSERT INTO tipo_documento(id, numero, documento)VALUES(1, 0, 'registro sin documento');
INSERT INTO tipo_documento(id, numero, documento)VALUES(2, 1, 'boleta');
INSERT INTO tipo_documento(id, numero, documento)VALUES(3, 2, 'factura');
INSERT INTO tipo_documento(id, numero, documento)VALUES(4, 3, 'nota de pedido');
INSERT INTO tipo_documento(id, numero, documento)VALUES(5, 4, 'guia de remision remitente');
INSERT INTO tipo_documento(id, numero, documento)VALUES(6, 5, 'cotizacion');
INSERT INTO tipo_documento(id, numero, documento)VALUES(7, 6, 'nota de credito');
INSERT INTO tipo_documento(id, numero, documento)VALUES(8, 7, 'nota de debito');
INSERT INTO tipo_documento(id, numero, documento)VALUES(9, 8, 'flete');
INSERT INTO tipo_documento(id, numero, documento)VALUES(10, 9, 'Orden');
INSERT INTO tipo_documento(id, numero, documento)VALUES(11, 10, 'Saldo');

INSERT INTO operacion(id, operacion)VALUES(1, 'compra');
INSERT INTO operacion(id, operacion)VALUES(2, 'venta');