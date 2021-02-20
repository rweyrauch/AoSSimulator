/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <sylvaneth/SylvanethSpells.h>
#include <Board.h>
#include <Unit.h>

namespace Sylvaneth {

    class Metamorphosis : public DamageSpell {
    public:
        explicit Metamorphosis(Unit *caster);

    protected:

        int getDamage(int castingRoll) const override;
    };

    Metamorphosis::Metamorphosis(Unit *caster) :
            DamageSpell(caster, "Metamorphosis", 7, 16, 1) {
    }

    int Metamorphosis::getDamage(int castingRoll) const {
        Dice::RollResult result;
        Dice::RollD6(castingRoll, result);
        return result.rollsGE(4);
    }

    Spell *CreateMetamorphosis(Unit *caster) {
        return new Metamorphosis(caster);
    }

    class UnleashSpites : public AreaOfEffectSpell {
    public:
        explicit UnleashSpites(Unit *caster) :
                AreaOfEffectSpell(caster, "Unleash Spites", 5, 0, 9, 1, 0) {}

    protected:

        int getDamage(int castingRoll) const override {
            Dice::RollResult result;
            Dice::RollD6(castingRoll, result);
            return result.rollsGE(6);
        }
    };

    Spell *CreateUnleashSpites(Unit *caster) {
        return new UnleashSpites(caster);
    }

    class PrimalTerror : public Spell {
    public:
        explicit PrimalTerror(Unit *caster);

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit *target) override { return Result::Failed; }

        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override;
    };

    PrimalTerror::PrimalTerror(Unit *caster) :
            Spell(caster, "Primal Terror", 6, 10) {
        m_allowedTargets = Abilities::Target::Point;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result PrimalTerror::apply(int castingValue, int unmodifiedCastingValue, double x, double y) {

        const auto roll = Dice::Roll2D6();
        auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), m_range);
        for (auto unit : units) {
            if (unit->bravery() < roll) {
                unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            }
        }
        return Spell::Result::Success;
    }

    Spell *CreatePrimalTerror(Unit *caster) {
        return new PrimalTerror(caster);
    }

    class TheReaping : public Spell {
    public:
        explicit TheReaping(Unit *caster);

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit *target) override;

        Result
        apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    TheReaping::TheReaping(Unit *caster) :
            Spell(caster, "The Reaping", 6, 12) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result TheReaping::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        Dice::RollResult roll;
        Dice::RollD6(6, roll);
        if (roll.rollsGE(5) > 0) {
            target->applyDamage({0, roll.rollsGE(5), Wounds::Source::Spell}, m_caster);
        }

        return Spell::Result::Success;
    }

    Spell *CreateTheReaping(Unit *caster) {
        return new TheReaping(caster);
    }

    class TheDwellersBelow : public Spell {
    public:
        explicit TheDwellersBelow(Unit *caster);

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit *target) override;

        Result
        apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    TheDwellersBelow::TheDwellersBelow(Unit *caster) :
            Spell(caster, "The Dwellers Below", 7, 10) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result TheDwellersBelow::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        Dice::RollResult roll;
        Dice::RollD6(target->remainingModels(), roll);
        if (roll.rollsGE(6) > 0) {
            target->applyDamage({0, roll.rollsGE(6), Wounds::Source::Spell}, m_caster);
        }

        return Spell::Result::Success;
    }

    Spell *CreateLore(Lore lore, Unit *caster) {
        switch (lore) {
            case Lore::Throne_Of_Vines:
                // TODO: Throne_Of_Vines
                return nullptr;
            case Lore::Regrowth:
                return new HealSpell(caster, "Regrowth", 5, 18, RAND_D6);
            case Lore::The_Dwellers_Below:
                return new TheDwellersBelow(caster);
            case Lore::Deadly_Harvest:
                return new AreaOfEffectSpell(caster, "Deadly Harvest", 6, 0, 3, RAND_D3, 0);
            case Lore::Verdurous_Harmony:
                // TODO: Verdurous Harmony
                return nullptr;
            case Lore::Treesong:
                // TODO: Treesong
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }

}