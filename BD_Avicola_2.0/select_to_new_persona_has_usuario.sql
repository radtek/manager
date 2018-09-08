SET @id := 0;
SELECT @id := @id+1, per.conteo, u.id FROM persona per JOIN usuario u ON u.nombre = per.usuario_nombre;
