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
        return nullptr;
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
