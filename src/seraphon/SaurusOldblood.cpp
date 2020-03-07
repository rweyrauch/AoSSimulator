/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusOldblood.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SaurusOldblood::Create,
    SaurusOldblood::ValueToString,
    SaurusOldblood::EnumStringToInt,
    SaurusOldblood::ComputePoints,
    {
        {ParamType::Enum, "Weapon", SaurusOldblood::CelestiteWarblade, SaurusOldblood::CelestiteMaul, SaurusOldblood::CelestiteGreatblade, 1}
    },
    ORDER,
    { SERAPHON }
};

bool SaurusOldblood::s_registered = false;

SaurusOldblood::SaurusOldblood() :
    SeraphonBase("Saurus Oldblood", 5, WOUNDS, 8, 3, false),
    m_maul(Weapon::Type::Melee, "Celestite Maul", 1, 3, 4, 3, -1, RAND_D3),
    m_warblade(Weapon::Type::Melee, "Celestite Warblade", 1, 5, 3, 3, 0, 1),
    m_warspear(Weapon::Type::Melee, "Celestite Warspear", 2, 4, 3, 3, 0, 1),
    m_greatblade(Weapon::Type::Melee, "Celestite Greatblade", 1, 3, 4, 3, -1, 2),
    m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SAURUS, HERO, OLDBLOOD};
    m_weapons = {&m_maul, &m_warblade, &m_warspear, &m_greatblade, &m_jaws};
}

bool SaurusOldblood::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);
    if (option == CelestiteMaul)
    {
        model->addMeleeWeapon(&m_maul);
    }
    else if (option == CelestiteWarblade)
    {
        model->addMeleeWeapon(&m_warblade);
    }
    else if (option == CelestiteWarspear)
    {
        model->addMeleeWeapon(&m_warspear);
    }
    else if (option == CelestiteGreatblade)
    {
        model->addMeleeWeapon(&m_greatblade);
    }
    model->addMeleeWeapon(&m_jaws);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SaurusOldblood::Create(const ParameterList &parameters)
{
    auto unit = new SaurusOldblood();
    auto option = (WeaponOption)GetEnumParam("Weapon", parameters, CelestiteWarblade);

    bool ok = unit->configure(option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SaurusOldblood::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Saurus Oldblood", factoryMethod);
    }
}

std::string SaurusOldblood::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == CelestiteMaul) return "Celestite Maul";
        else if (parameter.intValue == CelestiteWarblade) return "Celestite Warblade";
        else if (parameter.intValue == CelestiteWarspear) return "Celestite War-spear";
        else if (parameter.intValue == CelestiteGreatblade) return "Celestite Greatblade";
    }
    return SeraphonBase::ValueToString(parameter);
}

int SaurusOldblood::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Celestite Maul") return CelestiteMaul;
    else if (enumString == "Celestite Warblade") return CelestiteWarblade;
    else if (enumString == "Celestite War-spear") return CelestiteWarspear;
    else if (enumString == "Celestite Greatblade") return CelestiteGreatblade;
    return SeraphonBase::EnumStringToInt(enumString);
}

} //namespace Seraphon
