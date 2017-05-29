use restaurante;

SELECT YEAR(venta.fecha) AS annio
, MONTH(venta.fecha) AS mes

, SUM(venta_h_plato.cantidad) AS cantidad FROM venta_h_plato 
JOIN plato ON plato.id = venta_h_plato.plato_id
JOIN venta ON venta.id = venta_h_plato.venta_id
--WHERE DATENAME(dw, venta.fecha) = 'Monday' AND DATENAME(mm, venta.fecha) = 'February'
GROUP BY YEAR(venta.fecha), MONTH(venta.fecha)
ORDER BY YEAR(venta.fecha), MONTH(venta.fecha);

GO