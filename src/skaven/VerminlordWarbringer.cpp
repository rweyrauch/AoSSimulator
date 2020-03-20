/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordWarbringer.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Skaven
{
static const int BASESIZE = 120; // x92 oval
static const int WOUNDS = 12;
static const int POINTS_PER_UNIT = 280;

struct TableEntry
{
    int m_move;
    int m_tailAttacks;
    int m_fistToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 4, 2},
        {10, 3, 3},
        {8, 2, 3},
        {6,  1, 4},
        {4,  0, 4}
    };


bool VerminlordWarbringer::s_registered = false;

Unit *VerminlordWarbringer::Create(const ParameterList &parameters)
{
    auto unit = new VerminlordWarbringer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VerminlordWarbringer::Init()
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

        s_registered = UnitFactory::Register("Verminlord Warbringer", factoryMethod);
    }
}

VerminlordWarbringer::VerminlordWarbringer() :
    Skaventide("Verminlord Warbringer", 12, WOUNDS, 10, 4, false),
    m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
    m_glaive(Weapon::Type::Melee, "Doom Glaive", 3, 6, 3, 3, -1, RAND_D3),
    m_fist(Weapon::Type::Melee, "Spike-fist", 1, 1, 3, 2, -2, 2)
{
    m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, CLANS_VERMINUS, MONSTER, HERO, WIZARD,
        VERMINLORD_WARBRINGER};
    m_weapons = {&m_tails, &m_glaive, &m_fist};

    s_globalBraveryMod.connect(this, &VerminlordWarbringer::terrifying, &m_connection);

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

VerminlordWarbringer::~VerminlordWarbringer()
{
    m_connection.disconnect();
}

bool VerminlordWarbringer::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMissileWeapon(&m_tails);
    model->addMeleeWeapon(&m_glaive);
    model->addMeleeWeapon(&m_fist);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds VerminlordWarbringer::applyWoundSave(const Wounds &wounds)
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

int VerminlordWarbringer::terrifying(const Unit *target)
{
    // Terrifying
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }
    return 0;
}

Wounds VerminlordWarbringer::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Fist of Verminus Supremacy
    if ((woundRoll == 6) && (weapon->name() == m_fist.name()))
    {
        return {weapon->damage()+4, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls VerminlordWarbringer::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Amidst the Seething Tide
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 13.0f);
    int totalModels = 0;
    for (auto unit : units)
    {
        if (unit->hasKeyword(SKAVEN))
        {
            totalModels += unit->remainingModels();
        }
    }
    if (totalModels >= 13)
        return RerollFailed;

    return Unit::toWoundRerolls(weapon, target);
}

void VerminlordWarbringer::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_move = g_damageTable[getDamageTableIndex()].m_move;
    m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
    m_fist.setToWound(g_damageTable[damageIndex].m_fistToWound);
}

void VerminlordWarbringer::onRestore()
{
    Unit::onRestore();

    // Restore table-driven attributes
    onWounded();
}

int VerminlordWarbringer::getDamageTableIndex() const
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

int VerminlordWarbringer::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace Skaven
