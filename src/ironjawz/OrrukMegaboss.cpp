/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukMegaboss.h>
#include <UnitFactory.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    OrrukMegaboss::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    IRONJAWZ
};

bool OrrukMegaboss::s_registered = false;

OrrukMegaboss::OrrukMegaboss() :
    Ironjawz("Orruk Megaboss", 4, WOUNDS, 8, 3, false),
    m_bossChoppa(Weapon::Type::Melee, "Boss Choppa", 1, 6, 3, 3, -1, 2),
    m_riptoothFist(Weapon::Type::Melee, "Rip-tooth Fist", 1, 2, 5, 3, -2, RAND_D3)
{
    m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, MEGABOSS};
}

bool OrrukMegaboss::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_bossChoppa);
    model.addMeleeWeapon(&m_riptoothFist);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void OrrukMegaboss::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bossChoppa);
    visitor(&m_riptoothFist);
}

Unit *OrrukMegaboss::Create(const ParameterList &parameters)
{
    auto unit = new OrrukMegaboss();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void OrrukMegaboss::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruk Megaboss", factoryMethod);
    }
}

} //namespace Ironjawz