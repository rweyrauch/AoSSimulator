/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordDeceiver.h>
#include <UnitFactory.h>

namespace Skaven
{
static const int BASESIZE = 120; // x92 oval
static const int WOUNDS = 12;
static const int POINTS_PER_UNIT = 320;

struct TableEntry
{
    int m_move;
    int m_tailAttacks;
    int m_stilettoToWound;
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


bool VerminlordDeceiver::s_registered = false;

Unit *VerminlordDeceiver::Create(const ParameterList &parameters)
{
    auto unit = new VerminlordDeceiver();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VerminlordDeceiver::Init()
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

        s_registered = UnitFactory::Register("Verminlord Deceiver", factoryMethod);
    }
}

VerminlordDeceiver::VerminlordDeceiver() :
    Skaventide("Verminlord Deceiver", 12, WOUNDS, 10, 4, false),
    m_doomstar(Weapon::Type::Missile, "Doomstar", 13, 1, 3, 3, -1, RAND_D3),
    m_tails(Weapon::Type::Missile, "Prehensile Tail", 6, 4, 3, 3, -1, 1),
    m_warpstiletto(Weapon::Type::Melee, "Warpstiletto", 1, 6, 3, 2, -3, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, CLANS_ESHIN, MONSTER, HERO, WIZARD,
        VERMINLORD_DECEIVER};
    m_weapons = {&m_doomstar, &m_tails, &m_warpstiletto};

    s_globalBraveryMod.connect(this, &VerminlordDeceiver::terrifying, &m_connection);

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

VerminlordDeceiver::~VerminlordDeceiver()
{
    m_connection.disconnect();
}

bool VerminlordDeceiver::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMissileWeapon(&m_doomstar);
    model->addMissileWeapon(&m_tails);
    model->addMeleeWeapon(&m_warpstiletto);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds VerminlordDeceiver::applyWoundSave(const Wounds &wounds)
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

int VerminlordDeceiver::terrifying(const Unit *target)
{
    // Terrifying
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }
    return 0;
}

Wounds VerminlordDeceiver::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Doomstar
    if ((weapon->name() == m_doomstar.name()) && (target->remainingModels() >= 10))
    {
        return {Dice::rollD6(), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int VerminlordDeceiver::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    auto mod = Unit::targetHitModifier(weapon, attacker);

    // Shrouded in Darkness
    if (weapon->isMissile()) mod -= 2;

    return mod;
}

void VerminlordDeceiver::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_move = g_damageTable[getDamageTableIndex()].m_move;
    m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
    m_warpstiletto.setToWound(g_damageTable[damageIndex].m_stilettoToWound);
}

void VerminlordDeceiver::onRestore()
{
    Unit::onRestore();

    // Restore table-driven attributes
    onWounded();
}

int VerminlordDeceiver::getDamageTableIndex() const
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

int VerminlordDeceiver::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace Skaven
