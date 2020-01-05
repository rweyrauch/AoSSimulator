/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/DarkoathChieftain.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    DarkoathChieftain::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool DarkoathChieftain::s_registered = false;

Unit *DarkoathChieftain::Create(const ParameterList &parameters)
{
    auto unit = new DarkoathChieftain();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DarkoathChieftain::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Darkoath Chieftain", factoryMethod);
    }
}

DarkoathChieftain::DarkoathChieftain() :
    Unit("Darkoath Chieftain", 6, WOUNDS, 8, 5, false),
    m_axe(Weapon::Type::Melee, "Warlord Axe", 1, 1, 3, 3, 0, 1),
    m_broadsword(Weapon::Type::Melee, "Cursed Broadsword", 1, 3, 4, 3, -1, 2)
{
    m_keywords = { CHAOS, MORTAL, SLAVES_TO_DARKNESS, EYE_OF_THE_GODS, HERO, DARKOATH_CHIEFTAIN };
    m_weapons = {&m_axe, &m_broadsword};
}

bool DarkoathChieftain::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_axe);
    model->addMeleeWeapon(&m_broadsword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //namespace SlavesToDarkness
