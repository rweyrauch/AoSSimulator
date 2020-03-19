/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <beastsofchaos/ChaosGargant.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace BeastsOfChaos
{
struct TableEntry
{
    int m_move;
    int m_clubAttacks;
    int m_eadbuttDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, ChaosGargant::WOUNDS};
const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, RAND_3D6, 6},
        {6, RAND_2D6, RAND_D6},
        {5, RAND_2D6, RAND_D3},
        {4, RAND_D6, RAND_D3},
        {3, RAND_D6, 1}
    };

bool ChaosGargant::s_registered = false;

ChaosGargant::ChaosGargant() :
    BeastsOfChaosBase("Chaos Gargant", 8, WOUNDS, 6, 5, false),
    m_eadbutt(Weapon::Type::Melee, "Vicious 'eadbutt", 1, 1, 4, 3, -3, 6),
    m_massiveClub(Weapon::Type::Melee, "Massive Club", 3, RAND_3D6, 3, 3, -1, 1),
    m_mightyKick(Weapon::Type::Melee, "Mighty Kick", 2, 1, 3, 3, -2, RAND_D3)
{
    m_keywords = {CHAOS, GARGANT, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, CHAOS_GARGANT};
    m_weapons = {&m_eadbutt, &m_massiveClub, &m_mightyKick};
}

bool ChaosGargant::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_eadbutt);
    model->addMeleeWeapon(&m_massiveClub);
    model->addMeleeWeapon(&m_mightyKick);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ChaosGargant::onWounded()
{
    m_move = g_damageTable[getDamageTableIndex()].m_move;
    m_eadbutt.setDamage(g_damageTable[getDamageTableIndex()].m_eadbuttDamage);
    m_massiveClub.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
}

Unit *ChaosGargant::Create(const ParameterList &parameters)
{
    auto unit = new ChaosGargant();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosGargant::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            ChaosGargant::Create,
            nullptr,
            nullptr,
            ChaosGargant::ComputePoints,
            {
            },
            DESTRUCTION,
            { GLOOMSPITE_GITZ }
        };
        s_registered = UnitFactory::Register("Chaos Gargant", factoryMethod);
    }
}

int ChaosGargant::getDamageTableIndex() const
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

void ChaosGargant::onRestore()
{
    Unit::onRestore();

    // Reset table-driven attributes
    onWounded();
}

} // namespace GloomspiteGitz