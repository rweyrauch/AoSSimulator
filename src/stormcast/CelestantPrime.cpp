/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/CelestantPrime.h>
#include <Board.h>
#include "UnitFactory.h"

namespace StormcastEternals
{

bool CelestantPrime::s_registered = false;

Unit *CelestantPrime::Create(const ParameterList &parameters)
{
    auto unit = new CelestantPrime();

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CelestantPrime::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Celestant Prime", *factoryMethod);
    }
}

std::string CelestantPrime::ValueToString(const Parameter &parameter)
{
    return StormcastEternal::ValueToString(parameter);
}

int CelestantPrime::EnumStringToInt(const std::string &enumString)
{
    return StormcastEternal::EnumStringToInt(enumString);
}

CelestantPrime::CelestantPrime() :
    StormcastEternal("Celestant Prime", 12, WOUNDS, 10, 3, true),
    m_ghalMaraz(Weapon::Type::Melee, "Ghal Maraz, the Hammer of Sigmar", 2, 3, 3, 2, -3, 3)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, CELESTANT_PRIME};
    m_weapons = {&m_ghalMaraz};
}

bool CelestantPrime::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_ghalMaraz);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}


} // namespace StormcastEternals