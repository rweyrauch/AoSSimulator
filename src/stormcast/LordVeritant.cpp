/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordVeritant.h>
#include <iostream>
#include <stormcast/StormcastPrayers.h>
#include <Board.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordVeritant::Create,
    LordVeritant::ValueToString,
    LordVeritant::EnumStringToInt,
    {
        {ParamType::Enum, "Prayers of the Stormhost", (int)PrayersOfTheStormhost::None, (int)PrayersOfTheStormhost::None, (int)PrayersOfTheStormhost::Translocation, 1},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordVeritant::s_registered = false;

LordVeritant::LordVeritant() :
    StormcastEternal("Lord-Veritant", 5, WOUNDS, 9, 3, false),
    m_judgementBlade(Weapon::Type::Melee, "Judgement Blade", 1, 4, 3, 3, -1, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, PRIEST, LORD_VERITANT};

    m_totalPrayers = 2;

    // Lantern of Abjuration
    m_totalUnbinds = 1;
}

bool LordVeritant::configure(PrayersOfTheStormhost prayer)
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_judgementBlade);
    addModel(model);

    m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateSanction(this)));
    m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreatePrayerOfTheStormhost(prayer, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordVeritant::Create(const ParameterList &parameters)
{
    auto unit = new LordVeritant();
    auto prayer = (PrayersOfTheStormhost)GetEnumParam("Prayers of the Stormhost", parameters, (int)PrayersOfTheStormhost::None);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure(prayer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordVeritant::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Veritant", factoryMethod);
    }
}

void LordVeritant::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_judgementBlade);
}

std::string LordVeritant::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Prayers of the Stormhost")
    {
        return ToString((PrayersOfTheStormhost) parameter.m_intValue);
    }
    return StormcastEternal::ValueToString(parameter);
}

int LordVeritant::EnumStringToInt(const std::string &enumString)
{
    PrayersOfTheStormhost prayer;
    if (FromString(enumString, prayer))
    {
        return (int)prayer;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

} // namespace StormcastEternals