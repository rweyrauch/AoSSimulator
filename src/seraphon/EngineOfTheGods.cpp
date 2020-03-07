/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/EngineOfTheGods.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    EngineOfTheGods::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    EngineOfTheGods::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

struct TableEntry
{
    int m_move;
    int m_hornDamage;
    int m_stompAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 5, 8, 10, EngineOfTheGods::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, 4, 5},
        {7, 3,  4},
        {6, 2,  3},
        {5, 2,  3},
        {4, 1,  1}
    };

bool EngineOfTheGods::s_registered = false;

EngineOfTheGods::EngineOfTheGods() :
    SeraphonBase("Engine of the Gods", 8, WOUNDS, 6, 4, false),
    m_javelins(Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1),
    m_horns(Weapon::Type::Melee, "Massive Horns", 2, 2, 3, 3, -1, 4),
    m_jaws(Weapon::Type::Melee, "Grinding Jaws", 1, 2, 3, 3, -1, 2),
    m_stomps(Weapon::Type::Melee, "Crushing Stomps", 1, 5, 3, 3, -1, 2)
{
    m_keywords = {ORDER, SERAPHON, STEGADON, SKINK, MONSTER, HERO, ENGINE_OF_THE_GODS};
    m_weapons = {&m_javelins, &m_horns, &m_jaws, &m_stomps};
}

bool EngineOfTheGods::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_javelins);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_jaws);
    model->addMeleeWeapon(&m_stomps);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

int EngineOfTheGods::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *EngineOfTheGods::Create(const ParameterList &parameters)
{
    auto unit = new EngineOfTheGods();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void EngineOfTheGods::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Engine of the Gods", factoryMethod);
    }
}

void EngineOfTheGods::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_stomps.setAttacks(g_damageTable[damageIndex].m_stompAttacks);
}

int EngineOfTheGods::getDamageTableIndex() const
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
