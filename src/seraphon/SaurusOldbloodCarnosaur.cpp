/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusOldbloodCarnosaur.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SaurusOldbloodOnCarnosaur::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    SaurusOldbloodOnCarnosaur::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

struct TableEntry
{
    int m_move;
    int m_forelimbToHit;
    int m_jawsAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, SaurusOldbloodOnCarnosaur::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 3, 5},
        {10,  4,  4},
        {8,  4,  3},
        {8,  5,  2},
        {6,  5,  1}
    };

bool SaurusOldbloodOnCarnosaur::s_registered = false;

SaurusOldbloodOnCarnosaur::SaurusOldbloodOnCarnosaur() :
    SeraphonBase("Saurus Oldblood on Carnosaur", 10, WOUNDS, 10, 4, false),
    m_gauntlet(Weapon::Type::Missile, "Sunbolt Gauntlet", 18, RAND_D6, 3, 4, -1, 1),
    m_spear(Weapon::Type::Melee, "Sunstone Spear", 2, 3, 3, 3, -1, RAND_D3),
    m_forelimbs(Weapon::Type::Melee, "Carnosaur's Clawed Forelimbs", 2, 2,3, 3, 0, 2),
    m_jaws(Weapon::Type::Melee, "Carnosaur's Massive Jaws", 2, 5, 4, 3, -1, 3)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, CARNOSAUR, SAURUS, MONSTER, HERO, SAURUS_OLDBLOOD};
    m_weapons = {&m_gauntlet, &m_spear, &m_forelimbs, &m_jaws};
}

bool SaurusOldbloodOnCarnosaur::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_gauntlet);
    model->addMeleeWeapon(&m_spear);
    model->addMeleeWeapon(&m_forelimbs);
    model->addMeleeWeapon(&m_jaws);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

int SaurusOldbloodOnCarnosaur::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *SaurusOldbloodOnCarnosaur::Create(const ParameterList &parameters)
{
    auto unit = new SaurusOldbloodOnCarnosaur();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SaurusOldbloodOnCarnosaur::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Saurus Oldblood on Carnosaur", factoryMethod);
    }
}

void SaurusOldbloodOnCarnosaur::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_jaws.setAttacks(g_damageTable[damageIndex].m_jawsAttacks);
    m_forelimbs.setToHit(g_damageTable[damageIndex].m_forelimbToHit);
}

int SaurusOldbloodOnCarnosaur::getDamageTableIndex() const
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
