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
    {
    }
};

bool KnightIncantor::s_registered = false;

Weapon KnightIncantor::s_staff(Weapon::Type::Melee, "Incantor's Staff", 2, 3, 3, 3, -1, RAND_D3);

KnightIncantor::KnightIncantor() :
    StormcastEternal("Knight-Incantor", 5, WOUNDS, 9, 3, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, KNIGHT_INCANTOR };
}

bool KnightIncantor::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&s_staff);
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_staff.name() << ": " << s_staff.strength() << std::endl;
    }

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

} // namespace StormcastEternals