/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/StormcastSpells.h>
#include <Board.h>
#include <Unit.h>

DamageSpell* CreatePrimeElectrids(Unit* caster)
{
    return new DamageSpell(caster, "Prime Electrids", 5, 18.0f, RAND_D3, 10, RAND_D6);
}

DamageSpell* CreateSpiritStorm(Unit* caster)
{
    return new DamageSpell(caster, "Spirit Storm", 7, 18.0f, 1);
}

LineOfEffectSpell *CreatePyroelectricBlast(Unit *caster)
{
    return new LineOfEffectSpell(caster, "Pyroelectric Blast", 6, 9.0f, RAND_D3, 0);
}

AreaOfEffectSpell *CreateLightningPulse(Unit *caster)
{
    return new AreaOfEffectSpell(caster, "Lightning Pulse", 5, 0.0f, 12.0f, 1, 5);
}

AreaOfEffectSpell *CreateLightningOrb(Unit *caster)
{
    return new AreaOfEffectSpell(caster, "Lightning Orb", 6, 12.0f, 3.0f, RAND_D3, 4);
}

LineOfEffectSpell *CreateStormLance(Unit *caster)
{
    return new LineOfEffectSpell(caster, "Storm Lance", 5, 12.0f, 1, 5);
}

HealSpell *CreateHealingLight(Unit *caster)
{
    return new HealSpell(caster, "Healing Light", 5, 18.0f, RAND_D3, 10, RAND_D6);
}

class PurifyingBlast : public Spell
{
public:
    PurifyingBlast(Unit* caster);

    Result cast(Unit* target, int round) override;
    Result cast(float x, float y, int round)  { return cast(nullptr, round); }

};

PurifyingBlast::PurifyingBlast(Unit *caster) :
    Spell(caster, "Purifying Blast", 5, 6.0f)
{

}

Spell::Result PurifyingBlast::cast(Unit* /*target*/, int round)
{
    Dice dice;
    Spell::Result result = Failed;

    const int castingRoll = dice.roll2D6();
    if (castingRoll >= m_castingValue)
    {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound)
        {
            auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), range());
            for (auto ip : units)
            {
                int roll = dice.rollD6();
                int total = roll + ip->bravery();
                if (ip->hasKeyword(DEATH) || ip->hasKeyword(DAEMON))
                {
                    total = roll + ip->bravery() / 2;
                }
                if (total < 10)
                {
                    Wounds wounds = {0, dice.rollD3()};
                    int numSlain = ip->applyDamage(wounds);
                    if (numSlain > 0)
                    {
                        SimLog(Verbosity::Narrative, "%s inflicts %d mortal wounds on unit %s.\n", m_caster->name().c_str(), wounds.mortal, ip->name().c_str());
                    }
                }
            }
            result = Success;
        }
        else
        {
            SimLog(Verbosity::Narrative, "%s spell %s was unbound.\n", m_caster->name().c_str(), name().c_str());
            result = Unbound;
        }
    }
    else
    {
        SimLog(Verbosity::Narrative, "%s spell %s failed with roll %d needing %d.\n", m_caster->name().c_str(), name().c_str(),
               castingRoll, m_castingValue);
    }

    return result;
}

Spell *CreatePurifyingBlast(Unit *caster)
{
    return new PurifyingBlast(caster);
}
