/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Prayer.h>
#include <string>

class Unit;

namespace StormcastEternals {

    enum class PrayersOfTheStormhost : int {
        Divine_Light,
        Bless_Weapons,
        Bolster_Faith,
        Abjuration,
        God_Kings_Aspect,
        Translocation,
    };

    std::string ToString(PrayersOfTheStormhost which);

    bool FromString(const std::string &enumString, PrayersOfTheStormhost &outPrayer);

    Prayer *CreateDivineLight(Unit *caster);

    Prayer *CreateBlessWeapons(Unit *caster);

    Prayer *CreateBolsterFaith(Unit *caster);

    Prayer *CreateAbjuration(Unit *caster);

    Prayer *CreateGodKingsAspect(Unit *caster);

    Prayer *CreateTranslocation(Unit *caster);

    Prayer *CreatePrayerOfTheStormhost(PrayersOfTheStormhost which, Unit *caster);

} // namespace StormcastEternals
