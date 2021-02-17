/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Lore.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {

    class Viletide : public Spell {
    public:
       explicit Viletide(Unit* caster);

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
   };

    Viletide::Viletide(Unit *caster) :
        Spell(caster, "Viletide", 6, 12) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result Viletide::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        Wounds wounds = { 0, Dice::RollD3(),Wounds::Source::Spell };
        if (m_caster->distanceTo(target) <= 6.0) {
            wounds.mortal = Dice::RollD6();
        }
        target->applyDamage(wounds, m_caster);

        return Spell::Result::Success;
    }

    class WildRampage : public Spell {
    public:
        explicit WildRampage(Unit* caster);

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    WildRampage::WildRampage(Unit *caster) :
        Spell(caster, "Wild Rampage", 6, 12) {
        m_allowedTargets = Abilities::Target::Friendly;
        m_effect = Abilities::EffectType::Buff;
    }

    Spell::Result WildRampage::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        target->buffReroll(To_Wound_Melee, Reroll_Failed, defaultDuration());
        target->buffModifier(To_Save, -1, defaultDuration());

        return Spell::Result::Success;
    }

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Viletide:
                return new Viletide(caster);
            case Lore::Vicious_Stanglethorns:
                return nullptr;
            case Lore::Savage_Dominion:
                return nullptr;
            case Lore::Tendrils_Of_Atrophy:
                return new BuffModifierSpell(caster, "Tendrils of Atrophy", 6, 12, To_Save, -1, Abilities::Target::Enemy);
            case Lore::Wild_Rampage:
                return new WildRampage(caster);
            case Lore::Titanic_Fury:
                return new BuffModifierSpell(caster, "Titanic Fury", 7, 12, Attacks_Melee, 1, Abilities::Target::Friendly, {BEASTS_OF_CHAOS, MONSTER});
            case Lore::Thunderwave:
                return new AreaOfEffectSpell(caster, "Thunderwave", 7, 0, 3, RAND_D3, 0);
            case Lore::Hailstorm:
                return nullptr;
            case Lore::Sundering_Blades:
                return nullptr;
            default:
                return nullptr;
        }
    }

} //namespace BeastsOfChaos
