-- Instance Dalaran Arena
-- Instance last changes: a393

UPDATE gameobject_template SET faction = 114, flags = 32, size = 1.5 WHERE entry IN (192642, 192643);
DELETE FROM gameobject WHERE map = 617;
INSERT INTO gameobject VALUES
-- buffs
('222020','184663','617','1','1','1291.7','813.424','7.11472','4.64562','0','0','0.730314','-0.683111','-120','100','1'),
('222021','184664','617','1','1','1291.7','768.911','7.11472','1.55194','0','0','0.700409','0.713742','-120','100','1'),
-- doors
('222022','192642','617','1','1','1350.95','817.2','21.4096','3.15','0','0','0.99627','0.0862864','86400','100','1'),
('222023','192643','617','1','1','1232.65','764.913','21.4729','6.3','0','0','0.0310211','-0.999519','86400','100','1'),
-- waterfall
('222024','194395','617','1','1','1291.6','791.05','7.11','3.22012','0','0','0.999229','-0.0392542','-120','100','1'),
('222025','191877','617','1','1','1291.6','791.05','7.11','3.22012','0','0','0.999229','-0.0392542','-120','100','1');
DELETE FROM gameobject_battleground WHERE guid IN (222020, 222021, 222022, 222023, 222024, 222025);
INSERT INTO gameobject_battleground VALUES
-- buffs
('222020','252','0'),
('222021','252','0'),
-- doors
('222022','254','0'),
('222023','254','0'),
-- waterfall
('222024','250','0'),
('222025','250','0');
DELETE FROM battleground_events WHERE map = 617;
INSERT INTO battleground_events VALUES
('617','252','0','buffs'),
('617','254','0','doors'),
('617','250','0','waterfall');



