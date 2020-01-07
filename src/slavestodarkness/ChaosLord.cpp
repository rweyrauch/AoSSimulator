/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosLord.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosLord::Create,
    ChaosLord::ValueToString,
    ChaosLord::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapon", ChaosLord::Reaperblade, ChaosLord::Reaperblade,
            ChaosLord::DaemonboundWarFlail, 1
        },
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool ChaosLord::s_registered = false;

Unit *ChaosLord::Create(const ParameterList &parameters)
{
    auto unit = new ChaosLord();

    auto weapon = (WeaponOption)GetEnumParam("Weapon", parameters, Reaperblade);

    bool ok = unit->configure(weapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosLord::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Lord", factoryMethod);
    }
}

ChaosLord::ChaosLord() :
    Unit("Chaos Lord", 5, WOUNDS, 8, 4, false),
    m_blade(Weapon::Type::Melee, "Reaperblade", 2, 3, 3, 3, -2, 2),
    m_steel(Weapon::Type::Melee, "Daemonbound Steel", 1, 3, 3, 3, -1, 1),
    m_flail(Weapon::Type::Melee, "Daemonbound War-flail", 2, 6, 4, 4, -2, 1)
{
    m_keywords = { CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, CHAOS_LORD };
    m_weapons = {&m_blade, &m_steel, &m_flail};
}

bool ChaosLord::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);

    if (option == Reaperblade)
        model->addMeleeWeapon(&m_blade);
    else if (option == DaemonboundSteel)
        model->addMeleeWeapon(&m_steel);
    else if (option == DaemonboundWarFlail)
        model->addMeleeWeapon(&m_flail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds ChaosLord::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    // Daemonbound
    if ((hitRoll == 6) && ((weapon->name() == m_steel.name())|| (weapon->name() == m_flail.name())))
    {
        damage.mortal++;
    }
    return damage;
}

std::string ChaosLord::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        switch (parameter.m_intValue)
        {
            case Reaperblade: return "Reaperblade";
            case DaemonboundSteel: return "Daemonbound Steel";
            case DaemonboundWarFlail: return "Daemonbound War-flail";
        }
    }
    return ParameterValueToString(parameter);
}

int ChaosLord::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Reaperblade") return Reaperblade;
    else if (enumString == "Daemonbound Steel") return DaemonboundSteel;
    else if (enumString == "Daemonbound War-flail") return DaemonboundWarFlail;

    return 0;
}

} //namespace SlavesToDarkness