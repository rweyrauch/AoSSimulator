/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/GavrielSureheart.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    GavrielSureheart::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    {
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool GavrielSureheart::s_registered = false;

GavrielSureheart::GavrielSureheart() :
    StormcastEternal("Gavriel Sureheart", 5, WOUNDS, 9, 3, false),
    m_starboundBlade(Weapon::Type::Melee, "Tempest Axe", 1, 6, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, HERO, LORD_CELESTANT, GAVRIEL_SUREHEART};
}

bool GavrielSureheart::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_starboundBlade);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *GavrielSureheart::Create(const ParameterList &parameters)
{
    auto unit = new GavrielSureheart();

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

void GavrielSureheart::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gavriel Sureheart", factoryMethod);
    }
}

void GavrielSureheart::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_starboundBlade);
}

Rerolls GavrielSureheart::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Thundershield
    return RerollOnes;
}

Wounds GavrielSureheart::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    auto wounds = StormcastEternal::computeReturnedDamage(weapon, saveRoll);
    // Sigmarite Thundershield
    // 1 mortal wound for each save of a 6
    if (saveRoll == 6)
    {
        wounds += {0, 1};
    }
    return wounds;
}

int GavrielSureheart::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);
    if (m_charged)
    {
        attacks += 1;
    }
    return attacks;
}

} // namespace StormcastEternals