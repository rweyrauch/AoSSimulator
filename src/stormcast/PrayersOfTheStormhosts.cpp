/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/PrayersOfTheStormhosts.h>

namespace StormcastEternals {

    std::string ToString(PrayersOfTheStormhost which) {
        switch (which) {
            case PrayersOfTheStormhost::DivineLight:
                return "Divine Light";
            case PrayersOfTheStormhost::BlessWeapons:
                return "Bless Weapons";
            case PrayersOfTheStormhost::BolsterFaith:
                return "Bolster Faith";
            case PrayersOfTheStormhost::Abjuration:
                return "Abjuration";
            case PrayersOfTheStormhost::GodKingsAspect:
                return "God-King's Aspect";
            case PrayersOfTheStormhost::Translocation:
                return "Translocation";
            default:
                return "None";
        }
    }

    bool FromString(const std::string &enumString, PrayersOfTheStormhost &outPrayer) {
        bool valid = true;

        if (enumString == "None")
            outPrayer = PrayersOfTheStormhost::None;
        else if (enumString == "Divine Light")
            outPrayer = PrayersOfTheStormhost::DivineLight;
        else if (enumString == "Bless Weapons")
            outPrayer = PrayersOfTheStormhost::BlessWeapons;
        else if (enumString == "Bolster Faith")
            outPrayer = PrayersOfTheStormhost::BolsterFaith;
        else if (enumString == "Abjuration")
            outPrayer = PrayersOfTheStormhost::Abjuration;
        else if (enumString == "God-King's Aspect")
            outPrayer = PrayersOfTheStormhost::GodKingsAspect;
        else if (enumString == "Translocation")
            outPrayer = PrayersOfTheStormhost::Translocation;
        else
            valid = false;
        return valid;
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
            case PrayersOfTheStormhost::DivineLight:
                return CreateDivineLight(caster);
            case PrayersOfTheStormhost::BlessWeapons:
                return CreateBlessWeapons(caster);
            case PrayersOfTheStormhost::BolsterFaith:
                return CreateBolsterFaith(caster);
            case PrayersOfTheStormhost::Abjuration:
                return CreateAbjuration(caster);
            case PrayersOfTheStormhost::GodKingsAspect:
                return CreateGodKingsAspect(caster);
            case PrayersOfTheStormhost::Translocation:
                return CreateTranslocation(caster);
            default:
                return nullptr;
        }
    }

} //namespace StormcastEternals
