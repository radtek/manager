use restaurante;

SELECT DATENAME(wk, venta.fecha) AS semana
, SUM(venta_h_plato.cantidad) AS cantidad FROM venta_h_plato 
JOIN plato ON plato.id = venta_h_plato.plato_id
JOIN venta ON venta.id = venta_h_plato.venta_id

GROUP BY DATENAME(wk, venta.fecha)
ORDER BY DATENAME(wk, venta.fecha);

GO