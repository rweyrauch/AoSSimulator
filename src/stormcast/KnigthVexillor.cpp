/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightVexillor.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{

bool KnightVexillor::s_registered = false;

KnightVexillor::KnightVexillor() :
    StormcastEternal("Knight-Vexillor", 5, WOUNDS, 8, 3, false),
    m_warhammer(Weapon::Type::Melee, "Warhammer", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_VEXILLOR};
    m_weapons = {&m_warhammer};
}

bool KnightVexillor::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_warhammer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *KnightVexillor::Create(const ParameterList &parameters)
{
    auto unit = new KnightVexillor();

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

void KnightVexillor::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            StormcastEternal::ValueToString,
            StormcastEternal::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Knight-Vexillor", *factoryMethod);
    }
}

} // namespace StormcastEternals