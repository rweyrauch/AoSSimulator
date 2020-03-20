/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/LordSkreech.h>
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
    int m_reaperAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 4, 8},
        {10, 3, 7},
        {8, 2, 6},
        {6,  1, 5},
        {4,  0, 4}
    };


bool LordSkreechVerminking::s_registered = false;

Unit *LordSkreechVerminking::Create(const ParameterList &parameters)
{
    auto unit = new LordSkreechVerminking();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordSkreechVerminking::Init()
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

        s_registered = UnitFactory::Register("Lord Skreech Verminking", factoryMethod);
    }
}

LordSkreechVerminking::LordSkreechVerminking() :
    Skaventide("Lord Skreech Verminking", 12, WOUNDS, 10, 4, false),
    m_tails(Weapon::Type::Missile, "Prehensile Tails", 6,4, 3, 3, -1, 1),
    m_glaive(Weapon::Type::Melee, "Doom Glaive", 3, 6, 3, 3, -1, RAND_D3),
    m_plaguereaper(Weapon::Type::Melee, "Plaguereaper", 1, 8, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, MASTERCLAN, MONSTER, HERO, WIZARD,
                  LORD_SKREECH_VERMINKING};
    m_weapons = {&m_tails, &m_glaive, &m_plaguereaper};

    s_globalBraveryMod.connect(this, &LordSkreechVerminking::terrifying, &m_connection);

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

LordSkreechVerminking::~LordSkreechVerminking()
{
    m_connection.disconnect();
}

bool LordSkreechVerminking::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMissileWeapon(&m_tails);
    model->addMeleeWeapon(&m_glaive);
    model->addMeleeWeapon(&m_plaguereaper);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds LordSkreechVerminking::applyWoundSave(const Wounds &wounds)
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

int LordSkreechVerminking::terrifying(const Unit *target)
{
    // Terrifying
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }
    return 0;
}

void LordSkreechVerminking::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_move = g_damageTable[getDamageTableIndex()].m_move;
    m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
    m_plaguereaper.setAttacks(g_damageTable[damageIndex].m_reaperAttacks);
}

void LordSkreechVerminking::onRestore()
{
    Unit::onRestore();

    // Restore table-driven attributes
    onWounded();
}

int LordSkreechVerminking::getDamageTableIndex() const
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

int LordSkreechVerminking::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace Skaven
