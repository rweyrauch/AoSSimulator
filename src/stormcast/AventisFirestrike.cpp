/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/AventisFirestrike.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <spells/StormcastSpells.h>
#include <Board.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    AventisFirestrike::Create,
    AventisFirestrike::ValueToString,
    AventisFirestrike::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Storm", (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::Stormcaller, 1},
        {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool AventisFirestrike::s_registered = false;

AventisFirestrike::AventisFirestrike() :
    StormcastEternal("Aventis Firestrike", 14, WOUNDS, 9, 3, true),
    m_staffOfHammerhal(Weapon::Type::Melee, "Staff of Hammerhal", 2, 4, 3, 3, -1, RAND_D3),
    m_hornsAndHooves(Weapon::Type::Melee, "Horns and Stamping Hooves", 1, 4, 3, 3, -1, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, TAURALON, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, HERO, MONSTER, WIZARD, LORD_ARCANUM, AVENTIS_FIRESTRIKE};
    m_weapons = {&m_staffOfHammerhal, &m_hornsAndHooves};
}

bool AventisFirestrike::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration)
{
    if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None))
    {
        return false;
    }

    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staffOfHammerhal);
    model->addMeleeWeapon(&m_hornsAndHooves);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePyroelectricBlast(this)));
    if (storm != LoreOfTheStorm::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *AventisFirestrike::Create(const ParameterList &parameters)
{
    auto unit = new AventisFirestrike();
    auto storm = (LoreOfTheStorm)GetEnumParam("Lore of the Storm", parameters, (int)LoreOfTheStorm::None);
    auto invigoration = (LoreOfInvigoration)GetEnumParam("Lore of Invigoration", parameters, (int)LoreOfInvigoration::None);

    unit->setStormhost(HammersOfSigmar);

    bool ok = unit->configure(storm, invigoration);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AventisFirestrike::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Aventis Firestrike", factoryMethod);
    }
}

std::string AventisFirestrike::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Lore of the Storm")
    {
        return ToString((LoreOfTheStorm) parameter.intValue);
    }
    else if (std::string(parameter.name) == "Lore of Invigoration")
    {
        return ToString((LoreOfInvigoration) parameter.intValue);
    }
    return StormcastEternal::ValueToString(parameter);
}

int AventisFirestrike::EnumStringToInt(const std::string &enumString)
{
    LoreOfTheStorm storm;
    LoreOfInvigoration invigoration;
    if (FromString(enumString, storm))
    {
        return (int)storm;
    }
    else if (FromString(enumString, invigoration))
    {
        return (int) invigoration;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

void AventisFirestrike::onStartCombat(PlayerId player)
{
    // Spirit Flask
    if (!m_shatteredFlasks)
    {
        m_shatteredFlasks = DoSpiritFlasks(this);
    }
}

void AventisFirestrike::onCharged()
{
    StormcastEternal::onCharged();

    // Meteoric Strike
    Dice dice;
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
    for (auto ip : units)
    {
        if (dice.rollD6() >= 2)
        {
            ip->applyDamage({0, 1});
        }
    }
}

void AventisFirestrike::onStartHero(PlayerId player)
{
    StormcastEternal::onStartHero(player);

    // Thunderhead Crown
    if (player == m_owningPlayer)
    {
        heal(1);
    }
}

} // namespace StormcastEternals