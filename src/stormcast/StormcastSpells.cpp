/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/StormcastSpells.h>
#include <Board.h>
#include <Unit.h>

DamageSpell *CreatePrimeElectrids(Unit *caster) {
    return new DamageSpell(caster, "Prime Electrids", 5, 18.0, RAND_D3, 10, RAND_D6);
}

DamageSpell *CreateSpiritStorm(Unit *caster) {
    return new DamageSpell(caster, "Spirit Storm", 7, 18.0, 1);
}

LineOfEffectSpell *CreatePyroelectricBlast(Unit *caster) {
    return new LineOfEffectSpell(caster, "Pyroelectric Blast", 6, 9.0, RAND_D3, 0);
}

AreaOfEffectSpell *CreateLightningPulse(Unit *caster) {
    return new AreaOfEffectSpell(caster, "Lightning Pulse", 5, 0.0, 12.0, 1, 5);
}

AreaOfEffectSpell *CreateLightningOrb(Unit *caster) {
    return new AreaOfEffectSpell(caster, "Lightning Orb", 6, 12.0, 3.0, RAND_D3, 4);
}

LineOfEffectSpell *CreateStormLance(Unit *caster) {
    return new LineOfEffectSpell(caster, "Storm Lance", 5, 12.0, 1, 5);
}

HealSpell *CreateHealingLight(Unit *caster) {
    return new HealSpell(caster, "Healing Light", 5, 18.0, RAND_D3, 10, RAND_D6);
}

class PurifyingBlast : public Spell {
public:
    explicit PurifyingBlast(Unit *caster);

    Result cast(Unit *target, int round) override;

    Result cast(double x, double y, int round) override { return cast(nullptr, round); }

};

PurifyingBlast::PurifyingBlast(Unit *caster) :
        Spell(caster, "Purifying Blast", 5, 6.0) {
    m_allowedTargets = Target::Enemy;
    m_effect = EffectType::Damage;
}

Spell::Result PurifyingBlast::cast(Unit * /*target*/, int round) {
    Spell::Result result = Result::Failed;

    const int castingRoll = m_caster->rollCasting();
    if (castingRoll >= m_castingValue) {
        bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
        if (!unbound) {
            auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), range());
            for (auto ip : units) {
                int roll = Dice::RollD6();
                int total = roll + ip->bravery();
                if (ip->hasKeyword(DEATH) || ip->hasKeyword(DAEMON)) {
                    total = roll + ip->bravery() / 2;
                }
                if (total < 10) {
                    Wounds wounds = {0, Dice::RollD3(), Wounds::Source::Spell};
                    int numSlain = ip->applyDamage(wounds, m_caster);
                    if (numSlain > 0) {
                        SimLog(Verbosity::Narrative, "%s inflicts %d mortal wounds on unit %s.\n",
                               m_caster->name().c_str(), wounds.mortal, ip->name().c_str());
                    }
                }
            }
            result = Result::Success;
        } else {
            SimLog(Verbosity::Narrative, "%s spell %s was unbound.\n", m_caster->name().c_str(), name().c_str());
            result = Result::Unbound;
        }
    } else {
        SimLog(Verbosity::Narrative, "%s spell %s failed with roll %d needing %d.\n", m_caster->name().c_str(),
               name().c_str(),
               castingRoll, m_castingValue);
    }

    return result;
}

Spell *CreatePurifyingBlast(Unit *caster) {
    return new PurifyingBlast(caster);
}

Spell *CreateThunderclap(Unit *caster) {
    return nullptr;
}
