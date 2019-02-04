/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightIncantor.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    KnightIncantor::Create,
    nullptr,
    nullptr,
    {
    }
};

bool KnightIncantor::s_registered = false;

KnightIncantor::KnightIncantor() :
    StormcastEternal("Knight-Incantor", 5, WOUNDS, 9, 3, false),
    m_staff(Weapon::Type::Melee, "Incantor's Staff", 2, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, KNIGHT_INCANTOR };
}

bool KnightIncantor::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_staff);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *KnightIncantor::Create(const ParameterList &parameters)
{
    auto unit = new KnightIncantor();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KnightIncantor::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Knight-Incantor", factoryMethod);
    }
}

void KnightIncantor::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_staff);
}

} // namespace StormcastEternals