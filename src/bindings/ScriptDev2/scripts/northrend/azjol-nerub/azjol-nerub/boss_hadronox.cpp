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
SDName: Boss_Hadronox
SD%Complete: 
SDComment:
SDCategory: Azjol'Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SPELL_ACID_CLOUD            = 53400,
    SPELL_ACID_CLOUD_H          = 59419,
    SPELL_LEECH_POISON          = 53030,
    SPELL_LEECH_POISON_H        = 59417,
    SPELL_PIERCE_ARMOR          = 53418,
    SPELL_WEB_GRAB              = 53406,
    SPELL_WEB_GRAB_H            = 59420,
    SPELL_WEB_FRONT_DOORS       = 53177, // dummy effect, let's players know that no more adds are spawning
    SPELL_WEB_SIDE_DOORS        = 53185,


    MOB_NECROMANCER             = 29098,
    MOB_CHAMPION                = 29117,
    MOB_CRYPT_FRIEND            = 29063,

};

/*######
## boss_hadronox
######*/

struct MANGOS_DLL_DECL boss_hadronoxAI : public ScriptedAI
{
    boss_hadronoxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_azjol_nerub*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_azjol_nerub* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiAcidCloudTimer;
    uint32 m_uiLeechPoisonTimer;
    uint32 m_uiPierceArmorTimer;
    uint32 m_uiWebGrabTimer;
    uint32 m_uiWebFrontDoorsTimer;
    uint32 m_uiWebSideDoorsTimer;

    void Reset()
    {
        m_uiAcidCloudTimer      = 20000;
        m_uiLeechPoisonTimer    = 9000;
        m_uiPierceArmorTimer    = 8000;
        m_uiWebGrabTimer        = 24000;
        m_uiWebFrontDoorsTimer  = 50000;
        m_uiWebSideDoorsTimer   = 49000;
    }

    void Aggro(Unit *pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HADRONOX, IN_PROGRESS);
    }

    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HADRONOX, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        m_creature->SetHealth(m_creature->GetHealth() + (m_creature->GetMaxHealth() * 0.1));
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiAcidCloudTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_ACID_CLOUD : SPELL_ACID_CLOUD_H);
                m_uiAcidCloudTimer = 20000;
            }
        }else m_uiAcidCloudTimer -= uiDiff;

        if (m_uiLeechPoisonTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_LEECH_POISON : SPELL_LEECH_POISON_H);
                m_uiLeechPoisonTimer = 9000;
            }
        }else m_uiLeechPoisonTimer -= uiDiff;

        if (m_uiPierceArmorTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_PIERCE_ARMOR);
            m_uiPierceArmorTimer = 8000;
        }else m_uiPierceArmorTimer -= uiDiff;

        if (m_uiWebGrabTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_WEB_GRAB : SPELL_WEB_GRAB_H);
                m_uiWebGrabTimer = 24000;
            }
        }else m_uiWebGrabTimer -= uiDiff;

        /*if (m_uiWebFrontDoorsTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WEB_FRONT_DOORS);
            m_uiWebFrontDoorsTimer = 50000;
        }else m_uiWebFrontDoorsTimer -= uiDiff;


        if (m_uiWebSideDoorsTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WEB_SIDE_DOORS);
            m_uiWebSideDoorsTimer = 49000;
        }else m_uiWebSideDoorsTimer -= uiDiff;*/


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hadronox(Creature* pCreature)
{
    return new boss_hadronoxAI(pCreature);
}

void AddSC_boss_hadronox()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_hadronox";
    pNewScript->GetAI = &GetAI_boss_hadronox;
    pNewScript->RegisterSelf();
}
