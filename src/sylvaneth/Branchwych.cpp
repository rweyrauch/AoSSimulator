/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Branchwych.h>
#include <UnitFactory.h>
#include <iostream>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    Branchwych::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    SYLVANETH
};

bool Branchwych::s_registered = false;

Branchwych::Branchwych() :
    Unit("Branchwych", 7, WOUNDS, 7, 5, false),
    m_greenwoodScythe(Weapon::Type::Melee, "Greenwood Scythe", 2, 2, 4, 3, 0, 2),
    m_bittergrubsMandibles(Weapon::Type::Melee, "Bittergrub's Mandibles", 1, 1, 4, 4, -1, 1)
{
    m_keywords = {ORDER, SYLVANETH, HERO, WIZARD, BRANCHWYCH};
}

bool Branchwych::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_greenwoodScythe);
    model.addMeleeWeapon(&m_bittergrubsMandibles);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Branchwych::Create(const ParameterList &parameters)
{
    auto unit = new Branchwych();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Branchwych::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Branchwych", factoryMethod);
    }
}

void Branchwych::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_greenwoodScythe);
    visitor(&m_bittergrubsMandibles);
}

} // namespace Sylvaneth
