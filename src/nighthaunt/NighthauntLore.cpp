/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "NighthauntLore.h"
#include "Spell.h"
#include "Unit.h"

namespace Nighthaunt {

    class SoulCage : public Spell {
    public:
        SoulCage(Unit *caster) :
                Spell(caster, "Soul Cage", 6, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            target->buffMovement(Can_Retreat, false, defaultDuration());
            target->buffAbility(Fights_Last, 1, defaultDuration());
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    class SpiritDrain : public Spell {
    public:
        SpiritDrain(Unit *caster) :
                Spell(caster, "Spirit Drain", 4, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            Dice::RollResult rolls;
            Dice::RollD6(target->wounds(), rolls);
            target->applyDamage({0, rolls.rollsGE(6), Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    class Lifestealer : public Spell {
    public:
        Lifestealer(Unit *caster) :
                Spell(caster, "Lifestealer", 7, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            auto mortalWounds = Dice::RollD6();
            target->applyDamage({0, mortalWounds, Wounds::Source::Spell}, m_caster);
            m_caster->heal(mortalWounds);
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    class Shademist : public Spell {
    public:
        Shademist(Unit *caster) :
                Spell(caster, "Shademist", 6, 12) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords = {NIGHTHAUNT};
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(Target_To_Wound_Melee, -1, defaultDuration());
            target->buffModifier(Target_To_Wound_Missile, -1, defaultDuration());
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Soul_Cage:
                return new SoulCage(caster);
            case Lore::Spirit_Drain:
                return new SpiritDrain(caster);
            case Lore::Lifestealer:
                return new Lifestealer(caster);
            case Lore::Reaping_Scythe:
                // TODO: Reaping_Scythe
                return nullptr;
            case Lore::Shademist:
                return new Shademist(caster);
            case Lore::Spectral_Tether:
                return new HealSpell(caster, "Spectral Tether", 6, 12, RAND_D3,
                                     -1, -1, {NIGHTHAUNT, HERO});
            default:
                break;
        }
        return nullptr;
    }

} // namespace Nighthaunt
