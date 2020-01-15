/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordExorcist.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <spells/StormcastSpells.h>
#include <Board.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordExorcist::Create,
    LordExorcist::ValueToString,
    LordExorcist::EnumStringToInt,
    LordExorcist::ComputePoints,
    {
        {ParamType::Enum, "Lore of the Storm", (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::Stormcaller, 1},
        {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    { STORMCAST_ETERNAL }
};

bool LordExorcist::s_registered = false;

LordExorcist::LordExorcist() :
    StormcastEternal("Lord-Exorcist", 5, WOUNDS, 9, 3, false),
    m_stave(Weapon::Type::Melee, "Redemption Stave", 2, 4, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, LORD_EXORCIST};
    m_weapons = {&m_stave};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool LordExorcist::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration)
{
    if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None))
    {
        return false;
    }

    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_stave);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePurifyingBlast(this)));
    if (storm != LoreOfTheStorm::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordExorcist::Create(const ParameterList &parameters)
{
    auto unit = new LordExorcist();
    auto storm = (LoreOfTheStorm)GetEnumParam("Lore of the Storm", parameters, (int)LoreOfTheStorm::None);
    auto invigoration = (LoreOfInvigoration)GetEnumParam("Lore of Invigoration", parameters, (int)LoreOfInvigoration::None);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure(storm, invigoration);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordExorcist::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Exorcist", factoryMethod);
    }
}

std::string LordExorcist::ValueToString(const Parameter &parameter)
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

int LordExorcist::EnumStringToInt(const std::string &enumString)
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

void LordExorcist::onStartShooting(PlayerId player)
{
    StormcastEternal::onStartShooting(player);

    if (player == m_owningPlayer)
    {
        // Redemptor Casket
        Dice dice;
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 6.0f);
        for (auto ip : units)
        {
            if (ip->hasKeyword(DAEMON) || ip->hasKeyword(NIGHTHAUNT))
            {
                int roll = dice.rollD6();
                if (roll >= 4)
                {
                    ip->applyDamage({0, 1});
                }
            }
        }
    }
}


} // namespace StormcastEternals