-- Instance Ulduar
-- Instance last changes: a630

-- teleporter
UPDATE gameobject_template SET ScriptName = "go_ulduar_teleporter" WHERE entry = 194569;

-- fragments of Val'Anyr
DELETE FROM creature_loot_template WHERE item = 45038;
INSERT INTO creature_loot_template VALUES
(33190, 45038, 6, 2, 1, 1, 0, 0, 0), -- Ignis 6%
(33724, 45038, 4, 2, 1, 1, 0, 0, 0), -- Razorscale %
(33885, 45038, 7, 3, 1, 1, 0, 0, 0), -- XT-002 normal %
(33692, 45038, 4, 3, 1, 1, 0, 0, 0), -- Runemaster %
(33694, 45038, 4, 2, 1, 1, 0, 0, 0), -- Stormcaller %
(33693, 45038, 7, 4, 1, 1, 0, 0, 0), --  Steelbreaker %
(34175, 45038, 6, 2, 1, 1, 0, 0, 0); -- Auriaya %

DELETE FROM gameobject_loot_template WHERE item = 45038;
INSERT INTO gameobject_loot_template VALUES
(26929, 45038, 10, 2, 1, 1, 0, 0, 0), -- Kologarn %
(27079, 45038, 18, 7, 1, 1, 0, 0, 0), -- Freya normal
(26962, 45038, 25, 7, 1, 1, 0, 0, 0), -- Freya hard
(26946, 45038,  7, 5, 1, 1, 0, 0, 0); -- Hodir normal

DELETE FROM reference_loot_template WHERE item = 45038;
INSERT INTO reference_loot_template VALUES
(33886, 45038, 14, 3, 1, 1, 18, 603, 38); -- XT-002 hard %

-- Trash loot
-- 10man
UPDATE `creature_template` SET `lootid` = 33115 WHERE `entry` IN (34183, 34197, 34133, 34193, 33354, 33755, 33754, 33431, 34085, 33430, 34190, 34198, 33526, 34199, 34086, 33525, 33355, 34069, 33527, 34267, 34196, 33699, 33722, 34191, 33818, 33819, 33822, 33824, 34134, 34135, 34273, 34271, 34269);
-- 25man
UPDATE `creature_template` SET `lootid` = 33116 WHERE `entry` IN (33700,33723,34186,34214,34217,34220,34268,34270,34272,34274, 33729,33737,33732,33731,33735,33734,33741,33757,33758,33827,33829,33828,33831,34185,34201,34139,34141,34142,34229,34245,34226,34236,34237);
DELETE FROM `creature_loot_template` WHERE `entry` IN (33115, 33116);
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`groupid`,`mincountOrRef`,`maxcount`,`lootcondition`,`condition_value1`,`condition_value2`) VALUES
(33115, 1, 1, 1, -33115, 1, 0, 0, 0), -- epic item 10man
(33116, 1, 1, 1, -33116, 1, 0, 0, 0); -- epic item 25man

DELETE FROM `reference_loot_template` WHERE entry IN (33115, 33116);
INSERT INTO `reference_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`groupid`,`mincountOrRef`,`maxcount`,`lootcondition`,`condition_value1`,`condition_value2`) VALUES
-- 25man
(33115,46341,0,1,1,1,0,0,0), -- Drape of the Spellweaver
(33115,46347,0,1,1,1,0,0,0), -- Cloak of the Dormant Blaze
(33115,46344,0,1,1,1,0,0,0), -- Iceshear Mantle
(33115,46346,0,1,1,1,0,0,0), -- Boots of Unsettled Prey
(33115,46345,0,1,1,1,0,0,0), -- Bracers of Righteous Reformation
(33115,46340,0,1,1,1,0,0,0), -- Adamant Handguards
(33115,46343,0,1,1,1,0,0,0), -- Fervor of the Protectorate
(33115,46339,0,1,1,1,0,0,0), -- Mimiron's Repeater
(33115,46351,0,1,1,1,0,0,0), -- Bloodcrush Cudgel
(33115,46350,0,1,1,1,0,0,0), -- Pillar of Fortitude
(33115,46342,0,1,1,1,0,0,0), -- Golemheart Longbow
-- 25man
(33116,45540,0,1,1,1,0,0,0), -- Bladebearer's Signet
(33116,45605,0,1,1,1,0,0,0), -- Daschal's Bite
(33116,45542,0,1,1,1,0,0,0), -- Greaves of the Stonewarder
(33116,45548,0,1,1,1,0,0,0), -- Belt of the Sleeper
(33116,45549,0,1,1,1,0,0,0), -- Grips of Chaos
(33116,46138,0,1,1,1,0,0,0), -- Idol of the Flourishing Life
(33116,45544,0,1,1,1,0,0,0), -- Leggings of the Tortured Earth
(33116,45539,0,1,1,1,0,0,0), -- Pendant of Focused Energies
(33116,45543,0,1,1,1,0,0,0), -- Shoulders of Misfortune
(33116,45541,0,1,1,1,0,0,0), -- Shroud of Alteration
(33116,45538,0,1,1,1,0,0,0), -- Titanstone Pendant
(33116,45547,0,1,1,1,0,0,0), -- Relic Hunter's Cord
(33116,37254,0,1,1,1,0,0,0); -- Super Simian Sphere

-- patterns - drop from some bosses only on 25man
-- ignis, xt, auriaya, assembly, kologarn
DELETE FROM `reference_loot_template` WHERE `entry` = 33117;
INSERT INTO `reference_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`groupid`,`mincountOrRef`,`maxcount`,`lootcondition`,`condition_value1`,`condition_value2`) VALUES
(33117,45105,4,6,1,1,0,0,0), -- Pattern: Savior's Slippers
(33117,45104,4,6,1,1,0,0,0), -- Pattern: Cord of the White Dawn
(33117,45103,4,6,1,1,0,0,0), -- Pattern: Spellslinger's Slippers
(33117,45102,4,6,1,1,0,0,0), -- Pattern: Sash of Ancient Power
(33117,45100,4,6,1,1,0,0,0), -- Pattern: Belt of Arctic Life
(33117,45099,4,6,1,1,0,0,0), -- Pattern: Footpads of Silence
(33117,45098,4,6,1,1,0,0,0), -- Pattern: Death-warmed Belt
(33117,45101,4,6,1,1,0,0,0), -- Pattern: Boots of Wintry Endurance
(33117,45095,4,6,1,1,0,0,0), -- Pattern: Boots of Living Scale
(33117,45096,4,6,1,1,0,0,0), -- Pattern: Blue Belt of Chaos
(33117,45094,4,6,1,1,0,0,0), -- Pattern: Belt of Dragons
(33117,45097,4,6,1,1,0,0,0), -- Pattern: Lightning Grounded Boots
(33117,45091,4,6,1,1,0,0,0), -- Plans: Treads of Destiny
(33117,45088,4,6,1,1,0,0,0), -- Plans: Belt of the Titans
(33117,45092,4,6,1,1,0,0,0), -- Plans: Indestructible Plate Girdle
(33117,45093,4,6,1,1,0,0,0), -- Plans: Spiked Deathdealers
(33117,45090,4,6,1,1,0,0,0), -- Plans: Plate Girdle of Righteousness
(33117,45089,4,6,1,1,0,0,0), -- Plans: Battlelord's Plate Boots
(33117,46027,14,6,1,1,0,0,0), -- Formula: Enchant Weapon - Blade Ward (more likely to drop)
(33117,46348,14,6,1,1,0,0,0); -- Formula: Enchant Weapon - Blood Draining (more likely to drop)

