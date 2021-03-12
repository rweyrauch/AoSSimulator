/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include "FeCSpells.h"

namespace FleshEaterCourt {

    class Bonestorm : public Spell {
    public:
        explicit Bonestorm(Unit *caster) :
                Spell(caster, "Bonestorm", 5, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::AreaOfEffectDamage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {

            auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), m_range);
            for (auto target : units) {
                if (Dice::RollD6() >= 2) {
                    target->applyDamage({0, 1, Wounds::Source::Spell}, m_caster);
                }
            }
            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class SpectralHost : public Spell {
    public:
        explicit SpectralHost(Unit *caster) :
                Spell(caster, "Spectral Host", 6, 12) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }

            if (!target->fly()) {
                target->buffMovement(MovementRule::Can_Fly, true, defaultDuration());
            } else {
                target->buffMovement(MovementRule::Run_And_Charge, true, defaultDuration());
            }

            if (castingRoll >= 10) {
                // Can affect 2 additional units
                auto units = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), m_range);
                int unitsAffected = 0;
                for (auto unit : units) {
                    if (unit == target) continue;

                    if (!unit->fly()) {
                        unit->buffMovement(MovementRule::Can_Fly, true, defaultDuration());
                    } else {
                        unit->buffMovement(MovementRule::Run_And_Charge, true, defaultDuration());
                    }

                    if (unitsAffected >= 2) break;
                }
            }

            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class MiasmalShroud : public Spell {
    public:
        explicit MiasmalShroud(Unit *caster) :
                Spell(caster, "Miasmal Shroud", 5, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            Wounds wounds{0, 0, Wounds::Source::Spell};
            auto roll0 = Dice::RollD6();
            if (roll0 >= 5) wounds.mortal++;
            auto roll1 = Dice::RollD6();
            if (roll1 >= 5) wounds.mortal++;
            auto roll2 = Dice::RollD6();
            if (roll2 >= 5) wounds.mortal++;

            target->applyDamage(wounds, m_caster);

            if ((roll0 == roll1) && (roll0 == roll2)) {
                target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
                target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());
                target->buffModifier(Attribute::To_Wound_Melee, -1, defaultDuration());
                target->buffModifier(Attribute::To_Wound_Missile, -1, defaultDuration());
            } else if ((roll0 == roll1) || (roll0 == roll2) || (roll1 == roll2)) {
                target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
                target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());
            }

            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class DerangedTransformation : public Spell {
    public:
        explicit DerangedTransformation(Unit *caster) :
                Spell(caster, "Deranged Transformation", 6, 24) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }

            if (target->wounds() > 6) {
                return Spell::Result::Failed;
            }

            target->buffModifier(Attribute::Move_Distance, target->wounds(), defaultDuration());

            if (castingRoll >= 10) {
                // Can affect 2 additional units
                auto units = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), m_range);
                int unitsAffected = 0;
                for (auto unit : units) {
                    if (unit == target) continue;

                    if (unit->wounds() <= 6) {
                        unit->buffModifier(Attribute::Move_Distance, unit->wounds(), defaultDuration());
                        unitsAffected++;
                    }

                    if (unitsAffected >= 2) break;
                }
            }

            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Bonestorm:
                return new Bonestorm(caster);
            case Lore::Spectral_Host:
                return new SpectralHost(caster);
            case Lore::Monstrous_Vigour:
                // TODO: Monstrous_Vigour
                return nullptr;
            case Lore::Miasmal_Shroud:
                return new MiasmalShroud(caster);
            case Lore::Deranged_Transformation:
                return new DerangedTransformation(caster);
            case Lore::Blood_Feast:
                // TODO: Blood_Feast
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }
}