DELETE FROM persona;
ALTER TABLE persona AUTO_INCREMENT = 1;
ALTER TABLE naturales AUTO_INCREMENT = 1;
ALTER TABLE usuario AUTO_INCREMENT = 1;
DELETE FROM producto;
ALTER TABLE producto AUTO_INCREMENT = 1;
DELETE FROM documento;
ALTER TABLE documento AUTO_INCREMENT = 1;

INSERT INTO `managersystem`.`persona`
(`id`,
`tipo_persona_id`,
`habilitado`)
VALUES
(1,
5,
'1');

INSERT INTO `managersystem`.`naturales`
(`persona_id`,
`dni`, `nombre`)
VALUES
((SELECT MAX(persona.id) FROM persona),
'71200685', 'ALEXANDER GABRIEL LUNA CHOQUECOTA');

INSERT INTO `managersystem`.`usuario`
(`naturales_persona_id`,
`rol_id`,
`usuario`,
`pass`)
VALUES
((SELECT MAX(persona.id) FROM persona),
1,
'aaa',
'1234');