-- ****** Ignis the Furnace Master ******
UPDATE `creature_template` SET `ScriptName` = "boss_ignis" WHERE `entry` = 33118;
UPDATE `creature_template` SET lootid = entry, `mechanic_immune_mask` = 617299839 WHERE entry IN (33118, 33190);
UPDATE `creature_template` SET `ScriptName` = "mob_iron_construct" WHERE `entry` = 33121;
UPDATE `creature_template` SET `AIName` = "EventAI" WHERE `entry` = 33221;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 33221;
INSERT INTO `creature_ai_scripts` VALUES
(3322101, 33221, 0, 0, 100, 2, 3000, 3000, 0, 0, 11, 62548, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, "Scorch Trigger - Cast Scorch normal"),
(3322102, 33221, 0, 0, 100, 4, 3000, 3000, 0, 0, 11, 63476, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, "Scorch Trigger - Cast Scorch heroic"),
(3322103, 33221, 4, 0, 100, 6, 0, 0, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, "Scorch Trigger - Disable Combat Movement on Aggro");
-- Ignis loot
DELETE FROM `creature_loot_template` WHERE `entry` IN (33118, 33190);
INSERT INTO `creature_loot_template` VALUES
-- 10 man
(33118, 40753, 100, 0, 1, 1, 0, 0, 0),
(33118, 1, 100, 1, -33119, 2, 0, 0, 0),
-- 25 man
(33190, 45624, 100, 0, 1, 1, 0, 0, 0),
(33190, 1, 100, 1, -33191, 4, 0, 0, 0),
(33190, 45087, 47, 2, 1, 1, 0, 0, 0), -- Runed Orb
(33190, 2, 10, 6, -33117, 1, 0, 0, 0); -- random pattern

DELETE FROM `reference_loot_template` WHERE `entry` IN (33119, 33191);
INSERT INTO `reference_loot_template` VALUES
-- Ignis10
(33119, 45316, 0, 1, 1, 1, 0, 0, 0),
(33119, 45318, 0, 1, 1, 1, 0, 0, 0),
(33119, 45313, 0, 1, 1, 1, 0, 0, 0),
(33119, 45310, 0, 1, 1, 1, 0, 0, 0),
(33119, 45312, 0, 1, 1, 1, 0, 0, 0),
(33119, 45314, 0, 1, 1, 1, 0, 0, 0),
(33119, 45321, 0, 1, 1, 1, 0, 0, 0),
(33119, 45311, 0, 1, 1, 1, 0, 0, 0),
(33119, 45309, 0, 1, 1, 1, 0, 0, 0),
(33119, 45317, 0, 1, 1, 1, 0, 0, 0),
-- Ignis25
(33191, 45166, 0, 1, 1, 1, 0, 0, 0),
(33191, 45157, 0, 1, 1, 1, 0, 0, 0),
(33191, 45162, 0, 1, 1, 1, 0, 0, 0),
(33191, 45185, 0, 1, 1, 1, 0, 0, 0),
(33191, 45161, 0, 1, 1, 1, 0, 0, 0),
(33191, 45158, 0, 1, 1, 1, 0, 0, 0),
(33191, 45164, 0, 1, 1, 1, 0, 0, 0),
(33191, 45171, 0, 1, 1, 1, 0, 0, 0),
(33191, 45167, 0, 1, 1, 1, 0, 0, 0),
(33191, 45168, 0, 1, 1, 1, 0, 0, 0),
(33191, 45170, 0, 1, 1, 1, 0, 0, 0),
(33191, 45186, 0, 1, 1, 1, 0, 0, 0),
(33191, 45169, 0, 1, 1, 1, 0, 0, 0),
(33191, 45165, 0, 1, 1, 1, 0, 0, 0),
(33191, 45187, 0, 1, 1, 1, 0, 0, 0);

-- ****** Razorscale ******
UPDATE creature_template SET `ScriptName`="boss_razorscale" WHERE entry=33186;
UPDATE creature_template SET mechanic_immune_mask=617299803, `lootid` = `entry`, `InhabitType` = 7 WHERE entry IN (33186, 33724);
-- original x=587.547, y= -174.927, z = 391.517; make the boss fly before encounter starts
update creature set position_x = 590.346741, position_y = -226.947647, position_z = 460.897583 where id = 33186;
UPDATE gameobject_template SET flags= 6553648, ScriptName="go_broken_harpoon" WHERE entry = 194565;
-- only 2 harpoons for 10 man
update gameobject set spawnMask = 2 where guid in (73595, 73592);
-- mole machines & adds
UPDATE creature_template SET ScriptName = "mob_mole_machine" WHERE entry = 33245;
UPDATE creature_template SET ScriptName = "mob_dark_rune_watcher" WHERE entry = 33453;
UPDATE creature_template SET ScriptName = "mob_dark_rune_sentinel" WHERE entry = 33846;
UPDATE creature_template SET ScriptName = "mob_dark_rune_guardian" WHERE entry = 33388;
UPDATE creature_template SET ScriptName = "npc_expedition_commander" WHERE entry = 33210;
UPDATE creature_template SET ScriptName = "mob_devouring_flame_target" WHERE entry in (34189, 34188);
UPDATE creature_template SET unit_flags = 0 WHERE `entry` = 33233;
UPDATE `creature_template`SET `unit_flags` = `unit_flags`&~33554432 WHERE entry = 33282;
-- expedition NPCs - don't enter the fight
UPDATE `creature_template` SET `unit_flags` = `unit_flags`|2|33554432, `flags_extra` = `flags_extra`|2 WHERE `entry` IN (33259, 33816, 33287);
-- harpoon script target
DELETE FROM `spell_script_target` WHERE `entry` = 62505;
INSERT INTO `spell_script_target` VALUES
(62505, 1, 33186);
-- loot
DELETE FROM `creature_loot_template` WHERE `entry` IN (33186, 33724);
INSERT INTO `creature_loot_template` VALUES
-- 10 man
(33186, 40753, 100, 0, 1, 1, 0, 0, 0),
(33186, 1, 100, 1, -33187, 2, 0, 0, 0),
-- 25 man
(33724, 45624, 100, 0, 1, 1, 0, 0, 0),
(33724, 1, 100, 1, -33725, 4, 0, 0, 0),
(33724, 45087, 54, 2, 1, 1, 0, 0, 0); -- Runed Orb

