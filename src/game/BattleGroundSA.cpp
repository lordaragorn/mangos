/*
* Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "GameObject.h"
#include "BattleGround.h"
#include "BattleGroundSA.h"
#include "WorldPacket.h"
#include "Language.h"
#include "Player.h"
#include "Object.h"
#include "Creature.h"
#include "BattleGroundMgr.h"
#include "Util.h"
#include "MapManager.h"
#include "ObjectMgr.h"

/*
* BattleGround Strand of the Ancients:
* TODO:
* - Remove all ugly hacks
* - Add proper spawns
* - Add checking of gates when trying to capture a graveyard
* - Fix Seaforium Charges
* - Fix Ram spell
* - Move all the harcode such as coords to header BattleGroundSA.h
*/

BattleGroundSA::BattleGroundSA()
{
    m_BgObjects.resize(BG_SA_MAXOBJ); // boats
    m_tDefender = urand(0,1) ? HORDE : ALLIANCE;
    m_rCurrentRound = BG_SA_ROUND_ONE;
    m_bTimerEnabled = false;
}

// called at first and second round
void BattleGroundSA::PrepareRound()
{
    SetStatus(STATUS_WAIT_JOIN);

    m_tDefender = m_tDefender == HORDE ? ALLIANCE : HORDE;

    uint8 defender_occupier = (m_tDefender == ALLIANCE) ? BG_SA_GRAVE_STATUS_ALLY_OCCUPIED : BG_SA_GRAVE_STATUS_HORDE_OCCUPIED;

    // set up gates
    for (int32 i = 0; i <= BG_SA_GATE_MAX; ++i)
        GateStatus[i] = 1;

    // set up graveyards
    for (uint8 i = 0; i < BG_SA_GRY_MAX; ++i)
    {
        // Despawn all graveyards
        for (uint8 z = 1; z < 5; ++z)
            SpawnEvent(i, z, false);
    }

    // graveyards on beach and near relic
    SpawnEvent(SA_EVENT_ADD_SPIR, m_tDefender == ALLIANCE ? 3 : 4, true);

    // despawn all bombs, vehicles, gates and NPCs
    SpawnEvent(SA_EVENT_ADD_VECH_E, 0, false);
    SpawnEvent(SA_EVENT_ADD_VECH_W, 0, false);
    SpawnEvent(SA_EVENT_ADD_NPC, 0, false);
    SpawnEvent(SA_EVENT_ADD_BOMB, 1, false);
    SpawnEvent(SA_EVENT_ADD_BOMB, 0, false);
    SpawnEvent(SA_EVENT_ADD_GO, 0, false);

    // spawn gameobjects that should always be visible to all (flagpole, portals, gates)
    SpawnEvent(SA_EVENT_ADD_GO, 0, true);

    // set time_to_start_battle events texts
    m_StartMessageIds[BG_STARTING_EVENT_FIRST] = LANG_BG_SA_START_TWO_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_SA_START_ONE_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_THIRD] = LANG_BG_SA_START_HALF_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_SA_HAS_BEGUN;

    // used in vendor script?
    //SetDemolisherDestroyed(HORDE, false);
    //SetDemolisherDestroyed(ALLIANCE, false);

    // timers etc.
    m_bRelicGateDestroyed = false;
    m_bShipsStarted = false;
    m_uiShipsTimer = BG_SA_BOAT_START;

    if (GetCurrentRound() == BG_SA_ROUND_TWO)
    {
        // in round 2 previous attackers must defend for the time it took them to capture the relic
        // if they didnt capture it, then they defend for the full round duration
        m_uiRoundElapsedTimer = (RoundScores[0].winner == m_tDefender) ? (BG_SA_ROUNDLENGTH - RoundScores[0].time) : 0;
    }

    // gates status
    GateStatus[BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE] = BG_SA_GO_GATES_NORMAL;
    GateStatus[BG_SA_GO_GATES_T_GREEN_EMERALD] = BG_SA_GO_GATES_NORMAL;
    GateStatus[BG_SA_GO_GATES_T_BLUE_SAPHIRE] = BG_SA_GO_GATES_NORMAL;
    GateStatus[BG_SA_GO_GATES_T_MAUVE_AMETHYST] = BG_SA_GO_GATES_NORMAL;
    GateStatus[BG_SA_GO_GATES_T_RED_SUN] = BG_SA_GO_GATES_NORMAL;
    GateStatus[BG_SA_GO_GATES_T_YELLOW_MOON] = BG_SA_GO_GATES_NORMAL;

    UpdateWorldStates();
}

void BattleGroundSA::ResetBattle(uint32 winner, Team teamDefending)
{
    m_rCurrentRound = BG_SA_ROUND_TWO;

    // reset all events including doors opening
    m_Events = 0;

    m_StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_2M;
    m_StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_1M;
    m_StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_30S;
    m_StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;

    SetupShips();
    SetStartTime(0);
    ToggleTimer();
    PrepareRound();

    for (BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if (Player *plr = sObjectMgr.GetPlayer(itr->first))
            TeleportPlayerToCorrectLoc(plr, true);
    }
    UpdateWorldStates();
}

void BattleGroundSA::Reset()
{
    //call parent's class reset
    BattleGround::Reset();

    PrepareRound();
}

bool BattleGroundSA::SetupBattleGround()
{
    return SetupShips();
}

