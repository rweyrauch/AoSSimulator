/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordArcanum.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <spells/StormcastSpells.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordArcanum::Create,
    LordArcanum::ValueToString,
    LordArcanum::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Storm", (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::Stormcaller, 1},
        {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordArcanum::s_registered = false;

LordArcanum::LordArcanum() :
    StormcastEternal("Lord-Arcanum", 5, WOUNDS, 9, 3, false),
    m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, LORD_ARCANUM};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool LordArcanum::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration)
{
    if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None))
    {
        return false;
    }

    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_aetherstave);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateThunderclap(this)));
    if (storm != LoreOfTheStorm::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordArcanum::Create(const ParameterList &parameters)
{
    auto unit = new LordArcanum();
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

void LordArcanum::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Arcanum", factoryMethod);
    }
}

std::string LordArcanum::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Lore of the Storm")
    {
        return ToString((LoreOfTheStorm) parameter.m_intValue);
    }
    else if (parameter.m_name == "Lore of Invigoration")
    {
        return ToString((LoreOfInvigoration) parameter.m_intValue);
    }
    return StormcastEternal::ValueToString(parameter);
}

int LordArcanum::EnumStringToInt(const std::string &enumString)
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

void LordArcanum::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_aetherstave);
}

void LordArcanum::onStartCombat(PlayerId player)
{
    // Spirit Flask
    if (!m_shatteredFlasks)
    {
        m_shatteredFlasks = DoSpiritFlasks(this);
    }
}

} // namespace StormcastEternals