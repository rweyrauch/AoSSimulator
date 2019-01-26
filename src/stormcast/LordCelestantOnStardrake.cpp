/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestantOnStardrake.h>
#include <iostream>
#include <Board.h>
#include <Roster.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    LordCelestantOnStardrake::Create,
    LordCelestantOnStardrake::ValueToString,
    LordCelestantOnStardrake::EnumStringToInt,
    {
        {ParamType::Integer, "weapons", {.m_intValue = LordCelestantOnStardrake::CelestineHammer}, LordCelestantOnStardrake::CelestineHammer,
         LordCelestantOnStardrake::StormboundBlade, 1},
    }
};

struct TableEntry
{
    int m_move;
    int m_greatClawsToHit;
    int m_cavernousJawsBits;
};

const size_t NUM_TABLE_ENTRIES = 4;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 4, 8, 12, LordCelestantOnStardrake::WOUNDS };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        { 12, 3, 3 },
        { 11, 3, 2 },
        { 10, 4, 2 },
        { 8, 4, 1 }
    };

bool LordCelestantOnStardrake::s_registered = false;

Weapon LordCelestantOnStardrake::s_celestineHammer(Weapon::Type::Melee, "Celestine Hammer", 2, 3, 3, 2, -1, RAND_D3);
Weapon LordCelestantOnStardrake::s_stormboundBlade(Weapon::Type::Melee, "Stormbound Blade", 2, 3, 3, 4, -1, 2);
Weapon LordCelestantOnStardrake::s_greatClaws(Weapon::Type::Melee, "Great Claws", 1, 4, 3, 3, -1, RAND_D3);

LordCelestantOnStardrake::LordCelestantOnStardrake() :
    StormcastEternal("Lord-Celestant on Stardrake", 12, WOUNDS, 9, 3, true)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STARDRAKE, STORMCAST_ETERNAL, HERO, MONSTER, LORD_CELESTANT };
}

bool LordCelestantOnStardrake::configure(WeaponOption weapons)
{
    m_weapons = weapons;

    m_pGreatClaws = new Weapon(s_greatClaws);

    Model model(BASESIZE, WOUNDS);
    if (weapons == CelestineHammer)
        model.addMeleeWeapon(&s_celestineHammer);
    else if (weapons == StormboundBlade)
        model.addMeleeWeapon(&s_stormboundBlade);
    model.addMeleeWeapon(m_pGreatClaws);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_celestineHammer.name() << ": " << s_celestineHammer.strength() << std::endl;
        std::cout << "\t" << s_stormboundBlade.name() << ": " << s_stormboundBlade.strength() << std::endl;
        std::cout << "\t" << s_greatClaws.name() << ": " << s_greatClaws.strength() << std::endl;
    }

    return true;
}

int LordCelestantOnStardrake::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *LordCelestantOnStardrake::Create(const ParameterList &parameters)
{
    auto unit = new LordCelestantOnStardrake();
    auto weapons = (WeaponOption)GetIntParam("weapons", parameters, CelestineHammer);

    bool ok = unit->configure(weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordCelestantOnStardrake::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord-Celestant-on-Stardrake", factoryMethod);
    }
}

std::string LordCelestantOnStardrake::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == CelestineHammer)
            return "CelestineHammer";
        else if (parameter.m_intValue == StormboundBlade)
            return "StormboundBlade";
    }
    return ParameterValueToString(parameter);
}


void LordCelestantOnStardrake::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_pGreatClaws->setToHit(g_damageTable[damageIndex].m_greatClawsToHit);
}

int LordCelestantOnStardrake::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();
    for (auto i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}

int LordCelestantOnStardrake::extraAttacks(const Weapon *weapon) const
{
    int attacks = Unit::extraAttacks(weapon);

    // Inescapable Vengeance
    if (m_charged)
    {
        Dice dice;
        attacks += dice.rollD3();
    }
    return attacks;
}