bool BattleGroundSA::SetupShips()
{
    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
        for (BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player *plr = sObjectMgr.GetPlayer(itr->first))
                SendTransportsRemove(plr);
    for (uint8 i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
    {
        uint32 boatid=0;
        switch (i)
        {
            case BG_SA_BOAT_ONE:
                boatid = m_tDefender == ALLIANCE ? BG_SA_BOAT_ONE_H : BG_SA_BOAT_ONE_A;
                break;
            case BG_SA_BOAT_TWO:
                boatid = m_tDefender == ALLIANCE ? BG_SA_BOAT_TWO_H : BG_SA_BOAT_TWO_A;
                break;
        }
        if (!(AddObject(i, boatid, BG_SA_START_LOCATIONS[i + 5][0], BG_SA_START_LOCATIONS[i + 5][1], BG_SA_START_LOCATIONS[i + 5][2]+ (m_tDefender == ALLIANCE ? -3.750f: 0) , BG_SA_START_LOCATIONS[i + 5][3], 0, 0, 0, 0, RESPAWN_ONE_DAY)))
        {
            sLog.outError("SA_ERROR: Can't spawn ships!");
            return false;
        }
    }

    GetBGObject(BG_SA_BOAT_ONE)->UpdateRotationFields(1.0f, 0.0002f);
    GetBGObject(BG_SA_BOAT_TWO)->UpdateRotationFields(1.0f, 0.00001f);
    SpawnBGObject(m_BgObjects[BG_SA_BOAT_ONE], RESPAWN_IMMEDIATELY);
    SpawnBGObject(m_BgObjects[BG_SA_BOAT_TWO], RESPAWN_IMMEDIATELY);

    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
        for (BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player *plr = sObjectMgr.GetPlayer(itr->first))
                SendTransportInit(plr);
    return true;
}

void BattleGroundSA::FillInitialWorldStates(WorldPacket& data, uint32& count)
{
    for (uint8 i = 0; i < BG_SA_GRY_MAX; ++i)
        _GydOccupied(i, m_Gyd[i] == BG_SA_GRAVE_STATUS_HORDE_OCCUPIED ? HORDE : ALLIANCE);

    // Graveyard near relic
    _GydOccupied(3, m_tDefender);

    for (uint32 z = 0; z <= BG_SA_GATE_MAX; ++z)
        FillInitialWorldState(data, count, BG_SA_GateStatus[z], GateStatus[z]);

    //Time will be sent on first update...
    FillInitialWorldState(data, count, BG_SA_ENABLE_TIMER, m_bTimerEnabled);
    FillInitialWorldState(data, count, BG_SA_TIMER_MINUTES, 0);
    FillInitialWorldState(data, count, BG_SA_TIMER_10SEC, 0);
    FillInitialWorldState(data, count, BG_SA_TIMER_SEC, 0);
}

void BattleGroundSA::StartShips()
{
    if (m_bShipsStarted)
        return;

    DoorOpen(m_BgObjects[BG_SA_BOAT_ONE]);
    DoorOpen(m_BgObjects[BG_SA_BOAT_TWO]);

    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
    {
        for (BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end();itr++)
        {
            if (Player* p = sObjectMgr.GetPlayer(itr->first))
            {
                UpdateData data;
                WorldPacket pkt;
                GetBGObject(i)->BuildValuesUpdateBlockForPlayer(&data, p);
                data.BuildPacket(&pkt);
                p->GetSession()->SendPacket(&pkt);
            }
        }
    }
    m_bShipsStarted = true;
}

void BattleGroundSA::ToggleTimer()
{
    m_bTimerEnabled = !m_bTimerEnabled;
    UpdateWorldState(BG_SA_ENABLE_TIMER, m_bTimerEnabled);
}

void BattleGroundSA::EndBattleGround(Team winner)
{
    if (RoundScores[0].time == RoundScores[1].time) // Noone got in time
        winner = TEAM_NONE;
    else if (RoundScores[0].time < RoundScores[1].time)
        winner = RoundScores[0].winner == ALLIANCE ? ALLIANCE : HORDE;
    else
        winner = RoundScores[1].winner == ALLIANCE ? ALLIANCE : HORDE;

    //win reward
    if(winner)
    {
        RewardHonorToTeam(GetBonusHonorFromKill(1), winner);
        RewardXpToTeam(0, 0.8f, winner);
    }

    //complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(2), ALLIANCE);
    RewardHonorToTeam(GetBonusHonorFromKill(2), HORDE);
    RewardXpToTeam(0, 0.8f, ALLIANCE);
    RewardXpToTeam(0, 0.8f, HORDE);
    
    BattleGround::EndBattleGround(winner);
}

void BattleGroundSA::Update(uint32 diff)
{
    BattleGround::Update(diff);

    if (!GetPlayersSize())
        return;

    if (GetStatus() == STATUS_WAIT_JOIN)
    {
        if (!m_bShipsStarted)
        {
            // Round has not started yet, countdown to launch boats
            if (m_uiShipsTimer <= diff)
            {
                StartShips();
            }else m_uiShipsTimer -= diff;
            return;
        }
    }

    if (GetStatus() == STATUS_IN_PROGRESS) // Battleground already in progress
    {
        if (m_uiRoundElapsedTimer >= BG_SA_ROUNDLENGTH)
        {
            if(GetCurrentRound() == BG_SA_ROUND_ONE) // Timeout of first round
            {
                PlaySoundToAll(BG_SA_SOUND_GYD_VICTORY);
                SendMessageToAll(m_tDefender == ALLIANCE ? LANG_BG_SA_ALLIANCE_TIMEOUT_END_1ROUND : LANG_BG_SA_HORDE_TIMEOUT_END_1ROUND, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                RoundScores[0].winner = m_tDefender;
                RoundScores[0].time = m_uiRoundElapsedTimer;
                ResetBattle(0, m_tDefender);
            }
            else // Timeout of second round
            {
                SendMessageToAll(m_tDefender == ALLIANCE ? LANG_BG_SA_ALLIANCE_TIMEOUT_END_2ROUND : LANG_BG_SA_HORDE_TIMEOUT_END_2ROUND, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                RoundScores[1].winner = m_tDefender;
                RoundScores[1].time = m_uiRoundElapsedTimer;
                EndBattleGround(TEAM_NONE);
                return;
            }
        }
        else
            m_uiRoundElapsedTimer += diff;

        for (int gyd = 0; gyd < BG_SA_GRY_MAX; ++gyd)
        {
            // 3 sec delay to spawn new banner instead previous despawned one
            if (m_BannerTimers[gyd].timer)
            {
                if (m_BannerTimers[gyd].timer > diff)
                    m_BannerTimers[gyd].timer -= diff;
                else
                {
                    m_BannerTimers[gyd].timer = 0;
                    _CreateBanner(gyd, m_BannerTimers[gyd].type, m_BannerTimers[gyd].teamIndex, false);
                }
            }
            // 1-minute to occupy a node from contested state
            if (m_GydTimers[gyd])
            {
                if (m_GydTimers[gyd] > diff)
                    m_GydTimers[gyd] -= diff;
                else
                {
                    m_GydTimers[gyd] = 0;
                    // Change from contested to occupied !
                    uint8 teamIndex = m_Gyd[gyd]-1;
                    m_prevGyd[gyd] = m_Gyd[gyd];
                    m_Gyd[gyd] += 2;
                    // create new occupied banner
                    _CreateBanner(gyd, BG_SA_GRAVE_TYPE_OCCUPIED, teamIndex, true);
                    //_SendNodeUpdate(node);
                    _GydOccupied(gyd,(teamIndex == 0) ? ALLIANCE:HORDE);
                    // Message to chatlog
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardXpToTeam(0, 0.6f, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    switch(gyd)
                    {
                        case 0: SpawnEvent(SA_EVENT_ADD_VECH_W, 0, true);break;
                        case 1: SpawnEvent(SA_EVENT_ADD_VECH_E, 0, true);break;
                    }
                    if (teamIndex == 0)
                    {
                        // SendMessage2ToAll(LANG_BG_SA_AH_SEIZES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE,NULL,LANG_BG_ALLY,_GydName(gyd));
                        PlaySoundToAll(BG_SA_SOUND_GYD_CAPTURED_ALLIANCE);
                        SendWarningToAllSA(gyd, STATUS_CONQUESTED, ALLIANCE);
                    }
                    else
                    {
                        // SendMessage2ToAll(LANG_BG_SA_AH_SEIZES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE,NULL,LANG_BG_HORDE,_GydName(gyd));
                        PlaySoundToAll(BG_SA_SOUND_GYD_CAPTURED_HORDE);
                        SendWarningToAllSA(gyd, STATUS_CONQUESTED, HORDE);
                    }
                }
            }
        }
        UpdateTimer();
    }
}

void BattleGroundSA::UpdateWorldStates()
{
    UpdateWorldState(BG_SA_ENABLE_TIMER, m_bTimerEnabled);

    UpdateWorldState(BG_SA_RIGHT_GY_HORDE , m_tDefender == HORDE);
    UpdateWorldState(BG_SA_LEFT_GY_HORDE , m_tDefender == HORDE);
    UpdateWorldState(BG_SA_CENTER_GY_HORDE , m_tDefender == HORDE);

    UpdateWorldState(BG_SA_RIGHT_GY_ALLIANCE , m_tDefender == ALLIANCE);
    UpdateWorldState(BG_SA_LEFT_GY_ALLIANCE , m_tDefender == ALLIANCE);
    UpdateWorldState(BG_SA_CENTER_GY_ALLIANCE , m_tDefender == ALLIANCE);

    UpdateWorldState(BG_SA_ALLY_ATTACKS, m_tDefender == HORDE);
    UpdateWorldState(BG_SA_HORDE_ATTACKS, m_tDefender != HORDE);

    UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_ALL, m_tDefender == HORDE);
    UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_ALL, m_tDefender == HORDE);
    UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_HRD, m_tDefender != HORDE);
    UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_HRD, m_tDefender != HORDE);

    UpdateWorldState(BG_SA_HORDE_DEFENCE_TOKEN, m_tDefender == HORDE);
    UpdateWorldState(BG_SA_ALLIANCE_DEFENCE_TOKEN, m_tDefender != HORDE);

    UpdateWorldState(BG_SA_PURPLE_GATEWS, 1);
    UpdateWorldState(BG_SA_RED_GATEWS, 1);
    UpdateWorldState(BG_SA_BLUE_GATEWS, 1);
    UpdateWorldState(BG_SA_GREEN_GATEWS, 1);
    UpdateWorldState(BG_SA_YELLOW_GATEWS, 1);
    UpdateWorldState(BG_SA_ANCIENT_GATEWS, 1);

    UpdateWorldState(BG_SA_GateStatus[BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE], GateStatus[BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE]);
    UpdateWorldState(BG_SA_GateStatus[BG_SA_GO_GATES_T_GREEN_EMERALD], GateStatus[BG_SA_GO_GATES_T_GREEN_EMERALD]);
    UpdateWorldState(BG_SA_GateStatus[BG_SA_GO_GATES_T_BLUE_SAPHIRE], GateStatus[BG_SA_GO_GATES_T_BLUE_SAPHIRE]);
    UpdateWorldState(BG_SA_GateStatus[BG_SA_GO_GATES_T_MAUVE_AMETHYST], GateStatus[BG_SA_GO_GATES_T_MAUVE_AMETHYST]);
    UpdateWorldState(BG_SA_GateStatus[BG_SA_GO_GATES_T_RED_SUN], GateStatus[BG_SA_GO_GATES_T_RED_SUN]);
    UpdateWorldState(BG_SA_GateStatus[BG_SA_GO_GATES_T_YELLOW_MOON], GateStatus[BG_SA_GO_GATES_T_YELLOW_MOON]);
}

