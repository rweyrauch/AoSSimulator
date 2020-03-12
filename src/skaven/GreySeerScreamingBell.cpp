/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GreySeerScreamingBell.h>
#include <UnitFactory.h>

namespace Skaven
{
struct TableEntry
{
    int m_move;
    int m_spikesToHit;
    int m_pealRange;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 5, 8, 10, GreySeerOnScreamingBell::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {6, 2, 26},
        {6, 3, 22},
        {4, 4, 18},
        {4,  4, 14},
        {3,  5, 10}
    };


bool GreySeerOnScreamingBell::s_registered = false;

Unit *GreySeerOnScreamingBell::Create(const ParameterList &parameters)
{
    auto unit = new GreySeerOnScreamingBell();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GreySeerOnScreamingBell::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Grey Seer on Screaming Bell", *factoryMethod);
    }
}

GreySeerOnScreamingBell::GreySeerOnScreamingBell() :
    Skaventide("Grey Seer on Screaming Bell", 6, WOUNDS, 6, 4, false),
    m_staff(Weapon::Type::Melee, "Warpstone Staff", 2, 3, 4, 4, -1, 1),
    m_clawsAndFangs(Weapon::Type::Melee, "Tearing Claws and Fangs", 1, 4, 4, 3, -1, 2),
    m_spikes(Weapon::Type::Melee, "Rusty Spikes", 1, RAND_D6, 2, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, WAR_MACHINE, HERO, WIZARD, SCREAMING_BELL,
        GREY_SEER};
    m_weapons = {&m_staff, &m_clawsAndFangs, &m_spikes};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool GreySeerOnScreamingBell::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);
    model->addMeleeWeapon(&m_clawsAndFangs);
    model->addMeleeWeapon(&m_spikes);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds GreySeerOnScreamingBell::applyWoundSave(const Wounds &wounds)
{
    auto totalWounds = Skaventide::applyWoundSave(wounds);

    // Protection of the Horned Rat
    Dice dice;
    Dice::RollResult resultNormal, resultMortal;

    dice.rollD6(wounds.normal, resultNormal);
    dice.rollD6(wounds.mortal, resultMortal);

    Wounds negatedWounds = {resultNormal.rollsGE(5), resultNormal.rollsGE(5)};
    totalWounds -= negatedWounds;
    return totalWounds.clamp();
}

void GreySeerOnScreamingBell::onWounded()
{
    Unit::onWounded();

    const int damageIndex = getDamageTableIndex();
    m_spikes.setToHit(g_damageTable[damageIndex].m_spikesToHit);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

void GreySeerOnScreamingBell::onRestore()
{
    Unit::onRestore();

    // Restore table-driven attributes
    onWounded();
}

int GreySeerOnScreamingBell::getDamageTableIndex() const
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

} //namespace Skaven
