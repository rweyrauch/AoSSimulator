/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/DarkoathWarqueen.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    DarkoathWarqueen::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool DarkoathWarqueen::s_registered = false;

Unit *DarkoathWarqueen::Create(const ParameterList &parameters)
{
    auto unit = new DarkoathWarqueen();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DarkoathWarqueen::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Darkoath Warqueen", factoryMethod);
    }
}

DarkoathWarqueen::DarkoathWarqueen() :
    Unit("Darkoath Warqueen", 6, WOUNDS, 8, 5, false),
    m_axe(Weapon::Type::Melee, "Rune-etched Axe", 1, 6, 3, 3, -1, 1)
{
    m_keywords = { CHAOS, MORTAL, SLAVES_TO_DARKNESS, EYE_OF_THE_GODS, HERO, DARKOATH_WARQUEEN };
}

bool DarkoathWarqueen::configure()
{
        auto model = new Model(BASESIZE, WOUNDS);

    model->addMeleeWeapon(&m_axe);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void DarkoathWarqueen::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_axe);
}

} //namespace SlavesToDarkness
