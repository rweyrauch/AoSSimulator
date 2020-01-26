/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "everchosen/Varanguard.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    Varanguard::Create,
    Varanguard::ValueToString,
    Varanguard::EnumStringToInt,
    Varanguard::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Varanguard::MIN_UNIT_SIZE, Varanguard::MIN_UNIT_SIZE,
            Varanguard::MAX_UNIT_SIZE, Varanguard::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", Varanguard::DaemonforgedBlade, Varanguard::EnsorcelledWeapon,
            Varanguard::DaemonforgedBlade, 1
        },
        {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
        {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
    },
    CHAOS,
    { SLAVES_TO_DARKNESS, KHORNE, SLAANESH, TZEENTCH, NURGLE }
};

bool Varanguard::s_registered = false;

Unit *Varanguard::Create(const ParameterList &parameters)
{
    auto unit = new Varanguard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, DaemonforgedBlade);

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    auto mark = (MarkOfChaos)GetEnumParam("Mark of Chaos", parameters, Undivided);
    unit->setMarkOfChaos(mark);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Varanguard::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        switch (parameter.intValue)
        {
            case EnsorcelledWeapon: return "Ensorcelled Weapon";
            case Fellspear: return "Fellspear";
            case DaemonforgedBlade: return "Daemonforged Blade";
        }
    }
    return SlavesToDarknessBase::ValueToString(parameter);
}

int Varanguard::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Ensorcelled Weapon") return EnsorcelledWeapon;
    else if (enumString == "Fellspear") return Fellspear;
    else if (enumString == "Daemonforged Blade") return DaemonforgedBlade;

    return SlavesToDarknessBase::EnumStringToInt(enumString);
}

void Varanguard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Varanguard", factoryMethod);
    }
}

Varanguard::Varanguard() :
    SlavesToDarknessBase("Varanguard", 10, WOUNDS, 9, 3, false),
    m_ensorcelledWeapon(Weapon::Type::Melee, "Ensorcelled Weapon", 1, 6, 3, 3, -1, 1),
    m_fellspear(Weapon::Type::Melee, "Fellspear", 2, 3, 3, 4, -1, 2),
    m_blade(Weapon::Type::Melee, "Daemonforged Blade", 1, 3, 3, 3, -1, RAND_D3),
    m_fangs(Weapon::Type::Melee, "Tearing Fangs", 1, 3, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, EVERCHOSEN, MARK_OF_CHAOS, VARANGUARD};
    m_weapons = {&m_ensorcelledWeapon, &m_fellspear, &m_blade, &m_fangs};
}

bool Varanguard::configure(int numModels, Varanguard::WeaponOption weapon)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapon == EnsorcelledWeapon)
        {
            model->addMeleeWeapon(&m_ensorcelledWeapon);
        }
        else if (weapon == Fellspear)
        {
            model->addMeleeWeapon(&m_fellspear);
        }
        else if (weapon == DaemonforgedBlade)
        {
            model->addMeleeWeapon(&m_blade);
        }
        model->addMeleeWeapon(&m_fangs);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Wounds Varanguard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    // Daemonbound
    if ((hitRoll == 6) && (weapon->name() == m_blade.name()))
    {
        damage.mortal++;
    }
    return damage;
}

int Varanguard::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);

    // Impaling Charge
    if (m_charged && (weapon->name() == m_fellspear.name()))
    {
        rend -= 1;
    }
    return rend;
}

int Varanguard::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toWoundModifier(weapon, target);

    // Impaling Charge
    if (m_charged && (weapon->name() == m_fellspear.name()))
    {
        mod++;
    }

    return mod;
}

int Varanguard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace SlavesToDarkness