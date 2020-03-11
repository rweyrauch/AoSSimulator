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
#include <Roster.h>
#include <iostream>
#include <Board.h>
#include <spells/MysticShield.h>

namespace GloomspiteGitz
{

bool FungoidCaveShaman::s_registered = false;

FungoidCaveShaman::FungoidCaveShaman() :
    GloomspiteGitzBase("Fungoid Cave-shaman", 5, WOUNDS, 4, 6, false),
    m_moonSickle(Weapon::Type::Melee, "Moon-sickle", 1, 3, 4, 4, -1, 1),
    m_squigsTeeth(Weapon::Type::Melee, "Spore Squig's Vicious Teeth", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, FUNGOID_CAVE_SHAMAN};
    m_weapons = {&m_moonSickle, &m_squigsTeeth};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool FungoidCaveShaman::configure(LoreOfTheMoonclans lore)
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_moonSickle);
    model->addMeleeWeapon(&m_squigsTeeth);

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
        static auto factoryMethod = new FactoryMethod{
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Lore of the Moonclans", (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::CallDaMoon, 1},
            },
            DESTRUCTION,
            { GLOOMSPITE_GITZ }
        };

        s_registered = UnitFactory::Register("Fungoid Cave-shaman", *factoryMethod);
    }
}

std::string FungoidCaveShaman::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Lore of the Moonclans")
    {
        return ToString((LoreOfTheMoonclans)parameter.intValue);
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

void FungoidCaveShaman::onStartHero(PlayerId playerId)
{
    GloomspiteGitzBase::onStartHero(playerId);

    // Mouthpiece of Mork
    if ((owningPlayer() == playerId) && m_roster)
    {
        Dice dice;
        if (dice.rollD6() >= 4)
        {
            m_roster->addCommandPoints(1);
        }
    }

}

} // namespace GloomspiteGitz
