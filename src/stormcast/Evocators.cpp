/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/Evocators.h>
#include <iostream>
#include <spells/Empower.h>

namespace StormcastEternals
{

static FactoryMethod factoryMethod = {
    Evocators::Create,
    Evocators::ValueToString,
    Evocators::EnumStringToInt,
    {
        {ParamType::Integer, "Models", Evocators::MIN_UNIT_SIZE, Evocators::MIN_UNIT_SIZE, Evocators::MAX_UNIT_SIZE, Evocators::MIN_UNIT_SIZE},
        {ParamType::Boolean, "Prime Grandstave", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Integer, "Grandstaves", 2, 0, Evocators::MAX_UNIT_SIZE, 1},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
        {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Evocators::s_registered = false;

Evocators::Evocators() :
    StormcastEternal("Evocators", 5, WOUNDS, 8, 4, false),
    m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1),
    m_tempestBladeAndStavePrime(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 5, 3, 3, -1, 1),
    m_grandStave(Weapon::Type::Melee, "Grandstave", 2, 3, 3, 3, 0, 2),
    m_grandStavePrime(Weapon::Type::Melee, "Grandstave", 2, 4, 3, 3, 0, 2)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, CORPUSCANT, WIZARD, EVOCATORS};
    m_weapons = {&m_tempestBladeAndStave, &m_tempestBladeAndStavePrime, &m_grandStave, &m_grandStavePrime};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool Evocators::configure(int numModels, int numGrandstaves, bool primeGrandstave, LoreOfInvigoration invigoration)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    const int maxGrandstaves = numModels;
    if (numGrandstaves > maxGrandstaves)
    {
        // Invalid weapon configuration.
        return false;
    }

    // Add the Prime
    auto primeModel = new Model(BASESIZE, WOUNDS);
    if (primeGrandstave)
    {
        primeModel->addMeleeWeapon(&m_grandStavePrime);
        numGrandstaves--;
    }
    else
    {
        primeModel->addMeleeWeapon(&m_tempestBladeAndStavePrime);
    }
    addModel(primeModel);

    for (auto i = 0; i < numGrandstaves; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_grandStave);
        addModel(model);
    }

    auto currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_tempestBladeAndStave);
        addModel(model);
    }

    m_knownSpells.push_back(std::make_unique<Empower>(this));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Rerolls Evocators::toSaveRerolls(const Weapon *weapon) const
{
    // Celestial Lightning Arc
    if (weapon->isMissile())
    {
        return RerollOnes;
    }

    return StormcastEternal::toSaveRerolls(weapon);
}

int Evocators::generateMortalWounds(const Unit *unit)
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

Unit *Evocators::Create(const ParameterList &parameters)
{
    auto *evos = new Evocators();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool primeGrandstave = GetBoolParam("Prime Grandstave", parameters, false);
    int numGrandstaves = GetIntParam("Grandstaves", parameters, 0);
    auto invigoration = (LoreOfInvigoration)GetEnumParam("Lore of Invigoration", parameters, (int)LoreOfInvigoration::None);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    evos->setStormhost(stormhost);

    bool ok = evos->configure(numModels, numGrandstaves, primeGrandstave, invigoration);
    if (!ok)
    {
        delete evos;
        evos = nullptr;
    }
    return evos;
}

void Evocators::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Evocators", factoryMethod);
    }
}

std::string Evocators::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Lore of Invigoration")
    {
        return ToString((LoreOfInvigoration) parameter.m_intValue);
    }
    return StormcastEternal::ValueToString(parameter);
}

int Evocators::EnumStringToInt(const std::string &enumString)
{
    LoreOfInvigoration invigoration;
    if (FromString(enumString, invigoration))
    {
        return (int) invigoration;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

} // namespace StormcastEternals
