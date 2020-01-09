/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordArcanumOnGryphcharger.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <spells/StormcastSpells.h>

#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordArcanumOnGryphcharger::Create,
    LordArcanumOnGryphcharger::ValueToString,
    LordArcanumOnGryphcharger::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Storm", (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::Stormcaller, 1},
        {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool LordArcanumOnGryphcharger::s_registered = false;

LordArcanumOnGryphcharger::LordArcanumOnGryphcharger() :
    StormcastEternal("Lord-Arcanum on Gryph-charger", 12, WOUNDS, 9, 3, false),
    m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
    m_beakAndClaws(Weapon::Type::Melee, "Razor Beak and Claws", 1, 3, 3, 3, -2, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, GRYPH_CHARGER, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, LORD_ARCANUM};
    m_weapons = {&m_aetherstave, &m_beakAndClaws};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool LordArcanumOnGryphcharger::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration)
{
    if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None))
    {
        return false;
    }

    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_aetherstave);
    model->addMeleeWeapon(&m_beakAndClaws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateHealingLight(this)));
    if (storm != LoreOfTheStorm::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *LordArcanumOnGryphcharger::Create(const ParameterList &parameters)
{
    auto unit = new LordArcanumOnGryphcharger();
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

void LordArcanumOnGryphcharger::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Arcanum on Gryph-charger", factoryMethod);
    }
}

std::string LordArcanumOnGryphcharger::ValueToString(const Parameter &parameter)
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

int LordArcanumOnGryphcharger::EnumStringToInt(const std::string &enumString)
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

Wounds LordArcanumOnGryphcharger::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Aethereal Strike
    if ((hitRoll == 6) && (weapon->name() == m_beakAndClaws.name()))
    {
        return {0, 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void LordArcanumOnGryphcharger::onStartCombat(PlayerId player)
{
    // Spirit Flask
    if (!m_shatteredFlasks)
    {
        m_shatteredFlasks = DoSpiritFlasks(this);
    }
}

} // namespace StormcastEternals