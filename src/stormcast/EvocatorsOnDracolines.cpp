/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/EvocatorsOnDracolines.h>
#include <iostream>
#include <spells/Empower.h>
#include <Board.h>

namespace StormcastEternals
{

bool EvocatorsOnCelestialDracolines::s_registered = false;

EvocatorsOnCelestialDracolines::EvocatorsOnCelestialDracolines() :
    StormcastEternal("Evocators on Celestial Dracolines", 12, WOUNDS, 8, 4, false),
    m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1),
    m_tempestBladeAndStavePrime(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 5, 3, 3, -1, 1),
    m_grandStave(Weapon::Type::Melee, "Grandstave", 2, 3, 3, 3, 0, 2),
    m_grandStavePrime(Weapon::Type::Melee, "Grandstave", 2, 4, 3, 3, 0, 2),
    m_monstrousClaws(Weapon::Type::Melee, "Monstrous Claws",1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOLINE, STORMCAST_ETERNAL, SACROSANCT, CORPUSCANT, WIZARD, EVOCATORS};
    m_weapons = {&m_tempestBladeAndStave, &m_tempestBladeAndStavePrime, &m_grandStave, &m_grandStavePrime, &m_monstrousClaws};

    s_globalBraveryMod.connect(this, &EvocatorsOnCelestialDracolines::supernaturalRoar, &m_connection);

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

EvocatorsOnCelestialDracolines::~EvocatorsOnCelestialDracolines()
{
    m_connection.disconnect();
}

bool EvocatorsOnCelestialDracolines::configure(int numModels, int numGrandstaves, bool primeGrandstave, LoreOfInvigoration invigoration)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    const int maxGrandstaves = numModels / MIN_UNIT_SIZE * 2;
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
    primeModel->addMeleeWeapon(&m_monstrousClaws);
    addModel(primeModel);

    for (auto i = 0; i < numGrandstaves; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_grandStave);
        model->addMeleeWeapon(&m_monstrousClaws);
        addModel(model);
    }

    auto currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_tempestBladeAndStave);
        model->addMeleeWeapon(&m_monstrousClaws);
        addModel(model);
    }

    m_knownSpells.push_back(std::make_unique<Empower>(this));
    if (invigoration != LoreOfInvigoration::None)
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfInvigoration(invigoration, this)));

    m_points = ComputePoints(numModels);

    return true;
}

Rerolls EvocatorsOnCelestialDracolines::toSaveRerolls(const Weapon *weapon) const
{
    // Celestial Lightning Arc
    if (weapon->isMissile())
    {
        return RerollOnes;
    }

    return StormcastEternal::toSaveRerolls(weapon);
}

int EvocatorsOnCelestialDracolines::generateMortalWounds(const Unit *unit)
{
    auto mortalWounds = StormcastEternal::generateMortalWounds(unit);

    // Celestial Lightning Arc
    int roll = Dice::rollD6();
    if (roll >= 4)
    {
        mortalWounds++;
    }
    roll = Dice::rollD6();
    if (roll >= 4)
    {
        mortalWounds++;
    }

    return mortalWounds;
}

Unit *EvocatorsOnCelestialDracolines::Create(const ParameterList &parameters)
{
    auto *evos = new EvocatorsOnCelestialDracolines();
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

void EvocatorsOnCelestialDracolines::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Boolean, "Prime Grandstave", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Integer, "Grandstaves", 2, 0, MAX_UNIT_SIZE/MIN_UNIT_SIZE*2, 1},
                {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
                {ParamType::Enum, "Lore of Invigoration", (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::None, (int)LoreOfInvigoration::SpeedOfLightning, 1},
            },
            ORDER,
            { STORMCAST_ETERNAL }
        };

        s_registered = UnitFactory::Register("Evocators on Celestial Dracolines", factoryMethod);
    }
}

Rerolls EvocatorsOnCelestialDracolines::chargeRerolls() const
{
    if (m_charged)
        return RerollFailed;
    return StormcastEternal::chargeRerolls();
}

std::string EvocatorsOnCelestialDracolines::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Lore of Invigoration")
    {
        return ToString((LoreOfInvigoration) parameter.intValue);
    }
    return StormcastEternal::ValueToString(parameter);
}

int EvocatorsOnCelestialDracolines::EnumStringToInt(const std::string &enumString)
{
    LoreOfInvigoration invigoration;
    if (FromString(enumString, invigoration))
    {
        return (int) invigoration;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

int EvocatorsOnCelestialDracolines::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int EvocatorsOnCelestialDracolines::supernaturalRoar(const Unit *target)
{
    // Supernatural Roar
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }

    return 0;
}

} // namespace StormcastEternals
