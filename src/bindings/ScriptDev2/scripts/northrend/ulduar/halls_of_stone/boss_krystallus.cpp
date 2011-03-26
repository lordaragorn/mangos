/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_KRYSTALLUS
SD%Complete: 
SDComment:
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"
#include "halls_of_stone.h"

enum
{
    //Yells
    SAY_AGGRO                   = -1599066,
    SAY_KILL                    = -1599068,
    SAY_DEATH                   = -1599069,
    SAY_SHATTER                 = -1599067,

    //Spells
    SPELL_BOULDER               = 50843,
    SPELL_BOULDER_H             = 59742,
    SPELL_STOMP                 = 50868,
    SPELL_STOMP_H               = 59744,
    SPELL_GROUND_SLAM           = 50827,
    SPELL_SHATTER               = 50810,
    SPELL_SHATTER_H             = 61546,
    SPELL_GROUND_SPIKE          = 59750,
    SPELL_STONED                = 33652
};

struct MANGOS_DLL_DECL boss_krystallusAI : public ScriptedAI
{
    boss_krystallusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiBoulderTimer;
    uint32 m_uiStompTimer;
    uint32 m_uiGroundSlamTimer;
    uint32 m_uiShatterTimer;
    uint32 m_uiGroundSpikeTimer;

    bool m_bPerformingGroundSlam;
    bool m_bIsRegularMode;

    void Reset()
    {
        m_uiBoulderTimer            = 13000;
        m_uiStompTimer              = 12000;
        m_uiGroundSlamTimer         = 30000;
        m_uiShatterTimer            = 35000;
        m_uiGroundSpikeTimer        = 21000;
        m_bPerformingGroundSlam     = false;
    }

    void Aggro(Unit *pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRYSTALLUS, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRYSTALLUS, NOT_STARTED);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRYSTALLUS, DONE);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_SHATTER)
        {
            pTarget->CastSpell(pTarget, SPELL_SHATTER, true);

            if (pTarget->HasAura(SPELL_STONED))
                pTarget->RemoveAurasDueToSpell(SPELL_STONED);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiGroundSlamTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_GROUND_SLAM) == CAST_OK)
            {
                m_uiGroundSlamTimer = 30000;
                m_uiShatterTimer = 5000;
            }
        }else m_uiGroundSlamTimer -= uiDiff;

        if (m_uiShatterTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_SHATTER) == CAST_OK)
                m_uiShatterTimer = 30000;
        }else m_uiShatterTimer -= uiDiff;

        if (m_uiStompTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STOMP : SPELL_STOMP_H) == CAST_OK)
                m_uiStompTimer = 12000;
        }else m_uiStompTimer -= uiDiff;
        

        if (m_uiBoulderTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_BOULDER : SPELL_BOULDER_H) == CAST_OK)
                    m_uiBoulderTimer = 13000;
            }
        }else m_uiBoulderTimer -= uiDiff;

        if (!m_bIsRegularMode)
        {
            if (m_uiGroundSpikeTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_GROUND_SPIKE) == CAST_OK)
                    m_uiGroundSpikeTimer = 21000;
            }else m_uiGroundSpikeTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_krystallus(Creature* pCreature)
{
    return new boss_krystallusAI(pCreature);
}

void AddSC_boss_krystallus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_krystallus";
    newscript->GetAI = &GetAI_boss_krystallus;
    newscript->RegisterSelf();
}
