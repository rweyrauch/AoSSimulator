/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightHeraldor.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{

bool KnightHeraldor::s_registered = false;

KnightHeraldor::KnightHeraldor() :
    StormcastEternal("Knight-Heraldor", 5, WOUNDS, 8, 3, false),
    m_broadsword(Weapon::Type::Melee, "Sigmarite Broadsword", 1, 4, 3, 4, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_HERALDOR};
    m_weapons = {&m_broadsword};
}

bool KnightHeraldor::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_broadsword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *KnightHeraldor::Create(const ParameterList &parameters)
{
    auto unit = new KnightHeraldor();

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

void KnightHeraldor::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
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

        s_registered = UnitFactory::Register("Knight-Heraldor", factoryMethod);
    }
}


} // namespace StormcastEternals