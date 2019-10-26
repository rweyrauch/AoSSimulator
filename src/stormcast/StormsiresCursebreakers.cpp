/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/StormsiresCursebreakers.h>
#include <iostream>
#include <spells/Empower.h>

namespace StormcastEternals
{

static FactoryMethod factoryMethod = {
    StormsiresCursebreakers::Create,
    StormsiresCursebreakers::ValueToString,
    StormsiresCursebreakers::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool StormsiresCursebreakers::s_registered = false;

StormsiresCursebreakers::StormsiresCursebreakers() :
    StormcastEternal("Stormsire's Cursebreakers", 5, WOUNDS, 8, 4, false),
    m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, CORPUSCANT, WIZARD, EVOCATORS};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool StormsiresCursebreakers::configure(LoreOfInvigoration invigoration)
{
    Model ammis(BASESIZE, WOUNDS);
    ammis.addMeleeWeapon(&m_tempestBladeAndStave);
    addModel(ammis);

    Model rastus(BASESIZE, WOUNDS);
    rastus.addMeleeWeapon(&m_tempestBladeAndStave);
    addModel(rastus);

    m_knownSpells.push_back(std::make_unique<Empower>(this));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Rerolls StormsiresCursebreakers::toSaveRerolls(const Weapon *weapon) const
{
    // Celestial Lightning Arc
    if (weapon->isMissile())
    {
        return RerollOnes;
    }

    return StormcastEternal::toSaveRerolls(weapon);
}

int StormsiresCursebreakers::generateMortalWounds(const Unit *unit)
{
    auto mortalWounds = StormcastEternal::generateMortalWounds(unit);

    // Celestial Lightning Arc
    Dice dice;
    int roll = dice.rollD6();
    if (roll >= 4)
    {
        mortalWounds++;
    }
    roll = dice.rollD6();
    if (roll >= 4)
    {
        mortalWounds++;
    }

    return mortalWounds;
}

Unit *StormsiresCursebreakers::Create(const ParameterList &parameters)
{
    auto *evos = new StormsiresCursebreakers();
    auto invigoration = (LoreOfInvigoration)GetEnumParam("Lore of Invigoration", parameters, (int)LoreOfInvigoration::None);

    evos->setStormhost(HammersOfSigmar);

    bool ok = evos->configure(invigoration);
    if (!ok)
    {
        delete evos;
        evos = nullptr;
    }
    return evos;
}

void StormsiresCursebreakers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Stormsire's Cursebreakers", factoryMethod);
    }
}

void StormsiresCursebreakers::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_tempestBladeAndStave);
}

std::string StormsiresCursebreakers::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Lore of Invigoration")
    {
        return ToString((LoreOfInvigoration) parameter.m_intValue);
    }
    return StormcastEternal::ValueToString(parameter);
}

int StormsiresCursebreakers::EnumStringToInt(const std::string &enumString)
{
    LoreOfInvigoration invigoration;
    if (FromString(enumString, invigoration))
    {
        return (int) invigoration;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

Rerolls StormsiresCursebreakers::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Blessed Banishment
    if (target->hasKeyword(DEATH) || target->hasKeyword(CHAOS))
    {
        return RerollOnes;
    }
    return StormcastEternal::toHitRerolls(weapon, target);
}

} // namespace StormcastEternals