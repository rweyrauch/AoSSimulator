/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/FungoidCaveShaman.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <spells/MysticShield.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    FungoidCaveShaman::Create,
    FungoidCaveShaman::ValueToString,
    FungoidCaveShaman::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Moonclans", (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::CallDaMoon, 1},
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool FungoidCaveShaman::s_registered = false;

FungoidCaveShaman::FungoidCaveShaman() :
    GloomspiteGitzBase("Fungoid Cave-shaman", 5, WOUNDS, 4, 6, false),
    m_moonSickle(Weapon::Type::Melee, "Moon-sickle", 1, 3, 4, 4, -1, 1),
    m_squigsTeeth(Weapon::Type::Melee, "Spore Squig's Vicious Teeth", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, FUNGOID_CAVE_SHAMAN};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool FungoidCaveShaman::configure(LoreOfTheMoonclans lore)
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_moonSickle);
    model.addMeleeWeapon(&m_squigsTeeth);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    //m_knownSpells.push_back(std::make_unique<SporeMaws>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheMoonclans(lore, this)));

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *FungoidCaveShaman::Create(const ParameterList &parameters)
{
    auto unit = new FungoidCaveShaman();
    auto lore = (LoreOfTheMoonclans)GetEnumParam("Lore of the Moonclans", parameters, (int)LoreOfTheMoonclans ::None);

    bool ok = unit->configure(lore);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void FungoidCaveShaman::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Fungoid Cave-shaman", factoryMethod);
    }
}

void FungoidCaveShaman::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_moonSickle);
    visitor(&m_squigsTeeth);
}

std::string FungoidCaveShaman::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Lore of the Moonclans")
    {
        return ToString((LoreOfTheMoonclans)parameter.m_intValue);
    }
    return ParameterValueToString(parameter);
}

int FungoidCaveShaman::EnumStringToInt(const std::string &enumString)
{
    LoreOfTheMoonclans lore;
    if (FromString(enumString, lore))
    {
        return (int) lore;
    }
    return 0;
}

} // namespace GloomspiteGitz
