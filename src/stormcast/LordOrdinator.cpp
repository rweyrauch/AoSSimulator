/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordOrdinator.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordOrdinator::Create,
    LordOrdinator::ValueToString,
    LordOrdinator::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapon", {.m_intValue = LordOrdinator::AstralHammers}, LordOrdinator::AstralHammers,
            LordOrdinator::AstralGrandhammer, 1
        },
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordOrdinator::s_registered = false;

LordOrdinator::LordOrdinator() :
    StormcastEternal("Lord-Ordinator", 5, WOUNDS, 9, 4, false),
    m_astralHammers(Weapon::Type::Melee, "Astral Hammers", 1, 6, 4, 3, 0, 1),
    m_astralGrandhammer(Weapon::Type::Melee, "Astral Grandhammer", 1, 3, 3, 3, -1, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, LORD_ORDINATOR};
}

bool LordOrdinator::configure(LordOrdinator::WeaponOption weaponOption)
{
    m_weaponOption = weaponOption;

    Model model(BASESIZE, WOUNDS);
    if (m_weaponOption == AstralHammers)
    {
        model.addMeleeWeapon(&m_astralHammers);
    }
    else
    {
        model.addMeleeWeapon(&m_astralGrandhammer);
    }
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordOrdinator::Create(const ParameterList &parameters)
{
    auto unit = new LordOrdinator();
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapon", parameters, false);

    bool ok = unit->configure(weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordOrdinator::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Ordinator", factoryMethod);
    }
}

std::string LordOrdinator::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        if (parameter.m_intValue == AstralHammers)
        {
            return "Astral Hammers";
        }
        else if (parameter.m_intValue == AstralGrandhammer)
        {
            return "Astral Grandhammer";
        }
    }

    return ParameterValueToString(parameter);
}

int LordOrdinator::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Astral Hammers")
    {
        return AstralHammers;
    }
    else if (enumString == "Astral Grandhammer")
    {
        return AstralGrandhammer;
    }

    return 0;
}

void LordOrdinator::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_astralHammers);
    visitor(&m_astralGrandhammer);
}

Wounds LordOrdinator::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Comet Strike
    if (hitRoll == 6 && weapon->name() == m_astralGrandhammer.name())
    {
        return {0, 2};
    }
    if (hitRoll == 6 && weapon->name() == m_astralHammers.name())
    {
        // remember this
        m_meteoricSlam.push_back(target);
    }
    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void LordOrdinator::onStartCombat(PlayerId player)
{
    StormcastEternal::onStartCombat(player);

    m_meteoricSlam.clear();
}

void LordOrdinator::onEndCombat(PlayerId player)
{
    StormcastEternal::onEndCombat(player);

    // Meteoric Slam
    if (m_meteoricSlam.size() > 1)
    {
        // TODO: assuming all targets are the same unit and that
        // the only unit the LO hit was the target unit
        if (m_meteoricSlam.front() == m_meleeTarget)
        {
            Dice dice;
            m_meleeTarget->applyDamage({0, dice.rollD3()});
        }
    }
}

} // namespace StormcastEternals