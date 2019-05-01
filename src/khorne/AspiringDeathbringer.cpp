/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/AspiringDeathbringer.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    AspiringDeathbringer::Create,
    AspiringDeathbringer::ValueToString,
    AspiringDeathbringer::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapon", AspiringDeathbringer::BloodaxeAndWrathhammer, AspiringDeathbringer::BloodaxeAndWrathhammer,
            AspiringDeathbringer::GoreaxeAndSkullhammer, 1
        },
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool AspiringDeathbringer::s_registered = false;

AspiringDeathbringer::AspiringDeathbringer() :
    KhorneBase("Aspiring Deathbringer", 5, WOUNDS, 7, 4, false),
    m_bloodAxe(Weapon::Type::Melee, "Bloodaxe", 1, 3, 3, 4, 0, 1),
    m_wrathHammer(Weapon::Type::Melee, "Wrath-hammer", 3, RAND_D3, 3, 4, 0, 1),
    m_goreaxe(Weapon::Type::Melee, "Goreaxe", 1, 3, 3, 4, 0, 1),
    m_skullhammer(Weapon::Type::Melee, "Skullhammer", 3, 3, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, ASPIRING_DEATHBRINGER};
}

bool AspiringDeathbringer::configure(WeaponOption weapon)
{
    m_weaponOption = weapon;

    Model model(BASESIZE, WOUNDS);

    if (m_weaponOption == BloodaxeAndWrathhammer)
    {
        model.addMeleeWeapon(&m_bloodAxe);
        model.addMeleeWeapon(&m_wrathHammer);
        m_points = POINTS_PER_UNIT;
    }
    else if (m_weaponOption == GoreaxeAndSkullhammer)
    {
        model.addMeleeWeapon(&m_goreaxe);
        model.addMeleeWeapon(&m_skullhammer);
        m_points = POINTS_PER_UNIT_WITH_GOREAXE;
    }
    addModel(model);

    return true;
}

void AspiringDeathbringer::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bloodAxe);
    visitor(&m_wrathHammer);
    visitor(&m_goreaxe);
    visitor(&m_skullhammer);
}

Unit *AspiringDeathbringer::Create(const ParameterList &parameters)
{
    auto unit = new AspiringDeathbringer();
    WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, BloodaxeAndWrathhammer);

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

void AspiringDeathbringer::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Aspiring Deathbringer", factoryMethod);
    }
}

std::string AspiringDeathbringer::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        if (parameter.m_intValue == BloodaxeAndWrathhammer)
        {
            return "Bloodaxe and Wrath-hammer";
        }
        else if (parameter.m_intValue == GoreaxeAndSkullhammer)
        {
            return "Goreaxe and Skullhammer";
        }
    }
    return KhorneBase::ValueToString(parameter);
}

int AspiringDeathbringer::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Bloodaxe and Wrath-hammer")
    {
        return BloodaxeAndWrathhammer;
    }
    else if (enumString == "Goreaxe and Skullhammer")
    {
        return GoreaxeAndSkullhammer;
    }
    return KhorneBase::EnumStringToInt(enumString);
}


} // namespace Khorne