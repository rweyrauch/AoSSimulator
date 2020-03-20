/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordWarpseer.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <Board.h>

namespace Skaven
{
static const int BASESIZE = 120; // x92 oval
static const int WOUNDS = 12;
static const int POINTS_PER_UNIT = 320;

struct TableEntry
{
    int m_move;
    int m_tailAttacks;
    int m_glaiveToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 4, 2},
        {10, 3, 2},
        {8, 2, 3},
        {6,  1, 3},
        {4,  0, 4}
    };


bool VerminlordWarpseer::s_registered = false;

Unit *VerminlordWarpseer::Create(const ParameterList &parameters)
{
    auto unit = new VerminlordWarpseer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VerminlordWarpseer::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Verminlord Warpseer", factoryMethod);
    }
}

VerminlordWarpseer::VerminlordWarpseer() :
    Skaventide("Verminlord Warpseer", 12, WOUNDS, 10, 4, false),
    m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
    m_glaive(Weapon::Type::Melee, "Doom Glaive", 3, 6, 3, 2, -1, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, MASTERCLAN, MONSTER, HERO, WIZARD,
                  VERMINLORD_WARPSEER};
    m_weapons = {&m_tails, &m_glaive};

    s_globalBraveryMod.connect(this, &VerminlordWarpseer::terrifying, &m_connection);

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

VerminlordWarpseer::~VerminlordWarpseer()
{
    m_connection.disconnect();
}

bool VerminlordWarpseer::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMissileWeapon(&m_tails);
    model->addMeleeWeapon(&m_glaive);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds VerminlordWarpseer::applyWoundSave(const Wounds &wounds)
{
    auto totalWounds = Skaventide::applyWoundSave(wounds);

    // Protection of the Horned Rat
    Dice::RollResult resultNormal, resultMortal;

    Dice::rollD6(wounds.normal, resultNormal);
    Dice::rollD6(wounds.mortal, resultMortal);

    Wounds negatedWounds = {resultNormal.rollsGE(5), resultNormal.rollsGE(5)};
    totalWounds -= negatedWounds;
    return totalWounds.clamp();
}

int VerminlordWarpseer::terrifying(const Unit *target)
{
    // Terrifying
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }
    return 0;
}

void VerminlordWarpseer::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    if ((player == owningPlayer()) && (remainingModels() >= 0) && m_roster)
    {
        auto roll = Dice::rollD6();
        if (roll == 6) m_roster->addCommandPoints(Dice::rollD3());
        else if (roll >= 3) m_roster->addCommandPoints(1);
    }
}

Rerolls VerminlordWarpseer::toSaveRerolls(const Weapon *weapon) const
{
    if (!m_usedOrb)
    {
        // Scry-orb
        return RerollFailed;
    }

    return Skaventide::toSaveRerolls(weapon);
}

void VerminlordWarpseer::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    if (!m_usedOrb)
    {
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 13.0f)
        {
            unit->applyDamage({0, Dice::rollD6()});
            m_usedOrb = true;
        }
    }
}

void VerminlordWarpseer::onRestore()
{
    Unit::onRestore();

    m_usedOrb = false;

    // Restore table-driven attributes
    onWounded();
}

void VerminlordWarpseer::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_move = g_damageTable[getDamageTableIndex()].m_move;
    m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
    m_glaive.setToWound(g_damageTable[damageIndex].m_glaiveToWound);
}

int VerminlordWarpseer::getDamageTableIndex() const
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

int VerminlordWarpseer::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace Skaven