DELETE FROM `reference_loot_template` WHERE `entry` IN (33187, 33725);
INSERT INTO `reference_loot_template` VALUES
-- Razorscale10
(33187, 45303, 0, 1, 1, 1, 0, 0, 0),
(33187, 45306, 0, 1, 1, 1, 0, 0, 0),
(33187, 45301, 0, 1, 1, 1, 0, 0, 0),
(33187, 45305, 0, 1, 1, 1, 0, 0, 0),
(33187, 45299, 0, 1, 1, 1, 0, 0, 0),
(33187, 45308, 0, 1, 1, 1, 0, 0, 0),
(33187, 45307, 0, 1, 1, 1, 0, 0, 0),
(33187, 45298, 0, 1, 1, 1, 0, 0, 0),
(33187, 45304, 0, 1, 1, 1, 0, 0, 0),
(33187, 45302, 0, 1, 1, 1, 0, 0, 0),
-- Razorscale25
(33725, 45151, 0, 1, 1, 1, 0, 0, 0),
(33725, 45149, 0, 1, 1, 1, 0, 0, 0),
(33725, 45150, 0, 1, 1, 1, 0, 0, 0),
(33725, 45139, 0, 1, 1, 1, 0, 0, 0),
(33725, 45138, 0, 1, 1, 1, 0, 0, 0),
(33725, 45147, 0, 1, 1, 1, 0, 0, 0),
(33725, 45510, 0, 1, 1, 1, 0, 0, 0),
(33725, 45148, 0, 1, 1, 1, 0, 0, 0),
(33725, 45141, 0, 1, 1, 1, 0, 0, 0),
(33725, 45140, 0, 1, 1, 1, 0, 0, 0),
(33725, 45142, 0, 1, 1, 1, 0, 0, 0),
(33725, 45143, 0, 1, 1, 1, 0, 0, 0),
(33725, 45146, 0, 1, 1, 1, 0, 0, 0),
(33725, 45144, 0, 1, 1, 1, 0, 0, 0),
(33725, 45137, 0, 1, 1, 1, 0, 0, 0);

-- ****** XT-002 Deconstructor ******
UPDATE `creature_template` SET `ScriptName` = "boss_xt_002" WHERE `entry` = 33293;
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299839, `lootid` = `entry` WHERE entry IN (33293, 33885);
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299839 WHERE entry IN (33329, 33995);
UPDATE `creature_template` SET `ScriptName` = "mob_xtheart" WHERE `entry` = 33329;
UPDATE `creature_template` SET `ScriptName` = "mob_pummeler" WHERE `entry` = 33344;
UPDATE `creature_template` SET `ScriptName` = "mob_boombot" WHERE `entry` = 33346;
UPDATE `creature_template` SET `ScriptName` = "mob_voidzone" WHERE `entry` = 34001;
UPDATE `creature_template` SET `ScriptName` = "mob_lifespark" WHERE `entry` = 34004;
UPDATE `creature_template` SET `ScriptName` = "mob_scrap_bot" WHERE `entry` = 33343;

-- adjust the damage of Life Sparks
UPDATE `creature_template` SET mindmg = 478, maxdmg = 530, dmg_multiplier = 22 WHERE entry = 34004;
UPDATE `creature_template` SET mindmg = 478, maxdmg = 530, dmg_multiplier = 30 WHERE entry = 34005;

-- loot
DELETE FROM `creature_loot_template` WHERE `entry` IN (33293, 33885);
INSERT INTO `creature_loot_template` VALUES
-- 10 man normal
(33293, 40753, 100, 0, 1, 1, 0, 0, 0),
(33293, 1, 100, 1, -33294, 2, 0, 0, 0),
-- 10 man hard mode
(33293, 45868, 0, 2, 1, 1, 18, 603, 38),
(33293, 45867, 0, 2, 1, 1, 18, 603, 38),
(33293, 45869, 0, 2, 1, 1, 18, 603, 38),
(33293, 45870, 0, 2, 1, 1, 18, 603, 38),
(33293, 45871, 0, 2, 1, 1, 18, 603, 38),
(33293, 45087, 50, 3, 1, 1, 0, 0, 0), -- Runed Orb
(33293, 3, 10, 6, -33117, 1, 0, 0, 0), -- random pattern
-- 25 man normal
(33885, 45624, 100, 0, 1, 1, 0, 0, 0),
(33885, 1, 100, 1, -33886, 4, 0, 0, 0),
(33885, 45087, 50, 2, 1, 1, 0, 0, 0), -- Runed Orb
(33885, 3, 10, 6, -33117, 1, 0, 0, 0), -- random pattern
-- 25 man hard mode
(33885, 45445, 0, 2, 1, 1, 18, 603, 38),
(33885, 45443, 0, 2, 1, 1, 18, 603, 38),
(33885, 45444, 0, 2, 1, 1, 18, 603, 38),
(33885, 45446, 0, 2, 1, 1, 18, 603, 38),
(33885, 45442, 0, 2, 1, 1, 18, 603, 38);

