/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Board.h>
#include "Spell.h"
#include "SeraphonLore.h"

namespace Seraphon {

    class WalkBetweenRealms : public Spell {
    public:
        explicit WalkBetweenRealms(Unit *caster) :
                Spell(caster, "Walk Between Realms", 6, 18) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;
            target->buffMovement(MovementRule::Can_Fly, true, defaultDuration());
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    class StellarTempest : public Spell {
    public:
        explicit StellarTempest(Unit *caster) :
                Spell(caster, "Stellar Tempest", 8, 24) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            Dice::RollResult rolls;
            Dice::RollD6(target->remainingModels(), rolls);
            target->applyDamage({0, rolls.rollsGE(5), Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    class CelestialHarmony : public Spell {
    public:
        explicit CelestialHarmony(Unit *caster) :
                Spell(caster, "Celestial Harmony", 5, 18) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;
            target->buffAbility(Ability::Ignore_Battleshock, 1, defaultDuration());
            if (castingRoll >= 10) {
                auto units = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), m_range);
                for (auto unit : units) {
                    if (unit == target) continue;
                    unit->buffAbility(Ability::Ignore_Battleshock, 1, defaultDuration());
                }
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    class HandOfGlory : public Spell {
    public:
        explicit HandOfGlory(Unit *caster) :
                Spell(caster, "Hand of Glory", 6, 18) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;
            target->buffReroll(Attribute::To_Hit_Melee, Rerolls::Ones, defaultDuration());
            target->buffReroll(Attribute::To_Hit_Missile, Rerolls::Ones, defaultDuration());
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    class TideOfSerpents : public Spell {
    public:
        explicit TideOfSerpents(Unit *caster) :
                Spell(caster, "Tide of Serpents", 8, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            Dice::RollResult rolls;
            Dice::RollD6(target->remainingModels(), rolls);
            target->applyDamage({0, rolls.rollsGE(5), Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Celestial_Apotheosis:
                // TODO: Celestial_Apotheosis
                return nullptr;
            case Lore::Walk_Between_Realms:
                return new WalkBetweenRealms(caster);
            case Lore::Mystical_Unforging:
                // TODO: Mystical_Unforging
                return nullptr;
            case Lore::Celestial_Equilibrium:
                // TODO: Celestial_Equilibrium
                return nullptr;
            case Lore::Stellar_Tempest:
                return new StellarTempest(caster);
            case Lore::Drain_Magic:
                // TODO: Drain_Magic
                return nullptr;
            case Lore::Celestial_Harmony:
                return new CelestialHarmony(caster);
            case Lore::Hand_Of_Glory:
                return new HandOfGlory(caster);
            case Lore::Extend_Astromatrix:
                // TODO: Extend_Astromatrix
                return nullptr;
            case Lore::Fiery_Convocation:
                // TODO: Fiery_Convocation
                return nullptr;
            case Lore::Bind_Endless_Spell:
                // TODO: Bind_Endless_Spell
                return nullptr;
            case Lore::Tide_Of_Serpents:
                return new TideOfSerpents(caster);
            default:
                break;
        }
        return nullptr;
    }

} // namespace Seraphon
