/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/MadcapShaman.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <spells/MysticShield.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    MadcapShaman::Create,
    MadcapShaman::ValueToString,
    MadcapShaman::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Moonclans", (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::CallDaMoon, 1},
    },
    DESTRUCTION,
    { GLOOMSPITE_GITZ }
};

bool MadcapShaman::s_registered = false;

MadcapShaman::MadcapShaman() :
    GloomspiteGitzBase("Madcap Shaman", 5, WOUNDS, 4, 6, false),
    m_moonStaff(Weapon::Type::Melee, "Moon Staff", 2, 1, 4, 4, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, MADCAP_SHAMAN};
    m_weapons = {&m_moonStaff};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool MadcapShaman::configure(LoreOfTheMoonclans lore)
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_moonStaff);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    //m_knownSpells.push_back(std::make_unique<NightShroud>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheMoonclans(lore, this)));

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *MadcapShaman::Create(const ParameterList &parameters)
{
    auto unit = new MadcapShaman();
    auto lore = (LoreOfTheMoonclans)GetEnumParam("Lore of the Moonclans", parameters, (int)LoreOfTheMoonclans ::None);

    bool ok = unit->configure(lore);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void MadcapShaman::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Madcap Shaman", factoryMethod);
    }
}

std::string MadcapShaman::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Lore of the Moonclans")
    {
        return ToString((LoreOfTheMoonclans)parameter.intValue);
    }
    return ParameterValueToString(parameter);
}

int MadcapShaman::EnumStringToInt(const std::string &enumString)
{
    LoreOfTheMoonclans lore;
    if (FromString(enumString, lore))
    {
        return (int) lore;
    }
    return 0;
}

} // namespace GloomspiteGitz
