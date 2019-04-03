/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Thunderers.h>
#include <UnitFactory.h>

namespace Dispossessed
{
static FactoryMethod factoryMethod = {
    Thunderers::Create,
    Thunderers::ValueToString,
    Thunderers::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Thunderers::MIN_UNIT_SIZE, Thunderers::MIN_UNIT_SIZE,
            Thunderers::MAX_UNIT_SIZE, Thunderers::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Veteran Weapon", Thunderers::DuardinHandgun, Thunderers::DuardinHandgun,
            Thunderers::BraceOfDuardinPistols, 1
        },
        {ParamType::Boolean, "Duardin Bucklers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Drummers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE}
    },
    ORDER,
    DISPOSSESSED
};

bool Thunderers::s_registered = false;

Thunderers::Thunderers() :
    Dispossessed("Thunderers", 4, WOUNDS, 6, 5, false),
    m_duardinHandgun(Weapon::Type::Missile, "Duardin Handgun", 16, 1, 4, 3, -1, 1),
    m_duardinHandgunVeteran(Weapon::Type::Missile, "Duardin Handgun (Veteran)", 16, 1, 3, 3, -1, 1),
    m_braceOfDuardinPistols(Weapon::Type::Missile, "Brace of Duardin Pistols", 8, 2, 4, 3, -1, 1),
    m_duardinHandgunMelee(Weapon::Type::Melee, "Duardin Handgun", 1, 1, 4, 5, 0, 1),
    m_braceOfDuardinPistolsMelee(Weapon::Type::Melee, "Brace of Duardin Pistols", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, THUNDERERS};
}

bool Thunderers::configure(int numModels, WeaponOptions veteranWeapon, bool duardinBucklers, bool standardBearer, bool drummers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_drummers = drummers;
    m_duardinBucklers = duardinBucklers;

    Model veteran(BASESIZE, WOUNDS);
    if (veteranWeapon == DuardinHandgun)
    {
        veteran.addMissileWeapon(&m_duardinHandgunVeteran);
        veteran.addMeleeWeapon(&m_duardinHandgunMelee);
    }
    else if (veteranWeapon == BraceOfDuardinPistols)
    {
        veteran.addMissileWeapon(&m_braceOfDuardinPistols);
        veteran.addMeleeWeapon(&m_braceOfDuardinPistolsMelee);
    }
    addModel(veteran);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_duardinHandgun);
        model.addMeleeWeapon(&m_duardinHandgunMelee);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Thunderers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_duardinHandgun);
    visitor(&m_duardinHandgunVeteran);
    visitor(&m_braceOfDuardinPistols);
    visitor(&m_duardinHandgunMelee);
    visitor(&m_braceOfDuardinPistolsMelee);
}

Unit *Thunderers::Create(const ParameterList &parameters)
{
    auto unit = new Thunderers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapon = (WeaponOptions)GetEnumParam("Veteran Weapon", parameters, DuardinHandgun);
    bool duardinBucklers = GetBoolParam("Duardin Bucklers", parameters, false);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool drummer = GetBoolParam("Drummer", parameters, false);

    bool ok = unit->configure(numModels, weapon, duardinBucklers, standardBearer, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Thunderers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Thunderers", factoryMethod);
    }
}

std::string Thunderers::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Veteran Weapon")
    {
        if (parameter.m_intValue == DuardinHandgun)
        {
            return "Duardin Handgun";
        }
        else if (parameter.m_intValue == BraceOfDuardinPistols)
        {
            return "Brace Of Duardin Pistols";
        }
    }

    return ParameterValueToString(parameter);
}

int Thunderers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Duardin Handgun")
    {
        return DuardinHandgun;
    }
    else if (enumString == "Brace Of Duardin Pistols")
    {
        return BraceOfDuardinPistols;
    }
    return 0;
}

} // namespace Dispossessed
