/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordRelictor.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordRelictor::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    {
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordRelictor::s_registered = false;

LordRelictor::LordRelictor() :
    StormcastEternal("Lord-Relictor", 5, WOUNDS, 9, 3, false),
    m_relicHammer(Weapon::Type::Melee, "Relic Hammer", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, PRIEST, LORD_RELICTOR};
}

bool LordRelictor::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_relicHammer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordRelictor::Create(const ParameterList &parameters)
{
    auto unit = new LordRelictor();

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

void LordRelictor::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Relictor", factoryMethod);
    }
}

void LordRelictor::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_relicHammer);
}

} // namespace StormcastEternals