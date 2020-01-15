/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/AkhelianAllopexes.h>
#include <UnitFactory.h>

namespace IdonethDeepkin
{

bool AkhelianAllopexes::s_registered = false;

static FactoryMethod factoryMethod = {
    AkhelianAllopexes::Create,
    AkhelianAllopexes::ValueToString,
    AkhelianAllopexes::EnumStringToInt,
    AkhelianAllopexes::ComputePoints,
    {
        {
            ParamType::Integer, "Models", AkhelianAllopexes::MIN_UNIT_SIZE, AkhelianAllopexes::MIN_UNIT_SIZE,
            AkhelianAllopexes::MAX_UNIT_SIZE, AkhelianAllopexes::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", AkhelianAllopexes::HarpoonLauncher, AkhelianAllopexes::HarpoonLauncher, AkhelianAllopexes::NetLauncher, 1},
    },
    ORDER,
    { IDONETH_DEEPKIN }
};


AkhelianAllopexes::AkhelianAllopexes() :
    Unit("Akhelian Alloplexes", 12, WOUNDS, 6, 4, true),
    m_harpoonLauncher(Weapon::Type::Missile, "Razorshell Harpoon Launcher", 24, 3, 3, 3, 0, 1),
    m_netLauncher(Weapon::Type::Missile, "Retarius Net Launcher", 18, 1, 3, 3, 0, 3),
    m_hooksAndBlades(Weapon::Type::Melee, "Barbed Hooks and Blades", 1, 5, 3, 4, 0, 1),
    m_allopexBite(Weapon::Type::Melee, "Allopex's Ferocious Bite", 1, 1, 3, 3, -2, 3),
    m_allopexFins(Weapon::Type::Melee, "Allopex's Scythed Fins", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, AKHELIAN, MONSTER, ALLOPEX};
    m_weapons = {&m_harpoonLauncher, &m_netLauncher, &m_hooksAndBlades, &m_allopexBite, &m_allopexFins};
}

bool AkhelianAllopexes::configure(int numModels, WeaponOption weapons)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == HarpoonLauncher)
        {
            model->addMissileWeapon(&m_harpoonLauncher);
        }
        else if (weapons == NetLauncher)
        {
            model->addMissileWeapon(&m_netLauncher);
        }
        model->addMeleeWeapon(&m_hooksAndBlades);
        model->addMeleeWeapon(&m_allopexBite);
        model->addMeleeWeapon(&m_allopexFins);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *AkhelianAllopexes::Create(const ParameterList &parameters)
{
    auto unit = new AkhelianAllopexes();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, HarpoonLauncher);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AkhelianAllopexes::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Akhelian Alloplexes", factoryMethod);
    }
}

std::string AkhelianAllopexes::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == HarpoonLauncher)
        {
            return "Razorshell Harpoon Launcher";
        }
        else if (parameter.intValue == NetLauncher)
        {
            return "Retarius Net Launcher";
        }
    }
    return ParameterValueToString(parameter);
}

int AkhelianAllopexes::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Razorshell Harpoon Launcher")
    {
        return HarpoonLauncher;
    }
    else if (enumString == "Retarius Net Launcher")
    {
        return NetLauncher;
    }
    return 0;
}

int AkhelianAllopexes::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace IdonethDeepkin