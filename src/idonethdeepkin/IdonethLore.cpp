/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include "IdonethLore.h"
#include "Unit.h"
#include "Spell.h"

namespace IdonethDeepkin {

    class VorpalMaelstrom : public Spell {
    public:
        explicit VorpalMaelstrom(Unit *caster) :
                Spell(caster, "Vorpal Maelstrom", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::AreaOfEffectDamage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            return apply(castingRoll, unmodifiedCastingRoll, target->x(), target->y());
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override {

            auto units = Board::Instance()->getUnitsWithin({x, y, 0}, GetEnemyId(m_caster->owningPlayer()), 3.0);
            for (auto unit : units) {
                if (Dice::RollD6() <= unit->remainingModels()) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
                }
            }
            return Result::Success;
        }
    };

    class PressureOfTheDeep : public Spell {
    public:
        explicit PressureOfTheDeep(Unit *caster) :
                Spell(caster, "Pressure of the Deep", 7, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            if (Dice::RollD6() > target->wounds()) {
                target->slay(1);
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    class TideOfFear : public Spell {
    public:
        explicit TideOfFear(Unit *caster) :
                Spell(caster, "Tide of Fear", 6, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
            target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());
            target->buffModifier(Attribute::Bravery, -1, defaultDuration());
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    class ArcaneCorrasion : public Spell {
    public:
        explicit ArcaneCorrasion(Unit *caster) :
                Spell(caster, "Arcane Corrasion", 6, 48) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            const auto distance = m_caster->distanceTo(target);
            int mortalWounds = 1;
            if (distance >= 36.0) mortalWounds = 4;
            else if (distance >= 24.0) mortalWounds = 3;
            else if (distance >= 12.0) mortalWounds = 2;
            target->applyDamage({0, mortalWounds, Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Steed_Of_Tides:
                // TODO: Steed_Of_Tides
                return nullptr;
            case Lore::Abyssal_Darkness:
                // TODO: Abyssal_Darkness
                return nullptr;
            case Lore::Vorpal_Maelstrom:
                return new VorpalMaelstrom(caster);
            case Lore::Pressure_Of_The_Deep:
                return new PressureOfTheDeep(caster);
            case Lore::Tide_Of_Fear:
                return new TideOfFear(caster);
            case Lore::Arcane_Corrasion:
                return new ArcaneCorrasion(caster);
            case Lore::Freezing_Mists:
                // TODO: Freezing_Mists
                return nullptr;
            case Lore::Protective_Barrier:
                // TODO: Protective_Barrier
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }
}
