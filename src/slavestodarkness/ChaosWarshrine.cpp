/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosWarshrine.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosWarshrine::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

struct TableEntry
{
    int m_move;
    int m_fistAttacks;
    int m_protection;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, ChaosWarshrine::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, 6, 18},
        {7, 5,12},
        {6, 4,9},
        {5, 3,6},
        {4, 2,3}
    };

bool ChaosWarshrine::s_registered = false;

Unit *ChaosWarshrine::Create(const ParameterList &parameters)
{
    auto unit = new ChaosWarshrine();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosWarshrine::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Warshrine", factoryMethod);
    }
}

ChaosWarshrine::ChaosWarshrine() :
    Unit("Chaos Warshrine", 8, WOUNDS, 7, 4, false),
    m_blade(Weapon::Type::Melee, "Sacrificial Blade", 1, 4, 3, 3, -1, 2),
    m_fists(Weapon::Type::Melee, "Flailing Fists", 1, 6, 4, 3, 0, 2)
{
    m_keywords = { CHAOS, MORTAL, SLAVES_TO_DARKNESS, TOTEM, PRIEST, MARK_OF_CHAOS, CHAOS_WARSHRINE };
}

bool ChaosWarshrine::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_blade);
    model.addMeleeWeapon(&m_fists);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ChaosWarshrine::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_blade);
    visitor(m_fists);
}

int ChaosWarshrine::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void ChaosWarshrine::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_fists.setAttacks(g_damageTable[damageIndex].m_fistAttacks);
}

int ChaosWarshrine::getDamageTableIndex() const
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

} //namespace SlavesToDarkness
