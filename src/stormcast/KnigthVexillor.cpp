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
static FactoryMethod factoryMethod = {
    KnightVexillor::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool KnightVexillor::s_registered = false;

KnightVexillor::KnightVexillor() :
    StormcastEternal("Knight-Vexillor", 5, WOUNDS, 8, 3, false),
    m_warhammer(Weapon::Type::Melee, "Warhammer", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_VEXILLOR};
}

bool KnightVexillor::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_warhammer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *KnightVexillor::Create(const ParameterList &parameters)
{
    auto unit = new KnightVexillor();
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
        s_registered = UnitFactory::Register("Knight-Vexillor", factoryMethod);
    }
}

void KnightVexillor::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_warhammer);
}


} // namespace StormcastEternals