void BattleGroundSA::UpdateTimer()
{
    uint32 end_of_round = BG_SA_ROUNDLENGTH - m_uiRoundElapsedTimer;
    if (end_of_round < 0)
        end_of_round = 0;
    UpdateWorldState(BG_SA_TIMER_MINUTES, end_of_round/60000);
    UpdateWorldState(BG_SA_TIMER_10SEC, (end_of_round%60000)/10000);
    UpdateWorldState(BG_SA_TIMER_SEC, ((end_of_round%60000)%10000)/1000);
}

void BattleGroundSA::StartingEventCloseDoors() {}

void BattleGroundSA::StartingEventOpenDoors()
{
    uint8 defender_occupier = (m_tDefender == ALLIANCE) ? BG_SA_GRAVE_STATUS_ALLY_OCCUPIED : BG_SA_GRAVE_STATUS_HORDE_OCCUPIED;

    SpawnEvent(SA_EVENT_ADD_NPC, 0, true);  // spawn NPCs (vehicles)
    SpawnEvent(SA_EVENT_ADD_BOMB, m_tDefender == ALLIANCE ? 1 : 0, true); // spawn seaforium charges

    // set up graveyards
    for (uint8 i = 0; i < BG_SA_GRY_MAX; ++i)
    {
        // spawn proper graveyard banner
        SpawnEvent(i, defender_occupier, true);

        m_prevGyd[i] = 0;
        m_GydTimers[i] = 0;
        m_BannerTimers[i].timer = 0;
        m_Gyd[i] = defender_occupier;
        m_ActiveEvents[i] = defender_occupier;
        _GydOccupied(i, m_tDefender);
    }

    ToggleTimer();
    UpdateWorldStates();
}

