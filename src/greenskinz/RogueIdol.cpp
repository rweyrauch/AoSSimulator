/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/RogueIdol.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Greenskinz
{
static FactoryMethod factoryMethod = {
    RogueIdol::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GREENSKINZ
};

struct TableEntry
{
    int m_move;
    int m_fistToWound;
    int m_feetAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 8, 11, 13, RogueIdol::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 2, 10},
        {8,  3,  8},
        {6,  3,  6},
        {4,  4,  4},
        {2,  5,  2}
    };

bool RogueIdol::s_registered = false;

RogueIdol::RogueIdol() :
    Unit("Rogue Idol", 10, WOUNDS, 10, 4, false),
    m_boulderFists(Weapon::Type::Melee, "Boulder Fists", 3, 2, 3, 2, -2, RAND_D6),
    m_stompinFeet(Weapon::Type::Melee, "Stompin' Feet", 2, 10, 3, 3, -2, 2)
{
    m_keywords = {DESTRUCTION, GREENSKINZ, MONSTER, ROGUE_IDOL};
}

bool RogueIdol::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_boulderFists);
    model.addMeleeWeapon(&m_stompinFeet);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void RogueIdol::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_boulderFists);
    visitor(&m_stompinFeet);
}

Unit *RogueIdol::Create(const ParameterList &parameters)
{
    auto unit = new RogueIdol();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void RogueIdol::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Rogue Idol", factoryMethod);
    }
}

int RogueIdol::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

int RogueIdol::getDamageTableIndex() const
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

void RogueIdol::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_boulderFists.setToWound(g_damageTable[damageIndex].m_fistToWound);
    m_stompinFeet.setAttacks(g_damageTable[damageIndex].m_feetAttacks);

    Unit::onWounded();
}

void RogueIdol::onSlain()
{
    Dice dice;

    // Avalanche!
    auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0f);
    for (auto ip : units)
    {
        int roll = dice.rollD6();
        if (roll >= 4)
        {
            ip->applyDamage({0, dice.rollD3()});
        }
    }

    Unit::onSlain();
}

Wounds RogueIdol::applyWoundSave(const Wounds &wounds)
{
    Wounds modifiedWounds = wounds;

    // Da Big' Un
    modifiedWounds.normal = (wounds.normal + 1)/2;
    if (wounds.mortal > 0)
    {
        Dice dice;
        Dice::RollResult rolls;
        dice.rollD6(wounds.mortal, rolls);
        modifiedWounds.mortal = rolls.rollsGE(4);
    }
    return modifiedWounds;
}

Wounds RogueIdol::onEndCombat(PlayerId player)
{
    auto wounds = Unit::onEndCombat(player);

    Dice dice;

    // Rubble and Ruin
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
    for (auto ip : units)
    {
        int roll = dice.rollD6();
        if (roll >= 4)
        {
            Wounds rubbleRuins = {0,0};
            ip->applyDamage(rubbleRuins);
            wounds += rubbleRuins;
        }
    }
    return wounds;
}

Rerolls RogueIdol::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Spirit of the Waaagh!
    if (m_charged)
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, target);
}

} // namespace Greenskinz