DELETE FROM `reference_loot_template` WHERE `entry` IN (33294, 33886);
INSERT INTO `reference_loot_template` VALUES
-- XT10normal
(33294, 45680, 0, 1, 1, 1, 0, 0, 0),
(33294, 45694, 0, 1, 1, 1, 0, 0, 0),
(33294, 45676, 0, 1, 1, 1, 0, 0, 0),
(33294, 45679, 0, 1, 1, 1, 0, 0, 0),
(33294, 45687, 0, 1, 1, 1, 0, 0, 0),
(33294, 45685, 0, 1, 1, 1, 0, 0, 0),
(33294, 45686, 0, 1, 1, 1, 0, 0, 0),
(33294, 45677, 0, 1, 1, 1, 0, 0, 0),
(33294, 45682, 0, 1, 1, 1, 0, 0, 0),
(33294, 45675, 0, 1, 1, 1, 0, 0, 0),
-- XT25normal
(33886, 45260, 0, 1, 1, 1, 0, 0, 0),
(33886, 45249, 0, 1, 1, 1, 0, 0, 0),
(33886, 45250, 0, 1, 1, 1, 0, 0, 0),
(33886, 45248, 0, 1, 1, 1, 0, 0, 0),
(33886, 45246, 0, 1, 1, 1, 0, 0, 0),
(33886, 45252, 0, 1, 1, 1, 0, 0, 0),
(33886, 45253, 0, 1, 1, 1, 0, 0, 0),
(33886, 45257, 0, 1, 1, 1, 0, 0, 0),
(33886, 45259, 0, 1, 1, 1, 0, 0, 0),
(33886, 45258, 0, 1, 1, 1, 0, 0, 0),
(33886, 45251, 0, 1, 1, 1, 0, 0, 0),
(33886, 45254, 0, 1, 1, 1, 0, 0, 0),
(33886, 45247, 0, 1, 1, 1, 0, 0, 0),
(33886, 45255, 0, 1, 1, 1, 0, 0, 0),
(33886, 45256, 0, 1, 1, 1, 0, 0, 0);