void BattleGroundSA::RemovePlayer(Player* /*plr*/, ObjectGuid /*guid*/) {}

void BattleGroundSA::AddPlayer(Player *plr)
{
    BattleGround::AddPlayer(plr);

    TeleportPlayerToCorrectLoc(plr);

    BattleGroundSAScore* sc = new BattleGroundSAScore;
    m_PlayerScores[plr->GetObjectGuid()] = sc;

    UpdateWorldStates();
}

void BattleGroundSA::HandleAreaTrigger(Player * /*Source*/, uint32 /*Trigger*/)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
}

void BattleGroundSA::UpdatePlayerScore(Player* Source, uint32 type, uint32 value)
{
    BattleGroundScoreMap::iterator itr = m_PlayerScores.find(Source->GetGUID());
    if(itr == m_PlayerScores.end()) // player not found...
        return;

    switch(type)
    {
        case SCORE_DEMOLISHERS_DESTROYED:
            ((BattleGroundSAScore*)itr->second)->DemolishersDestroyed += value;
            break;
        case SCORE_GATES_DESTROYED:
            ((BattleGroundSAScore*)itr->second)->GatesDestroyed += value;
            break;
        default:
            BattleGround::UpdatePlayerScore(Source,type,value);
            break;
    }
}

/* type: 0-neutral, 1-contested, 3-occupied
team: 0-ally, 1-horde */
void BattleGroundSA::_CreateBanner(uint8 gry, uint8 type, uint8 teamIndex, bool delay)
{
    // Just put it into the queue
    if (delay)
    {
        m_BannerTimers[gry].timer = 2000;
        m_BannerTimers[gry].type = type;
        m_BannerTimers[gry].teamIndex = teamIndex;
        return;
    }

    // cause the node-type is in the generic form
    // please see in the headerfile for the ids
    type += teamIndex;

    SpawnEvent(gry, type, true); // will automaticly despawn other events
}

