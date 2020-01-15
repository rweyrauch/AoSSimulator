/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/DrakeswornTemplar.h>
#include <iostream>
#include <Board.h>
#include <Roster.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    DrakeswornTemplar::Create,
    DrakeswornTemplar::ValueToString,
    DrakeswornTemplar::EnumStringToInt,
    DrakeswornTemplar::ComputePoints,
    {
        {
            ParamType::Enum, "Weapon", DrakeswornTemplar::TempestAxe, DrakeswornTemplar::TempestAxe, DrakeswornTemplar::Stormlance, 1
        },
        {ParamType::Boolean, "Skybolt Bow", SIM_TRUE, SIM_FALSE, SIM_FALSE, 1},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    { STORMCAST_ETERNAL }
};

struct TableEntry
{
    int m_move;
    int m_greatClawsToHit;
    int m_cavernousJawsBits;
};

const size_t NUM_TABLE_ENTRIES = 4;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 8, 12, DrakeswornTemplar::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 3, 3},
        {11, 3, 2},
        {10, 4, 2},
        {8,  4, 1}
    };

bool DrakeswornTemplar::s_registered = false;

DrakeswornTemplar::DrakeswornTemplar() :
    StormcastEternal("Drakesworn Templar", 12, WOUNDS, 9, 3, true),
    m_skyboltBow(Weapon::Type::Missile, "Skybolt Bow", 24, 1, 3, 3, -1, 1),
    m_tempestAxe(Weapon::Type::Melee, "Tempest Axe", 2, 6, 3, 3, 0, 1),
    m_arcHammer(Weapon::Type::Melee, "Arc Hammer", 1, 2, 3, 3, -1, 3),
    m_stormlance(Weapon::Type::Melee, "Stormlance", 3, 3, 3, 3, -1, 2),
    m_greatClaws(Weapon::Type::Melee, "Great Claws", 1, 4, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STARDRAKE, STORMCAST_ETERNAL, MONSTER, HERO, DRAKESWORN_TEMPLAR};
    m_weapons = {&m_skyboltBow, &m_tempestAxe, &m_arcHammer, &m_stormlance, &m_greatClaws};
}

bool DrakeswornTemplar::configure(WeaponOption weapons, bool skyboltBow)
{
    m_weaponOption = weapons;

    auto model = new Model(BASESIZE, WOUNDS);
    if (skyboltBow)
    {
        model->addMissileWeapon(&m_skyboltBow);
    }
    if (weapons == TempestAxe)
    {
        model->addMeleeWeapon(&m_tempestAxe);
    }
    else if (weapons == ArcHammer)
    {
        model->addMeleeWeapon(&m_arcHammer);
    }
    else if (weapons == Stormlance)
    {
        model->addMeleeWeapon(&m_stormlance);
    }
    model->addMeleeWeapon(&m_greatClaws);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int DrakeswornTemplar::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *DrakeswornTemplar::Create(const ParameterList &parameters)
{
    auto unit = new DrakeswornTemplar();
    auto weapons = (WeaponOption) GetEnumParam("Weapon", parameters, TempestAxe);
    auto skyboltBow = GetBoolParam("Skybolt Bow", parameters, true);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure(weapons, skyboltBow);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DrakeswornTemplar::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Drakesworn Templar", factoryMethod);
    }
}

std::string DrakeswornTemplar::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == TempestAxe)
        {
            return "Tempest Axe";
        }
        else if (parameter.intValue == ArcHammer)
        {
            return "Arc Hammer";
        }
        else if (parameter.intValue == Stormlance)
        {
            return "Stormlance";
        }
    }
    return StormcastEternal::ValueToString(parameter);
}


void DrakeswornTemplar::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_greatClaws.setToHit(g_damageTable[damageIndex].m_greatClawsToHit);
}

int DrakeswornTemplar::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();
    for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}

void DrakeswornTemplar::onStartCombat(PlayerId player)
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

Wounds DrakeswornTemplar::onEndCombat(PlayerId player)
{
    auto wounds = StormcastEternal::onEndCombat(player);

    // Sweeping Tail
    {
        auto board = Board::Instance();

        PlayerId otherPlayer = PlayerId::Red;
        if (player == PlayerId::Red)
        {
            otherPlayer = PlayerId::Blue;
        }
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

                    wounds += mortalWounds;
                }
            }
        }
    }
    return wounds;
}

void DrakeswornTemplar::onStartShooting(PlayerId player)
{
    StormcastEternal::onStartShooting(player);

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
        {
            otherPlayer = PlayerId::Blue;
        }
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

            if (unitsAffected > numUnits)
            { break; }
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

int DrakeswornTemplar::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Tempest Axe")
    {
        return TempestAxe;
    }
    else if (enumString == "Arc Hammer")
    {
        return ArcHammer;
    }
    else if (enumString == "Stormlance")
    {
        return Stormlance;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

int DrakeswornTemplar::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_arcHammer.name()))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

Wounds DrakeswornTemplar::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    if ((hitRoll == 6) && (weapon->name() == m_stormlance.name()) && (target->hasKeyword(MONSTER)))
    {
        Dice dice;
        return {0, dice.rollD6()};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}
} // namespace StormcastEternals