-- Iron Council (Assembly of Iron)
-- Iron council
UPDATE creature_template SET ScriptName='boss_brundir' WHERE entry = 32857;
UPDATE creature_template SET ScriptName='boss_molgeim' WHERE entry = 32927;
UPDATE creature_template SET ScriptName='boss_steelbreaker' WHERE entry = 32867;
UPDATE creature_template SET ScriptName = 'mob_rune_of_power' WHERE entry = 33705;
UPDATE creature_template SET ScriptName = 'mob_rune_of_summoning' WHERE entry = 33051;
UPDATE creature_template SET ScriptName = 'mob_ulduar_lightning_elemental' WHERE entry = 32958;
UPDATE creature_template SET mechanic_immune_mask = 619397115 WHERE entry IN (32857, 33694, 32927, 33692, 32867, 33693);
UPDATE creature_template SET mechanic_immune_mask = 619395067 WHERE entry IN (32857, 33694); -- Brundir stunnable
-- LOOT
UPDATE creature_template SET lootid = entry WHERE entry IN (32857, 32927, 32867, 33694, 33692, 33693);
DELETE FROM creature_loot_template WHERE entry IN (32857, 32927, 32867, 33694, 33692, 33693);
INSERT INTO creature_loot_template VALUES
-- 10man
-- Brundir
(32857, 40753, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Valor
(32857, 1, 100, 1, -32857, 2, 0, 0, 0),
-- Molgeim
(32927, 40753, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Valor
(32927, 1, 100, 1, -32857, 2, 0, 0, 0),
(32927, 45506, 50, 2, 1, 1, 0, 0, 0), -- Data Disk qitem
-- Steelbreaker
(32867, 40753, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Valor
(32867, 1, 100, 1, -32857, 2, 0, 0, 0),
(32867, 45506, 50, 2, 1, 1, 0, 0, 0), -- Data Disk qitem
(32867, 2, 100, 3, -32867, 1, 0, 0, 0), -- hard mode loot
-- 25man
-- Brundir
(33694, 45624, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Conquest
(33694, 1, 100, 1, -33694, 4, 0, 0, 0),
(33694, 45087, 50, 4, 1, 1, 0, 0, 0), -- Runed Orb
-- Molgeim
(33692, 45624, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Conquest
(33692, 1, 100, 1, -33694, 4, 0, 0, 0),
(33692, 45857, 50, 2, 1, 1, 0, 0, 0), -- Data Disk qitem
(33692, 45087, 100, 4, 2, 3, 0, 0, 0), -- Runed Orb
-- Steelbreaker
(33693, 45624, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Conquest
(33693, 1, 100, 1, -33694, 4, 0, 0, 0),
(33693, 45857, 50, 2, 1, 1, 0, 0, 0), -- Data Disk qitem
(33693, 2, 100, 3, -33693, 1, 0, 0, 0), -- hard mode loot
(33693, 45087, 100, 4, 1, 3, 0, 0, 0); -- Runed Orb

DELETE FROM `reference_loot_template` WHERE `entry` IN (32857, 33694, 32867, 33693);
INSERT INTO `reference_loot_template` VALUES
-- 10man
-- normal mode
(32857, 45333, 0, 1, 1, 1, 0, 0, 0),
(32857, 45378, 0, 1, 1, 1, 0, 0, 0),
(32857, 45329, 0, 1, 1, 1, 0, 0, 0),
(32857, 45322, 0, 1, 1, 1, 0, 0, 0),
(32857, 45330, 0, 1, 1, 1, 0, 0, 0),
(32857, 45418, 0, 1, 1, 1, 0, 0, 0),
(32857, 45324, 0, 1, 1, 1, 0, 0, 0),
(32857, 45331, 0, 1, 1, 1, 0, 0, 0),
(32857, 45423, 0, 1, 1, 1, 0, 0, 0),
(32857, 45332, 0, 1, 1, 1, 0, 0, 0),
-- hard mode
(32867, 45455, 0, 3, 1, 1, 0, 0, 0),
(32867, 45456, 0, 3, 1, 1, 0, 0, 0),
(32867, 45448, 0, 3, 1, 1, 0, 0, 0),
(32867, 45449, 0, 3, 1, 1, 0, 0, 0),
(32867, 45447, 0, 3, 1, 1, 0, 0, 0),
(32867, 45087, 100, 4, 1, 1, 0, 0, 0), -- Runed Orb
(32867, 3, 30, 6, -33117, 1, 0, 0, 0), -- random pattern
-- 25man
(33694, 45226, 0, 1, 1, 1, 0, 0, 0),
(33694, 45224, 0, 1, 1, 1, 0, 0, 0),
(33694, 45228, 0, 1, 1, 1, 0, 0, 0),
(33694, 45193, 0, 1, 1, 1, 0, 0, 0),
(33694, 45227, 0, 1, 1, 1, 0, 0, 0),
(33694, 45238, 0, 1, 1, 1, 0, 0, 0),
(33694, 45237, 0, 1, 1, 1, 0, 0, 0),
(33694, 45235, 0, 1, 1, 1, 0, 0, 0),
(33694, 45240, 0, 1, 1, 1, 0, 0, 0),
(33694, 45234, 0, 1, 1, 1, 0, 0, 0),
(33694, 45232, 0, 1, 1, 1, 0, 0, 0),
(33694, 45239, 0, 1, 1, 1, 0, 0, 0),
(33694, 45225, 0, 1, 1, 1, 0, 0, 0),
(33694, 45233, 0, 1, 1, 1, 0, 0, 0),
(33694, 45236, 0, 1, 1, 1, 0, 0, 0),
(33694, 1, 15, 6, -33117, 1, 0, 0, 0), -- random pattern
-- hard mode
(33693, 45241, 0, 3, 1, 1, 0, 0, 0),
(33693, 45245, 0, 3, 1, 1, 0, 0, 0),
(33693, 45607, 0, 3, 1, 1, 0, 0, 0),
(33693, 45243, 0, 3, 1, 1, 0, 0, 0),
(33693, 45242, 0, 3, 1, 1, 0, 0, 0),
(33693, 45244, 0, 3, 1, 1, 0, 0, 0);

-- Kologarn
-- arms are mounted on the boss
DELETE FROM creature WHERE id IN (32933, 32934);
-- set proper Kologarn position
UPDATE creature SET position_x = 1797.15, position_y = -24.4027, position_z = 448.741 WHERE id = 32930;

UPDATE creature_model_info SET bounding_radius = 40, combat_reach = 40 WHERE modelid IN (28638, 28822, 28821);
UPDATE creature_template SET scriptname='boss_kologarn' WHERE entry = 32930;
UPDATE creature_template SET scriptname='boss_right_arm' WHERE entry=32934;
UPDATE creature_template SET scriptname='boss_left_arm' WHERE entry=32933;
UPDATE creature_template SET ScriptName = 'mob_ulduar_rubble' WHERE entry IN (33768, 33809, 33908, 33942);
UPDATE creature_template SET ScriptName = "mob_eyebeam_trigger" WHERE entry IN (33802, 33632);
UPDATE creature_template SET scriptname = "mob_kologarn_pit_kill_bunny" WHERE entry = 33742;
UPDATE creature SET DeathState = 1 WHERE id = 34297;
UPDATE creature_template SET dmg_multiplier = 0 WHERE entry IN (32934, 32933, 33911, 33910);
UPDATE creature_template SET mechanic_immune_mask = 617299803 WHERE entry IN (32930, 33909, 32934, 32933, 33911, 33910);
UPDATE creature_template SET RegenHealth = 1 WHERE entry = 33910;
UPDATE creature_template SET RegenHealth = 1 WHERE entry = 33911;

-- missing loot 25man
DELETE FROM gameobject_loot_template WHERE entry = 26929;
INSERT INTO gameobject_loot_template VALUES
(26929, 45624, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Conquest
(26929, 45261, 100, 1, -26929, 4, 0, 0, 0), -- 4 items
(26929, 45087, 50, 2, 1, 1, 0, 0, 0), -- Runed Orb
(26929, 1, 10, 6, -33117, 1, 0, 0, 0); -- random pattern
DELETE FROM reference_loot_template WHERE entry = 26929;
INSERT INTO reference_loot_template VALUES
(26929, 45261, 0, 1, 1, 1, 0, 0, 0),
(26929, 45266, 0, 1, 1, 1, 0, 0, 0),
(26929, 45269, 0, 1, 1, 1, 0, 0, 0),
(26929, 45273, 0, 1, 1, 1, 0, 0, 0),
(26929, 45271, 0, 1, 1, 1, 0, 0, 0),
(26929, 45262, 0, 1, 1, 1, 0, 0, 0),
(26929, 45263, 0, 1, 1, 1, 0, 0, 0),
(26929, 45270, 0, 1, 1, 1, 0, 0, 0),
(26929, 45274, 0, 1, 1, 1, 0, 0, 0),
(26929, 45267, 0, 1, 1, 1, 0, 0, 0),
(26929, 45272, 0, 1, 1, 1, 0, 0, 0),
(26929, 45265, 0, 1, 1, 1, 0, 0, 0),
(26929, 45275, 0, 1, 1, 1, 0, 0, 0),
(26929, 45264, 0, 1, 1, 1, 0, 0, 0),
(26929, 45268, 0, 1, 1, 1, 0, 0, 0);

-- Auriaya
UPDATE creature_template SET equipment_id = 103000, scriptname='boss_auriaya' WHERE entry=33515;
UPDATE creature_template SET scriptname='mob_feral_defender' WHERE entry IN (34035, 34171);
UPDATE creature_template SET minlevel=80, maxlevel=80, faction_h=14, faction_a=14, scriptname='mob_seeping_feral_essence' WHERE entry=34098;
UPDATE creature_template SET ScriptName = 'mob_sanctum_sentry' WHERE entry = 34014;
UPDATE `creature_template` SET `mechanic_immune_mask` = 619395071 WHERE `entry` IN (33515, 34175, 34035, 34171);

DELETE FROM creature_equip_template WHERE entry = 103000;
INSERT INTO creature_equip_template values (103000, 45315, 0, 0);

DELETE FROM spell_script_target WHERE entry = 64449;
INSERT INTO spell_script_target VALUES
(64449, 1, 34096);

-- LOOT
UPDATE creature_template SET lootid = entry WHERE entry IN (33515, 34175);
DELETE FROM creature_loot_template WHERE entry IN (33515, 34175);
INSERT INTO creature_loot_template VALUES
-- 10man
(33515, 40753, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Valor
(33515, 1, 100, 1, -33515, 2, 0, 0, 0),
-- 25man
(34175, 45624, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Conquest
(34175, 1, 100, 1, -34175, 4, 0, 0, 0),
(34175, 45087, 50, 2, 1, 1, 0, 0, 0), -- Runed Orb
(34175, 2, 10, 6, -33117, 1, 0, 0, 0); -- random pattern

DELETE FROM `reference_loot_template` WHERE `entry` IN (33515, 34175);
INSERT INTO `reference_loot_template` VALUES
-- 10man
-- normal mode
(33515, 45864, 0, 1, 1, 1, 0, 0, 0),
(33515, 45866, 0, 1, 1, 1, 0, 0, 0),
(33515, 45709, 0, 1, 1, 1, 0, 0, 0),
(33515, 45708, 0, 1, 1, 1, 0, 0, 0),
(33515, 45711, 0, 1, 1, 1, 0, 0, 0),
(33515, 45713, 0, 1, 1, 1, 0, 0, 0),
(33515, 45707, 0, 1, 1, 1, 0, 0, 0),
(33515, 45865, 0, 1, 1, 1, 0, 0, 0),
(33515, 45712, 0, 1, 1, 1, 0, 0, 0),
(33515, 45832, 0, 1, 1, 1, 0, 0, 0),
-- 25man
(34175, 45327, 0, 1, 1, 1, 0, 0, 0),
(34175, 45319, 0, 1, 1, 1, 0, 0, 0),
(34175, 45434, 0, 1, 1, 1, 0, 0, 0),
(34175, 45438, 0, 1, 1, 1, 0, 0, 0),
(34175, 45334, 0, 1, 1, 1, 0, 0, 0),
(34175, 45439, 0, 1, 1, 1, 0, 0, 0),
(34175, 45440, 0, 1, 1, 1, 0, 0, 0),
(34175, 45325, 0, 1, 1, 1, 0, 0, 0),
(34175, 45436, 0, 1, 1, 1, 0, 0, 0),
(34175, 45435, 0, 1, 1, 1, 0, 0, 0),
(34175, 45326, 0, 1, 1, 1, 0, 0, 0),
(34175, 45441, 0, 1, 1, 1, 0, 0, 0),
(34175, 45320, 0, 1, 1, 1, 0, 0, 0),
(34175, 45315, 0, 1, 1, 1, 0, 0, 0),
(34175, 45437, 0, 1, 1, 1, 0, 0, 0);

-- Freya
UPDATE creature_template SET ScriptName = 'boss_freya' WHERE entry = 32906;
UPDATE creature_template SET ScriptName = 'boss_elder_brightleaf' WHERE entry = 32915;
UPDATE creature_template SET ScriptName = 'boss_elder_ironbranch' WHERE entry = 32913;
UPDATE creature_template SET ScriptName = 'boss_elder_stonebark' WHERE entry = 32914;
UPDATE creature_template SET ScriptName = 'mob_iron_roots' WHERE entry IN (33088, 33168);
UPDATE creature_template SET ScriptName = 'mob_freya_ground' WHERE entry IN (33215, 33228, 33170, 33050, 34129);
UPDATE creature_template SET ScriptName = 'mob_freya_spawned' WHERE entry IN (32916, 32919, 33202, 33203, 32918);
UPDATE `creature_template` SET `mechanic_immune_mask` = 619397115 WHERE `entry` IN (32906, 32915, 32913, 32914, 33360, 33392, 33393, 33391);
-- fill some missing data in creature_template (health, level etc.)
-- UPDATE creature_template SET minhealth = 5856900, maxhealth = 5856900 WHERE entry = 33391; -- Brightleaf 25man
-- UPDATE creature_template SET minlevel = 80, maxlevel = 80, faction_A = 14, faction_H = 14, armor = 10673, minhealth = 18270, maxhealth = 18270 WHERE entry = 33168; -- Strengthened Iron Roots 10man
-- UPDATE creature_template SET minlevel = 80, maxlevel = 80, faction_A = 14, faction_H = 14, armor = 10673, minhealth = 54432, maxhealth = 54432 WHERE entry = 33397; -- Strengthened Iron Roots 25man
-- UPDATE creature_template SET minlevel = 81, maxlevel = 81, faction_A = 14, faction_H = 14, armor = 10673, minhealth = 104264, maxhealth = 104264 WHERE entry = 33385; -- Eonar's Gift 25man
-- UPDATE creature_template SET minlevel = 81, maxlevel = 81, faction_A = 14, faction_H = 14, armor = 10673, minhealth = 1563960, maxhealth = 1563960 WHERE entry = 33400; -- Snaplasher 25man
-- UPDATE creature_template SET minlevel = 81, maxlevel = 81, faction_A = 14, faction_H = 14, armor = 10673, minhealth = 1251168, maxhealth = 1251168, minmana = 8162, maxmana = 8162 WHERE entry = 33401; -- Storm Lasher 25man
-- UPDATE creature_template SET minlevel = 81, maxlevel = 81, faction_A = 14, faction_H = 14, armor = 10673, minhealth = 834080, maxhealth = 834080, minmana = 17958, maxmana = 17958 WHERE entry = 33398; -- Ancient Water Spirit 25man
-- UPDATE creature_template SET minlevel = 81, maxlevel = 81, faction_A = 14, faction_H = 14, armor = 10673, minhealth = 3544976, maxhealth = 3544976 WHERE entry = 33376; -- Ancient Conservator 25man
-- UPDATE creature_template SET minlevel = 81, maxlevel = 81, faction_A = 14, faction_H = 14, armor = 10673, minhealth = 375350, maxhealth = 375350 WHERE entry = 33399; -- Detonating Lasher 25man

-- some aura fixes, this may be wrong
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62525', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62524', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62521', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62385', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62387', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62386', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62584', '1', '32906');

DELETE FROM spell_proc_event WHERE entry IN (62664, 64191, 62337);
INSERT INTO spell_proc_event VALUES
(62664, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8|32|128|512|8192|131072, 0, 0, 0, 0),
(64191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8|32|128|512|8192|131072, 0, 0, 0, 0),
(62337, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8|32, 0, 0, 0, 0);

-- Freya's gift spawns
DELETE FROM `gameobject` WHERE `id` IN (194324, 194327, 194328, 194331);
INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(194324, 603, 1, 65535, 2374.876221, -78.512665, 425.073608, 3.260976, 0, 0, 0.706026, 0.708186, -604800, 100, 1), -- 10man normal
(194327, 603, 1, 65535, 2374.876221, -78.512665, 425.073608, 3.260976, 0, 0, 0.706026, 0.708186, -604800, 100, 1), -- 10man hard
(194328, 603, 2, 65535, 2374.876221, -78.512665, 425.073608, 3.260976, 0, 0, 0.706026, 0.708186, -604800, 100, 1), -- 25man normal
(194331, 603, 2, 65535, 2374.876221, -78.512665, 425.073608, 3.260976, 0, 0, 0.706026, 0.708186, -604800, 100, 1); -- 25man hard

UPDATE gameobject_template SET data1 = 26962 WHERE entry = 194328;

-- loot conditions dont work for gameobjects! must find other way
-- Freya Loot
DELETE FROM gameobject_loot_template WHERE entry IN (26962, 27078, 27079, 27081);
INSERT INTO gameobject_loot_template VALUES
-- 10man normal mode
(27078, 40753, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Valor
(27078, 1, 100, 1, -27078, 1, 0, 0, 0), -- 1 random item
(27078, 45644, 0, 2, 1, 1, 0, 0, 0), -- 1 out of 3 tokens
(27078, 45645, 0, 2, 1, 1, 0, 0, 0),
(27078, 45646, 0, 2, 1, 1, 0, 0, 0),
(27078, 2, 100, 3, -27084, 1, 18, 603, 35), -- 1 elder up
(27078, 3, 100, 4, -27085, 1, 18, 603, 36), -- 2 elders up
-- 10man hard mode
(27081, 40753, 100, 0, 4, 4, 0, 0, 0), -- 4 Emblems of Valor
(27081, 1, 100, 1, -27078, 1, 0, 0, 0), -- 1 random item
(27081, 45644, 0, 2, 1, 1, 0, 0, 0), -- 1 out of 3 tokens
(27081, 45645, 0, 2, 1, 1, 0, 0, 0),
(27081, 45646, 0, 2, 1, 1, 0, 0, 0),
(27081, 45087, 100, 4, 1, 1, 0, 0, 0), -- Runed Orb
(27081, 2, 100,  3, -27081, 1, 0, 0, 0), -- item hard mode
-- 25man normal mode
(27079, 45624, 100, 0, 1, 1, 0, 0, 0), -- Emblem of Conquest
(27079, 1, 100, 1, -27083, 2, 0, 0, 0), -- 2 random items
(27079, 3, 100, 3, -27079, 2, 0, 0, 0), -- 2 tokens
(27079, 45087, 75, 2, 1, 1, 0, 0, 0), -- Runed Orb
(27079, 2, 10, 6, -33117, 1, 0, 0, 0), -- random pattern
(27079, 4, 100, 4, -27086, 1, 18, 603, 35), -- 1 elder up
(27079, 5, 100, 5, -27087, 1, 18, 603, 36), -- 2 elders up
-- 25man hard mode
(26962, 45624, 100, 0, 4, 4, 0, 0, 0), -- 4 Emblems of Conquest
(26962, 1, 100, 1, -27083, 2, 0, 0, 0), -- 2 random items
(26962, 3, 100, 3, -27079, 2, 0, 0, 0), -- 2 tokens
(26962, 45087, 90, 2, 1, 4, 0, 0, 0), -- up to 4 Runed Orbs
(26962, 2, 10, 6, -33117, 1, 0, 0, 0), -- random pattern
(26962, 4, 100, 4, -27083, 1, 0, 0, 0); -- item hard mode

DELETE FROM reference_loot_template WHERE entry IN (27078, 27079, 27081, 27082, 27083, 27084, 27085, 27086, 27087);
INSERT INTO reference_loot_template VALUES
(27079, 45653, 0, 3, 1, 1, 0, 0, 0), -- tokens 25man
(27079, 45654, 0, 3, 1, 1, 0, 0, 0),
(27079, 45655, 0, 3, 1, 1, 0, 0, 0),
(27078, 45941, 0, 1, 1, 1, 0, 0, 0), -- items 10man normal
(27078, 45935, 0, 1, 1, 1, 0, 0, 0),
(27078, 45936, 0, 1, 1, 1, 0, 0, 0),
(27078, 45940, 0, 1, 1, 1, 0, 0, 0),
(27078, 45934, 0, 1, 1, 1, 0, 0, 0),
(27081, 45946, 0, 3, 1, 1, 0, 0, 0), -- 10man hard mode
(27081, 45943, 0, 3, 1, 1, 0, 0, 0),
(27081, 45945, 0, 3, 1, 1, 0, 0, 0),
(27081, 45947, 0, 3, 1, 1, 0, 0, 0),
(27081, 45294, 0, 3, 1, 1, 0, 0, 0),
(27082, 45455, 0, 1, 1, 1, 0, 0, 0), -- items 25man normal
(27082, 45483, 0, 1, 1, 1, 0, 0, 0),
(27082, 45481, 0, 1, 1, 1, 0, 0, 0),
(27082, 45482, 0, 1, 1, 1, 0, 0, 0),
(27082, 45480, 0, 1, 1, 1, 0, 0, 0),
(27082, 45479, 0, 1, 1, 1, 0, 0, 0),
(27083, 45484, 0, 1, 1, 1, 0, 0, 0), -- items 25man hard mode
(27083, 45485, 0, 1, 1, 1, 0, 0, 0),
(27083, 45486, 0, 1, 1, 1, 0, 0, 0),
(27083, 45613, 0, 1, 1, 1, 0, 0, 0),
(27083, 45487, 0, 1, 1, 1, 0, 0, 0),
(27083, 45488, 0, 1, 1, 1, 0, 0, 0),
(27084, 40753, 0, 3, 1, 1, 18, 603, 35), -- valor for 1 elder up
(27085, 40753, 0, 4, 1, 1, 18, 603, 36), -- valor for 2 elders up
(27086, 45624, 0, 4, 1, 1, 18, 603, 35), -- conquest for 1 elder up
(27086, 45087, 0, 4, 1, 1, 18, 603, 35), -- Runed Orb for 1 elder up
(27087, 45624, 0, 5, 1, 1, 18, 603, 36), -- conquest for 2 elders up
(27087, 45087, 0, 5, 1, 1, 18, 603, 36); -- Runed Orb for 2 elders up


-- Delete bugged spell from mobs
-- DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (33430,33732) AND `action1_param1` = 63007;

-- Hodir
UPDATE creature_template SET ScriptName = 'boss_hodir' WHERE entry = 32845;
UPDATE `creature_template` SET `mechanic_immune_mask` = 619397115 WHERE `entry` IN (32845, 32846);
UPDATE creature SET phaseMask = 1 WHERE id IN (32845, 32846, 33325, 32901, 32941, 33333, 33328, 32900, 33332, 32950, 32893, 33327, 33331, 32946, 32897, 33326, 32948, 33330);
UPDATE creature_template SET ScriptName = 'mob_toasty_fire' WHERE entry = 33342;
UPDATE creature_template SET ScriptName = 'mob_flashFreeze' WHERE entry IN (32926);
UPDATE creature_template SET ScriptName = 'npc_hodir_priest' WHERE entry IN (32897, 33326, 32948, 33330);
UPDATE creature_template SET ScriptName = 'npc_hodir_druid' WHERE entry IN (33325, 32901, 32941, 33333);
UPDATE creature_template SET ScriptName = 'npc_hodir_shaman' WHERE entry IN (33328, 32900, 33332, 32950);
UPDATE creature_template SET ScriptName = 'npc_hodir_mage' WHERE entry IN (32893, 33327, 33331, 32946);
-- flash freeze that will lock the npcs IN iceblock
UPDATE creature_template SET modelid_1 = 25865, ScriptName = 'mob_npc_flashFreeze' WHERE entry = 32938;
UPDATE creature_template SET modelid_1 = 15880 WHERE `entry` = 33174;
UPDATE creature_template SET ScriptName = 'mob_icicle' WHERE `entry` IN (33169, 33173, 33174);

-- FIXED SOME POSITIONING FOR THE FRIENDLY NPCS, Besides this the freeze aura should also be fixed.
-- fixed npc positioning and added 4 extra flashfreeze for them.
DELETE FROM creature WHERE id IN (32938, 33325, 32901, 32941, 33333, 33328, 32900, 33332, 32950, 32893, 33327, 33331, 32946, 32897, 33326, 32948, 33330);
-- INSERT INTO creature VALUES
-- ();

-- make friendly NPCs stand still
-- UPDATE creature SET MovementType = 0 WHERE id IN (33325, 32901, 32941, 33333, 33328, 32900, 33332, 32950, 32893, 33327, 33331, 32946, 32897, 33326, 32948, 33330);

-- 10 man: 
-- mage
-- UPDATE creature SET position_x = 2000.9, position_y = -231.232 WHERE guid = 131930;
-- priest
-- UPDATE creature SET position_x = 2009.06, position_y = -244.058 WHERE guid = 131933;
-- DELETE FROM creature WHERE guid IN (800005);
-- INSERT INTO creature VALUES
-- (800005, 32897, 603, 3, 128,0,0, 2009.06, -244.058, 432.687, 1.68485, 7200,0,0, 5647, 0, 0, 0);		-- aly priest 
-- shaman
-- UPDATE creature SET position_x = 1983.75, position_y = -243.358 WHERE id = 33328;
-- UPDATE creature SET position_x = 1983.75, position_y = -243.358 WHERE id = 33332;
-- druid
-- UPDATE creature SET position_x = 2021.12, position_y = -236.648 WHERE id = 32941;
-- UPDATE creature SET position_x = 2021.12, position_y = -236.648 WHERE id = 33325;
-- 25 man:
-- druid
-- UPDATE creature SET position_x = 2013.5, position_y = -240.338 WHERE id = 32901;
-- DELETE FROM creature WHERE guid IN (800006);
-- INSERT INTO creature VALUES
-- (800006, 32938, 603, 2, 1,0,0, 2013.5, -240.338, 432.687, 1.68485, 7200,0,0, 5647, 0, 0, 0);
-- shaman:
-- UPDATE creature SET position_x = 2011.48, position_y = -232.79 WHERE id = 32900;
-- UPDATE creature SET position_x = 2011.48, position_y = -232.79 WHERE id = 32950;
-- DELETE FROM creature WHERE guid IN (800007);
-- INSERT INTO creature VALUES
-- (800007, 32938, 603, 2, 1,0,0, 2011.48, -232.79, 432.687, 1.68485, 7200,0,0, 5647, 0, 0, 0);
-- mage:
-- DELETE FROM creature WHERE guid IN (800008, 800010);
-- INSERT INTO creature VALUES
-- (800008, 33327, 603, 2, 128,0,0, 1978.49, -241.476, 432.687, 1.68485, 7200,0,0, 5647, 0, 0, 0), -- aly mage
-- (800010, 32938, 603, 2, 1,0,0, 1978.49, -241.476, 432.687, 1.68485, 7200,0,0, 5647, 0, 0, 0);
-- priest
-- UPDATE creature SET position_x = 1997.88, position_y = -239.394 WHERE id = 33330;
-- DELETE FROM creature WHERE guid IN (800009);
-- INSERT INTO creature VALUES
-- (800009, 32938, 603, 2, 1,0,0, 1997.88, -239.394, 432.687, 1.68485, 7200,0,0, 5647, 0, 0, 0);

-- loot
UPDATE gameobject SET spawnMask = 1 WHERE id = 194307;
UPDATE gameobject SET spawnMask = 2 WHERE id = 194308;

-- 25man loot. 10man already in UDB
DELETE FROM gameobject_loot_template WHERE entry = 26946;
INSERT INTO gameobject_loot_template  VALUES
(26946, 1, 10, 3, -33117, 1, 0, 0, 0),
(26946, 45038, 7, 5, 1, 1, 0, 0, 0),
(26946, 45087, 50, 4, 1, 1, 0, 0, 0),
(26946, 45624, 100, 0, 1, 1, 0, 0, 0),
(26946, 2, 100, 1, -26946, 2, 0, 0, 0),
(26946, 3, 100, 2, -26946, 2, 0, 0, 0);

DELETE FROM reference_loot_template WHERE entry = 26946;
INSERT INTO reference_loot_template VALUES
(26946, 45450, 0, 1, 1, 1, 0, 0, 0),
(26946, 45451, 0, 1, 1, 1, 0, 0, 0),
(26946, 45452, 0, 1, 1, 1, 0, 0, 0),
(26946, 45453, 0, 1, 1, 1, 0, 0, 0),
(26946, 45454, 0, 1, 1, 1, 0, 0, 0),
(26946, 45632, 0, 2, 1, 1, 0, 0, 0),
(26946, 45633, 0, 2, 1, 1, 0, 0, 0),
(26946, 45634, 0, 2, 1, 1, 0, 0, 0);

-- rare cache, despawned as default
UPDATE gameobject SET spawntimesecs = -spawntimesecs WHERE id IN (194200, 194201);
UPDATE gameobject SET spawnMask = 1 WHERE id = 194200;
UPDATE gameobject SET spawnMask = 2 WHERE id = 194201;
