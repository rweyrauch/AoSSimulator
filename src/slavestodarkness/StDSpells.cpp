/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "StDSpells.h"

namespace SlavesToDarkness {

    class SpiteTongueCurse : public Spell {
    public:
        explicit SpiteTongueCurse(Unit* caster) :
                Spell(caster, "Spite-tongue Curse", 3, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }

            target->applyDamage({0, 3, Wounds::Source::Spell}, m_caster);

            return Spell::Result::Success;
        }
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }

        void onCast(Result result) override {
            if (result != Result::Success) {
                m_caster->applyDamage({0, 3, Wounds::Source::Spell}, m_caster);
            }
        }
    };

    class WhispersOfChaos : public Spell {
    public:
        explicit WhispersOfChaos(Unit* caster) :
                Spell(caster, "Whispers of Chaos", 7, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }

            Dice::RollResult rolls;
            Dice::RollD6(target->remainingModels(), rolls);
            Wounds wounds{0, rolls.rollsGE(6), Wounds::Source::Spell};
            if (wounds.mortal > 0) {
                auto numSlain = target->applyDamage(wounds, m_caster);
                if (numSlain > 0) {
                    // unit cannot move until next turn
                    target->buffMovement(Can_Move, false, defaultDuration());
                }
            }
            return Spell::Result::Success;
        }
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    Spell* CreateLore(Lore which, Unit* caster) {
        switch (which) {
            case Lore::Binding_Damnation:
                // TODO: Binding_Damnation
                return nullptr;
            case Lore::Spite_Tongue_Curse:
                return new SpiteTongueCurse(caster);
            case Lore::Whispers_Of_Chaos:
                return new WhispersOfChaos(caster);
            case Lore::Mask_Of_Darkness:
                // TODO: Mask_Of_Darkness
                return nullptr;
            case Lore::Call_To_Glory:
                // TODO: Call_To_Glory
                return nullptr;
            case Lore::Ruinous_Vigour:
                // TODO: Ruinous_Vigour
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }
}
