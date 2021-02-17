/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/PrayersOfTheStormhosts.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {

    class DivineLight : public Prayer {
    public:
        explicit DivineLight(Unit* priest);

    protected:

        bool apply(int prayingRoll, Unit* target) override;
        bool apply(int prayingRoll, double x, double y) override { return false; }

    };

    DivineLight::DivineLight(Unit *priest) :
        Prayer(priest, "Divine Light", 3, 18) {
        m_allowedTargets = Abilities::Target::Any;
        m_effect = Abilities::EffectType::Buff;
    }

    bool DivineLight::apply(int prayingRoll, Unit *target) {
        if (target == nullptr)
            return false;

        const Duration duration = {Phase::Hero, m_round + 1, m_priest->owningPlayer()};
        if (m_priest->isFriendly(target)) {
            target->buffReroll(Target_To_Hit_Melee, Reroll_Sixes, duration);
            target->buffReroll(Target_To_Hit_Missile,Reroll_Sixes, duration);
        }
        else {
            target->buffReroll(Target_To_Hit_Melee, Reroll_Ones, duration);
            target->buffReroll(Target_To_Hit_Missile, Reroll_Ones, duration);
        }
        return true;
    }

    std::string ToString(PrayersOfTheStormhost which) {
        return std::string(magic_enum::enum_name(which));
    }

    bool FromString(const std::string &enumString, PrayersOfTheStormhost &outPrayer) {
        auto prayer = magic_enum::enum_cast<PrayersOfTheStormhost>(enumString);
        if (prayer.has_value())
            outPrayer = prayer.value();
        return prayer.has_value();
    }

    Prayer *CreateDivineLight(Unit *caster) {
        return new DivineLight(caster);
    }

    Prayer *CreateBlessWeapons(Unit *caster) {
        return nullptr;
    }

    Prayer *CreateBolsterFaith(Unit *caster) {
        return nullptr;
    }

    Prayer *CreateAbjuration(Unit *caster) {
        return nullptr;
    }

    Prayer *CreateGodKingsAspect(Unit *caster) {
        return nullptr;
    }

    Prayer *CreateTranslocation(Unit *caster) {
        return nullptr;
    }

    Prayer *CreatePrayerOfTheStormhost(PrayersOfTheStormhost which, Unit *caster) {
        switch (which) {
            case PrayersOfTheStormhost::Divine_Light:
                return CreateDivineLight(caster);
            case PrayersOfTheStormhost::Bless_Weapons:
                return CreateBlessWeapons(caster);
            case PrayersOfTheStormhost::Bolster_Faith:
                return CreateBolsterFaith(caster);
            case PrayersOfTheStormhost::Abjuration:
                return CreateAbjuration(caster);
            case PrayersOfTheStormhost::God_Kings_Aspect:
                return CreateGodKingsAspect(caster);
            case PrayersOfTheStormhost::Translocation:
                return CreateTranslocation(caster);
            default:
                return nullptr;
        }
    }

} //namespace StormcastEternals
