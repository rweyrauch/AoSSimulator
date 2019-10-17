/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Slambo.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    Slambo::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool Slambo::s_registered = false;

Unit *Slambo::Create(const ParameterList &parameters)
{
    auto unit = new Slambo();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Slambo::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Slambo", factoryMethod);
    }
}

Slambo::Slambo() :
    Unit("Slambo", 5, WOUNDS, 8, 4, false),
    m_hurledAxe(Weapon::Type::Missile, "Hurled Chaos Axe", 8, 1, 3, 3, -1, RAND_D3),
    m_chaosAxes(Weapon::Type::Melee, "Chaos Axes", 1, RAND_D6, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, EXALTED_HERO_OF_CHAOS, SLAMBO};
}

bool Slambo::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMissileWeapon(&m_hurledAxe);
    model.addMeleeWeapon(&m_chaosAxes);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Slambo::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_hurledAxe);
    visitor(m_chaosAxes);
}

} //SlavesToDarkness