void BattleGroundSA::EventPlayerClickedOnFlag(Player *source, GameObject* target_obj)
{
    // GetDBTableGUIDLow())).event1;
    uint8 event = (sBattleGroundMgr.GetGameObjectEventIndex(target_obj->GetGUIDLow())).event1;

    if (event >= BG_SA_GRY_MAX) // not a node
        return;

    BG_SA_GraveYard gyd = BG_SA_GraveYard(event);
    BattleGroundTeamIndex teamIndex = GetTeamIndexByTeamId(source->GetTeam());

    // Check if player really could use this banner, not cheated
    if (!(m_Gyd[gyd] == 0 || teamIndex == m_Gyd[gyd] % 2))
        return;

    // check gates status for E, W and S graveyards
    if ((gyd == BG_SA_GRAVE_E || gyd == BG_SA_GRAVE_W) &&
        GateStatus[BG_SA_GO_GATES_T_GREEN_EMERALD] != BG_SA_GO_GATES_DESTROY &&
        GateStatus[BG_SA_GO_GATES_T_BLUE_SAPHIRE] != BG_SA_GO_GATES_DESTROY
        ||
        gyd == BG_SA_GRAVE_S && GateStatus[BG_SA_GO_GATES_T_MAUVE_AMETHYST] != BG_SA_GO_GATES_DESTROY &&
        GateStatus[BG_SA_GO_GATES_T_RED_SUN] != BG_SA_GO_GATES_DESTROY)
        return;

    uint32 sound = 0;

    if ((m_Gyd[gyd] == BG_SA_GRAVE_STATUS_ALLY_CONTESTED) || (m_Gyd[gyd] == BG_SA_GRAVE_STATUS_HORDE_CONTESTED))
    {
        // If last state is NOT occupied, change node to enemy-contested
        if (m_prevGyd[gyd] < BG_SA_GRAVE_TYPE_OCCUPIED)
        {
            //UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_prevGyd[gyd] = m_Gyd[gyd];
            m_Gyd[gyd] = teamIndex + BG_SA_GRAVE_TYPE_CONTESTED;
            // create new contested banner
            _CreateBanner(gyd, BG_SA_GRAVE_TYPE_CONTESTED, teamIndex, true);
            //_SendNodeUpdate(node);
            m_GydTimers[gyd] = BG_SA_FLAG_CAPTURING_TIME;

            if (teamIndex == BG_TEAM_ALLIANCE)
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, LANG_BG_ALLY, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, ALLIANCE);
            }
            else
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE, source, LANG_BG_HORDE, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, HORDE);
            }
        }
        // If contested, change back to occupied
        else
        {
            UpdatePlayerScore(source, SCORE_BASES_DEFENDED, 1);
            m_prevGyd[gyd] = m_Gyd[gyd];
            m_Gyd[gyd] = teamIndex + BG_SA_GRAVE_TYPE_OCCUPIED;
            // create new occupied banner
            _CreateBanner(gyd, BG_SA_GRAVE_TYPE_OCCUPIED, teamIndex, true);
            //_SendNodeUpdate(node);
            m_GydTimers[gyd] = 0;
            //_NodeOccupied(node,(teamIndex == BG_TEAM_ALLIANCE) ? ALLIANCE:HORDE);

            if (teamIndex == BG_TEAM_ALLIANCE)
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, LANG_BG_ALLY, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, ALLIANCE);
            }
            else
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE, source, LANG_BG_HORDE, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, HORDE);
            }
        }
        sound = (teamIndex == BG_TEAM_ALLIANCE) ? BG_SA_SOUND_GYD_ASSAULTED_ALLIANCE : BG_SA_SOUND_GYD_ASSAULTED_HORDE;
    }
    // If node is occupied, change to enemy-contested
    else if (m_tDefender == HORDE)
    {
        if (m_Gyd[gyd] == BG_SA_GRAVE_STATUS_HORDE_OCCUPIED)
        {
            //UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_prevGyd[gyd] = m_Gyd[gyd];
            m_Gyd[gyd] = teamIndex + BG_SA_GRAVE_TYPE_CONTESTED;
            // create new contested banner
            _CreateBanner(gyd, BG_SA_GRAVE_TYPE_CONTESTED, teamIndex, true);
            //_SendNodeUpdate(node);
            m_GydTimers[gyd] = BG_SA_FLAG_CAPTURING_TIME;

            // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, LANG_BG_ALLY, _GydName(gyd));
            SendWarningToAllSA(gyd, STATUS_CLAIMED, ALLIANCE);

            sound = BG_SA_SOUND_GYD_ASSAULTED_ALLIANCE;
        }
        else
            return;
    }
    else if (m_tDefender == ALLIANCE)
    {
        if (m_Gyd[gyd] == BG_SA_GRAVE_STATUS_ALLY_OCCUPIED)
        {
            //UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_prevGyd[gyd] = m_Gyd[gyd];
            m_Gyd[gyd] = teamIndex + BG_SA_GRAVE_TYPE_CONTESTED;
            // create new contested banner
            _CreateBanner(gyd, BG_SA_GRAVE_TYPE_CONTESTED, teamIndex, true);
            //_SendNodeUpdate(node);
            m_GydTimers[gyd] = BG_SA_FLAG_CAPTURING_TIME;

            // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE, source, LANG_BG_HORDE, _GydName(gyd));
            SendWarningToAllSA(gyd, STATUS_CLAIMED, HORDE);

            sound = BG_SA_SOUND_GYD_ASSAULTED_HORDE;
        }
        else
            return;
    }
    PlaySoundToAll(sound);
}

void BattleGroundSA::EventSpawnGOSA(Player *owner, GameObject* obj, float x, float y, float z)
{
    SendMessageToAll(LANG_BG_SA_INSTALL_BOMB, (m_tDefender == ALLIANCE) ? CHAT_MSG_BG_SYSTEM_HORDE : CHAT_MSG_BG_SYSTEM_ALLIANCE , owner);
}

void BattleGroundSA::SendMessageSA(Player *player, uint32 type, uint32 name)
{
    uint32 entryMSG = 0;
    BattleGroundTeamIndex teamIndex = GetTeamIndexByTeamId(player->GetTeam());
    switch (type)
    {
        case 0: entryMSG = LANG_BG_SA_GATE_ATTACK; break;
        case 1: entryMSG = LANG_BG_SA_GATE_DAMAGE; break;
        case 2: entryMSG = LANG_BG_SA_GATE_DETROYED; break;
    }
    if (teamIndex == BG_TEAM_ALLIANCE)
        SendMessage2ToAll(entryMSG,CHAT_MSG_BG_SYSTEM_ALLIANCE, player, name);
    else
        SendMessage2ToAll(entryMSG,CHAT_MSG_BG_SYSTEM_HORDE, player, name);
}

