UPDATE creature_template SET ScriptName='npc_venture_co_straggler' WHERE entry=27570;

DELETE FROM creature_ai_texts WHERE entry IN(-1603535,-1603536,-1603537,-1603538);
INSERT INTO creature_ai_texts (entry, content_default, COMMENT)VALUES
(-1603535,"No way I'm stickin' around!",'Venture_Co._Straggler'),
(-1603536,"Gotta get out of here!",'Venture_Co._Straggler'),
(-1603537,"Forget this! I'm going home!",'Venture_Co._Straggler'),
(-1603538,"I didn't sign up for this!",'Venture_Co._Straggler');
