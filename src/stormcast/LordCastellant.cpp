/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCastellant.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordCastellant::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    {
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    { STORMCAST_ETERNAL }
};

bool LordCastellant::s_registered = false;

LordCastellant::LordCastellant() :
    StormcastEternal("Lord-Castellant", 5, WOUNDS, 9, 3, false),
    m_halberd(Weapon::Type::Melee, "Castellant's Halberd", 2, 3, 3, 3, -1, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, LORD_CASTELLANT};
    m_weapons = {&m_halberd};
}

bool LordCastellant::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_halberd);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordCastellant::Create(const ParameterList &parameters)
{
    auto unit = new LordCastellant();

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

void LordCastellant::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Castellant", factoryMethod);
    }
}

} // namespace StormcastEternals