Hits LordCelestantOnStardrake::applyHitModifiers(const Weapon *weapon, const Unit *unit,
                                                 const Hits &hits) const
{
    // Stormbound Blade
    if (weapon->name() == s_stormboundBlade.name())
    {
        int num6s = hits.rolls.numUnmodified6s();
        Hits modHits = hits;
        modHits.numHits += 2 * num6s; // add 2 additional hits for each 6 for a total of 3 hits per unmodified 6.
        return modHits;
    }
    return StormcastEternal::applyHitModifiers(weapon, unit, hits);
}

Rerolls LordCelestantOnStardrake::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Thundershield
    return RerollOnes;
}

Wounds LordCelestantOnStardrake::computeReturnedDamage(const Weapon *weapon,
                                          const Dice::RollResult &saveRolls) const
{
    // Sigmarite Thundershield
    // 1 mortal wound for each save of a 6
    Wounds returnedDamage = {0, saveRolls.numUnmodified6s()};
    return returnedDamage;
}

void LordCelestantOnStardrake::onStartCombat(PlayerId player)
{
    StormcastEternal::onStartCombat(player);

    // Cavernous Jaws
    if (m_meleeTarget)
    {
        Dice dice;
        auto numBites = g_damageTable[getDamageTableIndex()].m_cavernousJawsBits;
        int numToSlay = 0;
        for (auto i = 0; i < numBites; i++)
        {
            int roll = dice.rollD6();
            if (roll > m_meleeTarget->wounds())
            {
                numToSlay++;
            }
        }
        m_meleeTarget->slay(numToSlay);
    }
}

void LordCelestantOnStardrake::onEndCombat(PlayerId player)
{
    StormcastEternal::onEndCombat(player);

    // Sweeping Tail
    {
        auto board = Board::Instance();

        PlayerId otherPlayer = PlayerId::Red;
        if (player == PlayerId::Red)
            otherPlayer = PlayerId::Blue;
        auto otherRoster = board->getPlayerRoster(otherPlayer);

        Dice dice;
        // find all enemy units within 3"
        for (auto ip = otherRoster->unitBegin(); ip != otherRoster->unitEnd(); ++ip)
        {
            auto dist = distanceTo(*ip);
            if (dist <= 3.0)
            {
                auto roll = dice.rollD6();
                if (roll < (*ip)->remainingModels())
                {
                    // inflict D3 mortal wounds
                    roll = dice.rollD3();
                    Wounds mortalWounds = {0, roll};
                    (*ip)->applyDamage(mortalWounds);
                }
            }
        }
    }
}

void LordCelestantOnStardrake::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    // Lord of the Heavens
    // Decide: 'Roiling Thunder' or 'Rain of Stars'?
    bool preferRainOfStars = true;
    if (m_shootingTarget)
    {
        auto range = distanceTo(m_shootingTarget);
        if (range <= 18.0f)
        {
            // Roiling Thunder
            preferRainOfStars = false;
        }
    }

    Dice dice;
    if (preferRainOfStars)
    {
        auto board = Board::Instance();

        PlayerId otherPlayer = PlayerId::Red;
        if (player == PlayerId::Red)
            otherPlayer = PlayerId::Blue;
        auto otherRoster = board->getPlayerRoster(otherPlayer);
        auto numUnits = dice.rollD6();

        int unitsAffected = 0;
        for (auto ip = otherRoster->unitBegin(); ip != otherRoster->unitEnd(); ++ip)
        {
            int roll = dice.rollD6();
            if (roll >= 4)
            {
                Wounds wounds = {0, dice.rollD3()};
                (*ip)->applyDamage(wounds);
            }
            unitsAffected++;

            if (unitsAffected > numUnits) break;
        }
    }
    else // Roiling Thunder
    {
        auto numModels = m_shootingTarget->remainingModels();
        Dice::RollResult rolls;
        dice.rollD6(numModels, rolls);
        int mortalWounds = rolls.numUnmodified6s();
        Wounds wounds = {0, mortalWounds};
        m_shootingTarget->applyDamage(wounds);
    }
}

int LordCelestantOnStardrake::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "CelestineHammer")
        return CelestineHammer;
    else if (enumString == "StormboundBlade")
        return StormboundBlade;
    return 0;
}

} // namespace StormcastEternals