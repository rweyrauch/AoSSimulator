/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Slaughterpriest.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Slaughterpriest::Create,
    Slaughterpriest::ValueToString,
    Slaughterpriest::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapon", Slaughterpriest::BloodbathedAxe, Slaughterpriest::BloodbathedAxe,
            Slaughterpriest::HackbladeAndWrathHammer, 1
        },
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool Slaughterpriest::s_registered = false;

Slaughterpriest::Slaughterpriest() :
    KhorneBase("Slaughterpriest", 6, WOUNDS, 8, 5, false),
    m_bloodbathedAxe(Weapon::Type::Melee, "Bloodbathed Axe", 2, 3, 4, 3, 0, 2),
    m_hackblade(Weapon::Type::Melee, "Hackblade", 1, 3, 3, 4, 0, 1),
    m_wrathHammer(Weapon::Type::Melee, "Wrath-hammer", 3, RAND_D3, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, PRIEST, SLAUGHTERPRIEST};

    // Collar of Khorne
    m_totalUnbinds = 1;
}

bool Slaughterpriest::configure(WeaponOption weapon)
{
    Model model(BASESIZE, WOUNDS);

    if (weapon == BloodbathedAxe)
    {
        model.addMeleeWeapon(&m_bloodbathedAxe);
    }
    else
    {
        model.addMeleeWeapon(&m_hackblade);
        model.addMeleeWeapon(&m_wrathHammer);
    }
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Slaughterpriest::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bloodbathedAxe);
    visitor(&m_hackblade);
    visitor(&m_wrathHammer);
}

Unit *Slaughterpriest::Create(const ParameterList &parameters)
{
    auto unit = new Slaughterpriest();
    WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, BloodbathedAxe);

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure(weapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Slaughterpriest::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Slaughterpriest", factoryMethod);
    }
}

std::string Slaughterpriest::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        if (parameter.m_intValue == BloodbathedAxe) { return "Bloodbathed Axe"; }
        else if (parameter.m_intValue == HackbladeAndWrathHammer) { return "Hackblade and Wrath-hammer"; }
    }
    return KhorneBase::ValueToString(parameter);
}

int Slaughterpriest::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Bloodbathed Axe") { return BloodbathedAxe; }
    else if (enumString == "Hackblade and Wrath-hammer") { return HackbladeAndWrathHammer; }
    return KhorneBase::EnumStringToInt(enumString);
}

} // namespace Khorne