INSERT INTO producto(nombre, familia_id
, operacion_item_nombre, x, y, font, color
, background_color, descripcion, precio, estado_item_nombre
, conteo, medida) SELECT prod.nombre, 10
, 'Compra', prod.x, prod.y, prod.font, prod.color
, prod.background_color, prod.descripcion, "0.0", prod.estado_item_nombre
, prod.conteo, prod.medida FROM producto prod
WHERE prod.operacion_item_nombre = 'Venta' AND familia_id = 1;