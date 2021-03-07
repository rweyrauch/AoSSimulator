/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "LoreOfShadows.h"

namespace DaughtersOfKhaine {

    class SteedOfShadows : public Spell {
    public:
        explicit SteedOfShadows(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    SteedOfShadows::SteedOfShadows(Unit *caster) :
        Spell(caster, "Steed of Shadows", 6, 0) {
        m_allowedTargets = Abilities::Target::Self;
        m_effect = Abilities::EffectType::Buff;
    }

    Spell::Result SteedOfShadows::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        target->buffModifier(Move_Distance, 16 - m_caster->move(), defaultDuration());
        target->buffMovement(Can_Fly, true, defaultDuration());

        return Spell::Result::Success;
    }

    class PitOfShades : public Spell {
    public:
        explicit PitOfShades(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    PitOfShades::PitOfShades(Unit *caster) :
        Spell(caster, "Pit of Shades", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result PitOfShades::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        auto roll = Dice::Roll2D6();
        if (roll > target->move()) {
            target->applyDamage({0, roll-target->move(), Wounds::Source::Spell}, m_caster);
        }
        return Spell::Result::Success;
    }

    class TheWithering : public Spell {
    public:
        explicit TheWithering(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    TheWithering::TheWithering(Unit *caster) :
        Spell(caster, "The Withering", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Debuff;
    }

    Spell::Result TheWithering::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        target->buffModifier(Target_To_Wound_Melee, 1, defaultDuration());
        target->buffModifier(Target_To_Wound_Missile, 1, defaultDuration());

        return Spell::Result::Success;
    }

    class ShroudOfDespair : public Spell {
    public:
        explicit ShroudOfDespair(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    ShroudOfDespair::ShroudOfDespair(Unit *caster) :
        Spell(caster, "Shroud of Despair", 4, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Debuff;
    }

    Spell::Result ShroudOfDespair::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        auto modifier = 1;
        if (castingValue >= 8) modifier = Dice::RollD3();

        target->buffModifier(Bravery, modifier, defaultDuration());

        return Spell::Result::Success;
    }

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Steed_Of_Shadows:
                return new SteedOfShadows(caster);
            case Lore::Pit_Of_Shades:
                return new PitOfShades(caster);
            case Lore::Mirror_Dance:
                // TODO: Mirror_Dance
                break;
            case Lore::The_Withering:
                return new TheWithering(caster);
            case Lore::Mindrazor:
                // TODO: Mindrazor
                break;
            case Lore::Shroud_Of_Despair:
                return new ShroudOfDespair(caster);
            default:
                break;
        }
        return nullptr;
    }

}