void BattleGroundSA::EventPlayerDamageGO(Player *player, GameObject* target_obj, uint32 eventId)
{
    BattleGroundTeamIndex teamIndex = GetTeamIndexByTeamId(player->GetTeam());

    uint32 type = NULL;
    switch (target_obj->GetEntry())
    {
        case BG_SA_GO_GATES_ROOM_ANCIENT_SHRINE:
        {
            type = BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19836:
                    SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19837:
                    SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(100, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 75, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_GREEN_EMERALD:
        {
            type = BG_SA_GO_GATES_T_GREEN_EMERALD;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19041:
                    SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19046:
                    SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_BLUE_SAPHIRE:
        {
            type = BG_SA_GO_GATES_T_BLUE_SAPHIRE;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19040:
                    SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19045:
                    SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_MAUVE_AMETHYST:
        {
            type = BG_SA_GO_GATES_T_MAUVE_AMETHYST;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19043:
                    SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19048:
                    SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_RED_SUN:
        {
            type = BG_SA_GO_GATES_T_RED_SUN;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19042:
                    SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19047:
                    SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_YELLOW_MOON:
        {
            type = BG_SA_GO_GATES_T_YELLOW_MOON;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19044:
                    SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19049:
                    SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_TITAN_RELIC:
        {
            if (eventId == 22097 && player->GetTeam() != m_tDefender)
            {
                if(GateStatus[BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE] != BG_SA_GO_GATES_DESTROY)
                {
                    sLog.outError("Player %s has clicked SOTA Relic without Relic gate being destroyed", player->GetName());
                    return;
                }
                if(GetCurrentRound() == BG_SA_ROUND_ONE) // Victory at first round
                {
                    RoundScores[0].winner = m_tDefender == ALLIANCE ? HORDE : ALLIANCE;
                    RoundScores[0].time = m_uiRoundElapsedTimer;
                    PlaySoundToAll(BG_SA_SOUND_GYD_VICTORY);
                    SendMessageToAll(m_tDefender == HORDE ? LANG_BG_SA_ALLIANCE_END_1ROUND : LANG_BG_SA_HORDE_END_1ROUND, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                    RewardHonorToTeam(150, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 100, (teamIndex == 0) ? ALLIANCE:HORDE);
                    ResetBattle(player->GetTeam(), m_tDefender);
                }
                else // Victory at second round
                {
                    RoundScores[1].winner = m_tDefender == ALLIANCE ? HORDE : ALLIANCE;
                    RoundScores[1].time = m_uiRoundElapsedTimer;
                    SendMessageToAll(m_tDefender == HORDE ? LANG_BG_SA_ALLIANCE_END_2ROUND : LANG_BG_SA_HORDE_END_2ROUND, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                    RewardHonorToTeam(150, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 100, (teamIndex == 0) ? ALLIANCE:HORDE);
                    EndBattleGround(player->GetTeam());
                }
            }
            break;
        }
    }
}

void BattleGroundSA::HandleKillUnit(Creature* unit, Player* killer)
{
    if(!unit)
        return;

    if(unit->GetEntry() == 28781) //Demolisher
    {
        UpdatePlayerScore(killer, SCORE_DEMOLISHERS_DESTROYED, 1);
        //SetDemolisherDestroyed(killer->GetTeam() == HORDE ? ALLIANCE : HORDE, true);
    }
}

int32 BattleGroundSA::_GatesName(GameObject* obj)
{
    if (!obj)
        return 0;
    switch (obj->GetEntry())
    {
        case BG_SA_GO_GATES_ROOM_ANCIENT_SHRINE: return LANG_BG_SA_GATE_ROOM_ANCIENT_SHRINE;
        case BG_SA_GO_GATES_GREEN_EMERALD: return LANG_BG_SA_GATE_GREEN_EMERALD;
        case BG_SA_GO_GATES_BLUE_SAPHIRE: return LANG_BG_SA_GATE_BLUE_SAPHIRE;
        case BG_SA_GO_GATES_MAUVE_AMETHYST: return LANG_BG_SA_GATE_MAUVE_AMETHYST;
        case BG_SA_GO_GATES_RED_SUN: return LANG_BG_SA_GATE_RED_SUN_;
        case BG_SA_GO_GATES_YELLOW_MOON: return LANG_BG_SA_GATE_YELLOW_MOON;
        default:
            MANGOS_ASSERT(0);
    }
    return 0;
}

int32 BattleGroundSA::_GydName(uint8 gyd)
{
    switch (gyd)
    {
        case 0: return LANG_BG_SA_EAST_GRAVEYARD;
        case 1: return LANG_BG_SA_WEST_GRAVEYARD;
        case 2: return LANG_BG_SA_SOUTH_GRAVEYARD;
        default:
            MANGOS_ASSERT(0);
    }
    return 0;
}

WorldSafeLocsEntry const* BattleGroundSA::GetClosestGraveYard(Player* player)
{
    BattleGroundTeamIndex teamIndex = GetTeamIndexByTeamId(player->GetTeam());

    // Is there any occupied node for this team?
    std::vector<uint8> gyd;
    for (uint8 i = 0; i < BG_SA_GRY_MAX; ++i)
        if (m_Gyd[i] == teamIndex + 3)
            gyd.push_back(i);

    WorldSafeLocsEntry const* good_entry = NULL;
    // If so, select the closest node to place ghost on
    if (!gyd.empty())
    {
        float plr_x = player->GetPositionX();
        float plr_y = player->GetPositionY();

        float mindist = 999999.0f;
        for (uint8 i = 0; i < gyd.size(); ++i)
        {
            WorldSafeLocsEntry const*entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIdsPhase[gyd[i]]);
            if (!entry)
                continue;
            float dist = (entry->x - plr_x)*(entry->x - plr_x)+(entry->y - plr_y)*(entry->y - plr_y);
            if (mindist > dist)
            {
                mindist = dist;
                good_entry = entry;
            }
        }
        gyd.clear();
    }
    // If not, place ghost on starting location
    if (!good_entry)
    {
        if (m_tDefender == HORDE)
        {
            if (teamIndex == 0)
                good_entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIds[1]);
            else
                good_entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIds[0]);
        }
        if (m_tDefender == ALLIANCE)
        {
            if (teamIndex == 0)
                good_entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIds[0]);
            else
                good_entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIds[1]);
        }
    }
    return good_entry;
}

void BattleGroundSA::_GydOccupied(uint8 node, Team team)
{
    if (node >= 0 && node < 3)
    {
        UpdateWorldState(GrraveYardWS[node][0], team == HORDE ? 0 : 1);
        UpdateWorldState(GrraveYardWS[node][1], team == HORDE ? 1 : 0);
    }
    else if (node == 3)
    {
        for (int8 i = 0; i <= BG_SA_MAX_WS; ++i)
        {
            UpdateWorldState(BG_SA_WorldStatusH[i], team == HORDE ? 1 : 0);
            UpdateWorldState(BG_SA_WorldStatusA[i], team == HORDE ? 0 : 1);
        }
    }
}

void BattleGroundSA::SendWarningToAllSA(uint8 gyd, int status, Team team, bool isDoor, int door, bool destroyed)
{
    if (!isDoor)
    {
        switch(status)
        {
            case STATUS_CLAIMED:
            {
                if (team == HORDE)
                {
                    switch(gyd)
                    {
                        case 0: SendWarningToAll(LANG_BG_SA_HORDE_EAST_CLAIMED); break;
                        case 1: SendWarningToAll(LANG_BG_SA_HORDE_WEST_CLAIMED); break;
                        case 2: SendWarningToAll(LANG_BG_SA_HORDE_SOUTH_CLAIMED); break;
                        default: sLog.outError("Error in SA strings: Unknow graveyard %s", gyd); break;
                    }
                }
                else
                {
                    switch(gyd)
                    {
                        case 0: SendWarningToAll(LANG_BG_SA_ALLIANCE_EAST_CLAIMED); break;
                        case 1: SendWarningToAll(LANG_BG_SA_ALLIANCE_WEST_CLAIMED); break;
                        case 2: SendWarningToAll(LANG_BG_SA_ALLIANCE_SOUTH_CLAIMED); break;
                        default: sLog.outError("Error in SA strings: Unknow graveyard %s", gyd); break;
                    }
                }
                break;
            }
            case STATUS_CONQUESTED:
            {
                if (team == HORDE)
                {
                    switch(gyd)
                    {
                        case 0: SendWarningToAll(LANG_BG_SA_HORDE_EAST_CONQUESTED); break;
                        case 1: SendWarningToAll(LANG_BG_SA_HORDE_WEST_CONQUESTED); break;
                        case 2: SendWarningToAll(LANG_BG_SA_HORDE_SOUTH_CONQUESTED); break;
                        default: sLog.outError("Error in SA strings: Unknow graveyard %s", gyd); break;
                    }
                }
                else
                {
                    switch(gyd)
                    {
                        case 0: SendWarningToAll(LANG_BG_SA_ALLIANCE_EAST_CONQUESTED); break;
                        case 1: SendWarningToAll(LANG_BG_SA_ALLIANCE_WEST_CONQUESTED); break;
                        case 2: SendWarningToAll(LANG_BG_SA_ALLIANCE_SOUTH_CONQUESTED); break;
                        default: sLog.outError("Error in SA strings: Unknow graveyard %s", gyd); break;
                    }
                }
                break;
            }
            default:
                sLog.outError("Error in SA strings: Unknow status %s", status); break;
        }
    }
    else
    {
        if(destroyed)
        {
            switch(door)
            {
                case BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE: SendWarningToAll(LANG_BG_SA_GATE_ROOM_ANCIENT_SHRINE_DESTROYED); break;
                case BG_SA_GO_GATES_T_GREEN_EMERALD: SendWarningToAll(LANG_BG_SA_GATE_GREEN_EMERALD_DESTROYED); break;
                case BG_SA_GO_GATES_T_BLUE_SAPHIRE: SendWarningToAll(LANG_BG_SA_GATE_BLUE_SAPHIRE_DESTROYED); break;
                case BG_SA_GO_GATES_T_MAUVE_AMETHYST: SendWarningToAll(LANG_BG_SA_GATE_MAUVE_AMETHYST_DESTROYED); break;
                case BG_SA_GO_GATES_T_RED_SUN: SendWarningToAll(LANG_BG_SA_GATE_RED_SUN_DESTROYED); break;
                case BG_SA_GO_GATES_T_YELLOW_MOON: SendWarningToAll(LANG_BG_SA_GATE_YELLOW_MOON_DESTROYED); break;
                default:
                    sLog.outError("Error in SA strings: Unknow door %s", door); break;
            }
        }
        else
        {
            switch(door)
            {
                case BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE: SendWarningToAll(LANG_BG_SA_GATE_ROOM_ANCIENT_SHRINE_DAMAGED); break;
                case BG_SA_GO_GATES_T_GREEN_EMERALD: SendWarningToAll(LANG_BG_SA_GATE_GREEN_EMERALD_DAMAGED); break;
                case BG_SA_GO_GATES_T_BLUE_SAPHIRE: SendWarningToAll(LANG_BG_SA_GATE_BLUE_SAPHIRE_DAMAGED); break;
                case BG_SA_GO_GATES_T_MAUVE_AMETHYST: SendWarningToAll(LANG_BG_SA_GATE_MAUVE_AMETHYST_DAMAGED); break;
                case BG_SA_GO_GATES_T_RED_SUN: SendWarningToAll(LANG_BG_SA_GATE_RED_SUN_DAMAGED); break;
                case BG_SA_GO_GATES_T_YELLOW_MOON: SendWarningToAll(LANG_BG_SA_GATE_YELLOW_MOON_DAMAGED); break;
                default:
                    sLog.outError("Error in SA strings: Unknow door %s", door); break;
            }
        }
    }
}

void BattleGroundSA::TeleportPlayerToCorrectLoc(Player *plr, bool resetBattle)
{
    if (!plr)
        return;

    if (!m_bShipsStarted)
    {
        if (plr->GetTeam() != m_tDefender)
        {
            plr->CastSpell(plr,12438,true);//Without this player falls before boat loads...

            if (urand(0,1))
                plr->TeleportTo(607, BG_SA_BoatOneStartPos[0], BG_SA_BoatOneStartPos[1], BG_SA_BoatOneStartPos[2], BG_SA_BoatOneStartPos[3], 0);
            else
                plr->TeleportTo(607, BG_SA_BoatTwoStartPos[0], BG_SA_BoatTwoStartPos[1], BG_SA_BoatTwoStartPos[2], BG_SA_BoatTwoStartPos[3], 0);

        }
        else
            plr->TeleportTo(607, 1209.7f, -65.16f, 70.1f, 0.0f, 0); // defenders
    }
    else
    {
        if (plr->GetTeam() != m_tDefender)
            plr->TeleportTo(607, 1600.381f, -106.263f, 8.8745f, 3.78f, 0); // docks
        else
            plr->TeleportTo(607, 1209.7f, -65.16f, 70.1f, 0.0f, 0); // defenders
    }
    SendTransportInit(plr);
    if (resetBattle)
    {
        if (!plr->isAlive())
        {
            plr->ResurrectPlayer(1.0f);
            plr->SpawnCorpseBones();
        }

        plr->SetHealth(plr->GetMaxHealth());
        plr->SetPower(POWER_MANA, plr->GetMaxPower(POWER_MANA));
        plr->CombatStopWithPets(true);
    }
}

void BattleGroundSA::SendTransportInit(Player *player)
{
    if (GetBGObject(BG_SA_BOAT_ONE) || GetBGObject(BG_SA_BOAT_TWO))
    {
        UpdateData transData;
        if (GetBGObject(BG_SA_BOAT_ONE))
            GetBGObject(BG_SA_BOAT_ONE)->BuildCreateUpdateBlockForPlayer(&transData, player);
        if (GetBGObject(BG_SA_BOAT_TWO))
            GetBGObject(BG_SA_BOAT_TWO)->BuildCreateUpdateBlockForPlayer(&transData, player);
        WorldPacket packet;
        transData.BuildPacket(&packet);
        player->GetSession()->SendPacket(&packet);
    }
}

void BattleGroundSA::SendTransportsRemove(Player * player)
{
    if (GetBGObject(BG_SA_BOAT_ONE) || GetBGObject(BG_SA_BOAT_TWO))
    {
        UpdateData transData;
        if (GameObject * boat1 = GetBGObject(BG_SA_BOAT_ONE))
        {
            boat1->BuildOutOfRangeUpdateBlock(&transData);
            boat1->SetRespawnTime(0);
            boat1->Delete();
        }
        if (GameObject * boat2 = GetBGObject(BG_SA_BOAT_TWO))
        {
            boat2->BuildOutOfRangeUpdateBlock(&transData);
            boat2->SetRespawnTime(0);
            boat2->Delete();
        }
        WorldPacket packet;
        transData.BuildPacket(&packet);
        player->GetSession()->SendPacket(&packet);
    }
}

uint32 BattleGroundSA::GetCorrectFactionSA(uint8 vehicleType) const
{
    if (GetStatus() != STATUS_WAIT_JOIN)
    {
        switch(vehicleType)
        {
            case VEHICLE_SA_DEMOLISHER: return (m_tDefender == ALLIANCE) ? VEHICLE_FACTION_HORDE : VEHICLE_FACTION_ALLIANCE;
            case VEHICLE_SA_CANNON:     return (m_tDefender == ALLIANCE) ? VEHICLE_FACTION_ALLIANCE : VEHICLE_FACTION_HORDE;
            default:                    return VEHICLE_FACTION_NEUTRAL;
        }
    }
    return VEHICLE_FACTION_NEUTRAL;
}
