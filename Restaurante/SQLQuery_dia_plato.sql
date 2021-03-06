use restaurante;

SELECT plato.id AS plato, plato.nombre AS plato_nombre 
, DATEPART(dw, venta.fecha) AS dia, SUM(venta_h_plato.cantidad) AS cantidad FROM venta_h_plato 
JOIN plato ON plato.id = venta_h_plato.plato_id
JOIN venta ON venta.id = venta_h_plato.venta_id
--WHERE DATENAME(dw, venta.fecha) = 'Monday' AND DATENAME(mm, venta.fecha) = 'February'
GROUP BY plato.id, plato.nombre, DATEPART(dw, venta.fecha) 
ORDER BY plato.id, plato.nombre, DATEPART(dw, venta.fecha);

GO