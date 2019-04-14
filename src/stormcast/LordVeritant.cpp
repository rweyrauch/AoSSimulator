/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordVeritant.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordVeritant::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordVeritant::s_registered = false;

LordVeritant::LordVeritant() :
    StormcastEternal("Lord-Veritant", 5, WOUNDS, 9, 3, false),
    m_judgementBlade(Weapon::Type::Melee, "Judgement Blade", 1, 4, 3, 3, -1, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, PRIEST, LORD_VERITANT};
}

bool LordVeritant::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_judgementBlade);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordVeritant::Create(const ParameterList &parameters)
{
    auto unit = new LordVeritant();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordVeritant::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Veritant", factoryMethod);
    }
}

void LordVeritant::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_judgementBlade);
}

} // namespace StormcastEternals