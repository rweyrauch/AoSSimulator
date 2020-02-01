/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "nighthaunt/BlackCoach.h"

namespace Nighthaunt
{


static FactoryMethod factoryMethod = {
    BlackCoach::Create,
    nullptr,
    nullptr,
    BlackCoach::ComputePoints,
    {
    },
    DEATH,
    { NIGHTHAUNT }
};

bool BlackCoach::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_clawAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, BlackCoach::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 9},
        {10, 8},
        {8,  7},
        {6, 6},
        {4, 5}
    };

Unit *BlackCoach::Create(const ParameterList &parameters)
{
    auto unit = new BlackCoach();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BlackCoach::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Black Coach", factoryMethod);
    }
}

BlackCoach::BlackCoach() :
    Nighthaunt("Black Coach", 14, WOUNDS, 10, 4, true),
    m_graspMissile(Weapon::Type::Missile, "Cairn Wraith's Soulreach Grasp", 10, 1, 3, 3, -3, RAND_D3),
    m_scythe(Weapon::Type::Melee, "Cairn Wraith's Reaper Scythe", 1, 3, 4, 3, -1, 2),
    m_grasp(Weapon::Type::Melee, "Cairn Wraith's Soulreach Grasp", 3, 1, 3, 3, -3, RAND_D3),
    m_claws(Weapon::Type::Melee, "Relic Bearers' Spectral Claws", 1, 9, 4, 4, -1, 1),
    m_hoovesAndTeeth(Weapon::Type::Melee, "Nightmares' Hooves and Teeth", 1, 8, 4, 4, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, BLACK_COACH};
    m_weapons = {&m_graspMissile, &m_scythe, &m_grasp, &m_claws, &m_hoovesAndTeeth};
}

bool BlackCoach::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_graspMissile);
    model->addMeleeWeapon(&m_scythe);
    model->addMeleeWeapon(&m_grasp);
    model->addMeleeWeapon(&m_claws);
    model->addMeleeWeapon(&m_hoovesAndTeeth);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

int BlackCoach::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void BlackCoach::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
}

int BlackCoach::getDamageTableIndex() const
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

Wounds BlackCoach::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Frightfull Touch
    if (hitRoll == 6)
    {
        if (weapon->name() == m_scythe.name())
        {
            return {0, 2};
        }
        else if (weapon->name() == m_claws.name())
        {
            return { 0, 1};
        }
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void BlackCoach::onBeginRound(int battleRound)
{
    Unit::onBeginRound(battleRound);

    // Evocation of Death
    Dice dice;
    Dice::RollResult result;
    dice.rollD6(3, result);
    m_powerLevel += result.rollsGE(4);

    // Insubstantial Form
    if (m_powerLevel >= 4)
    {
        m_retreatAndCharge = true;
    }

    // Unholy Vigour
    if (m_powerLevel >= 2)
    {
        m_runAndCharge = true;
    }
}

void BlackCoach::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    if ((player == owningPlayer()) && (m_powerLevel >= 1))
    {
        // Nimbus of Power
        Dice dice;
        heal(dice.rollD3());
    }

    if ((player == owningPlayer()) && (m_powerLevel >= 5))
    {
        Dice dice;
        // Witch-fire
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
        for (auto unit : units) {
            if (dice.rollD6() >= 4)
            {
                unit->applyDamage({0, dice.rollD3()});
            }
        }
    }
}

void BlackCoach::onCharged()
{
    Unit::onCharged();

    if (m_charged && (m_powerLevel >= 3))
    {
        // Spectral Scythes
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
        if (!units.empty())
        {
            Dice dice;
            if (dice.rollD6() >= 2)
            {
                units[0]->applyDamage({0, dice.rollD3()});
            }
        }
    }
}

void BlackCoach::onRestore()
{
    Unit::onRestore();

    m_powerLevel = 0;
    m_runAndCharge = false;
    m_retreatAndCharge = false;
}

Rerolls BlackCoach::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Reaped Like Corn
    if ((target->remainingModels() >= 5) && (weapon->name() == m_scythe.name()))
    {
        return RerollFailed;
    }

    // Unholy Vigour
    if ((m_powerLevel >= 2) && (!weapon->isMissile()))
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, target);
}

} // namespace Nighthaunt
