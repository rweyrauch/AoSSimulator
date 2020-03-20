/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/AstreiaSolbright.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <spells/StormcastSpells.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static const int BASESIZE = 90; // x52 oval
static const int WOUNDS = 7;
static const int POINTS_PER_UNIT = 220;

bool AstreiaSolbright::s_registered = false;

AstreiaSolbright::AstreiaSolbright() :
    StormcastEternal("Astreia Solblight", 12, WOUNDS, 9, 3, false),
    m_aetherstave(Weapon::Type::Melee, "Aetherstave", 2, 4, 3, 3, -1, RAND_D3),
    m_monstrousClaws(Weapon::Type::Melee, "Monstrous Claws", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOLINE, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, HERO, WIZARD, LORD_ARCANUM, ASTREIA_SOLBRIGHT};
    m_weapons = {&m_aetherstave, &m_monstrousClaws};

    s_globalBraveryMod.connect(this, &AstreiaSolbright::supernaturalRoar, &m_connection);

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

AstreiaSolbright::~AstreiaSolbright()
{
    m_connection.disconnect();
}

bool AstreiaSolbright::configure(LoreOfTheStorm storm, LoreOfInvigoration invigoration)
{
    if ((storm != LoreOfTheStorm::None) && (invigoration != LoreOfInvigoration::None))
    {
        return false;
    }

    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_aetherstave);
    model->addMeleeWeapon(&m_monstrousClaws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLightningPulse(this)));
    if (storm != LoreOfTheStorm::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheStorm(storm, this)));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *AstreiaSolbright::Create(const ParameterList &parameters)
{
    auto unit = new AstreiaSolbright();
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

void AstreiaSolbright::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Lore of the Storm", (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::None, (int)LoreOfTheStorm::Stormcaller, 1},
                {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Astreia Solblight", factoryMethod);
    }
}

std::string AstreiaSolbright::ValueToString(const Parameter &parameter)
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

int AstreiaSolbright::EnumStringToInt(const std::string &enumString)
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

Wounds AstreiaSolbright::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Thunderous Pounce
    if (m_charged && weapon->name() == m_monstrousClaws.name())
    {
        return {Dice::rollD3(), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls AstreiaSolbright::chargeRerolls() const
{
    // Thunderous Pounce
    return RerollFailed;
}

void AstreiaSolbright::onStartCombat(PlayerId player)
{
    // Spirit Flask
    if (!m_shatteredFlasks)
    {
        m_shatteredFlasks = DoSpiritFlasks(this);
    }
}

int AstreiaSolbright::supernaturalRoar(const Unit *target)
{
    // Supernatural Roar
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }

    return 0;
}

int AstreiaSolbright::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace StormcastEternals