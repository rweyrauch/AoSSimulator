/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Troglodon.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    Troglodon::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    Troglodon::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

struct TableEntry
{
    int m_move;
    int m_spittleRange;
    int m_biteToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, Troglodon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 18, 2},
        {9,  15,  3},
        {8,  12,  3},
        {7,  9,  4},
        {6,  6,  5}
    };

bool Troglodon::s_registered = false;

Troglodon::Troglodon() :
    SeraphonBase("Troglodon", 10, WOUNDS, 10, 4, false),
    m_spittle(Weapon::Type::Missile, "Noxious Spittle", 18, RAND_D3, 3, 3, 0, 2),
    m_bite(Weapon::Type::Melee, "Venomous Bite", 2, 6, 4, 2, 0, 2),
    m_forelimbs(Weapon::Type::Melee, "Troglodon's Clawed Forelimbs", 2, 2,4, 3, 0, 2),
    m_rod(Weapon::Type::Melee, "Skink Oracle's Divining Rod", 1, 1, 4, 5, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SKINK, MONSTER, TROGLODON};
    m_weapons = {&m_spittle, &m_bite, &m_forelimbs, &m_rod};
}

bool Troglodon::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_spittle);
    model->addMeleeWeapon(&m_bite);
    model->addMeleeWeapon(&m_forelimbs);
    model->addMeleeWeapon(&m_rod);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

int Troglodon::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *Troglodon::Create(const ParameterList &parameters)
{
    auto unit = new Troglodon();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Troglodon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Troglodon", factoryMethod);
    }
}

void Troglodon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_spittle.setRange(g_damageTable[damageIndex].m_spittleRange);
    m_bite.setToWound(g_damageTable[damageIndex].m_biteToWound);
}

int Troglodon::getDamageTableIndex() const
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

} //namespace Seraphon
