/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "OssiarchBonereaperPrivate.h"
#include "Roster.h"

namespace OssiarchBonereapers {

    class ArcaneCommand : public Spell {
    public:
        explicit ArcaneCommand(Unit *caster) :
                Spell(caster, "Arcane Command", 5, 0) {
            m_allowedTargets = Abilities::Target::Self;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            auto roster = m_caster->getRoster();
            if (roster) {
                roster->incrementResource(Dice::RollD3());
            }
            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override {
            return apply(castingRoll, unmodifiedCastingValue, nullptr);
        }
    };

    class DrainVitality : public Spell {
    public:
        explicit DrainVitality(Unit *caster) :
                Spell(caster, "Drain Vitality", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            target->buffReroll(Attribute::To_Hit_Missile, Rerolls::Sixes, defaultDuration());
            target->buffReroll(Attribute::To_Hit_Melee, Rerolls::Sixes, defaultDuration());
            target->buffReroll(Attribute::To_Save_Melee, Rerolls::Sixes, defaultDuration());
            target->buffReroll(Attribute::To_Save_Missile, Rerolls::Sixes, defaultDuration());

            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Arcane_Command:
                return new ArcaneCommand(caster);
            case Lore::Drain_Vitality:
                return new DrainVitality(caster);
            case Lore::Empower_Nadirite_Weapons:
                break;
            case Lore::Mortal_Contract:
                break;
            case Lore::Protection_Of_Nagash:
                break;
            case Lore::Reinforce_Battle_Shields:
                break;
            default:
                break;
        }
        return nullptr;
    }

} // namespace OssiarchBonereapers
