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

namespace StormcastEternals {

    DamageSpell *CreatePrimeElectrids(Unit *caster) {
        return new DamageSpell(caster, "Prime Electrids", 5, 18, RAND_D3, 10, RAND_D6);
    }

    DamageSpell *CreateSpiritStorm(Unit *caster) {
        return new DamageSpell(caster, "Spirit Storm", 7, 18, 1);
    }

    LineOfEffectSpell *CreatePyroelectricBlast(Unit *caster) {
        return new LineOfEffectSpell(caster, "Pyroelectric Blast", 6, 9, RAND_D3, 0);
    }

    AreaOfEffectSpell *CreateLightningPulse(Unit *caster) {
        return new AreaOfEffectSpell(caster, "Lightning Pulse", 5, 0, 12, 1, 5);
    }

    AreaOfEffectSpell *CreateLightningOrb(Unit *caster) {
        return new AreaOfEffectSpell(caster, "Lightning Orb", 6, 12, 3, RAND_D3, 4);
    }

    LineOfEffectSpell *CreateStormLance(Unit *caster) {
        return new LineOfEffectSpell(caster, "Storm Lance", 5, 12, 1, 5);
    }

    HealSpell *CreateHealingLight(Unit *caster) {
        return new HealSpell(caster, "Healing Light", 5, 18, RAND_D3, 10, RAND_D6);
    }

    class PurifyingBlast : public Spell {
    public:
        explicit PurifyingBlast(Unit *caster);

    protected:

        Result apply(int castingValue, int unmodifiedCastingValue, Unit *target) override;

        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override {
            return apply(castingValue, unmodifiedCastingValue, nullptr);
        }

    };

    PurifyingBlast::PurifyingBlast(Unit *caster) :
            Spell(caster, "Purifying Blast", 5, 6) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result PurifyingBlast::apply(int castingValue, int unmodifiedCastingValue, Unit * /*target*/) {

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
        return Result::Success;
    }

    Spell *CreatePurifyingBlast(Unit *caster) {
        return new PurifyingBlast(caster);
    }

    Spell *CreateThunderclap(Unit *caster) {
        return nullptr;
    }

    class Stormsire : public Spell {
    public:
        explicit Stormsire(Unit *caster);

    protected:

        Result apply(int castingValue, int unmodifiedCastingValue, Unit *target) override;

        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override {
            return apply(castingValue, unmodifiedCastingValue, nullptr);
        }

    };

    Stormsire::Stormsire(Unit *caster) :
            Spell(caster, "Stormsire", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::AreaOfEffectDamage;
    }

    Spell::Result Stormsire::apply(int castingValue, int unmodifiedCastingValue, Unit * /*target*/) {

        auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), range());
        for (auto ip : units) {
            Wounds wounds = {0, Dice::RollD3(), Wounds::Source::Spell};
            if (m_caster->distanceTo(ip) <= 6.0) {
                wounds.mortal = Dice::RollD6();
            }

            int numSlain = ip->applyDamage(wounds, m_caster);
            if (numSlain > 0) {
                SimLog(Verbosity::Narrative, "%s inflicts %d mortal wounds on unit %s.\n",
                       m_caster->name().c_str(), wounds.mortal, ip->name().c_str());
            }

            ip->buffModifier(Run_Distance, -1, defaultDuration());
            ip->buffModifier(Charge_Distance, -1, defaultDuration());
        }
        return Result::Success;
    }

    Spell *CreateStormsire(Unit *caster) {
        return new Stormsire(caster);
    }

}