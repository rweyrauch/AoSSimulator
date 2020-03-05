/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/HuskardStonehorn.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    HuskardOnStonehorn::Create,
    HuskardOnStonehorn::ValueToString,
    HuskardOnStonehorn::EnumStringToInt,
    HuskardOnStonehorn::ComputePoints,
    {
    },
    DESTRUCTION,
    {OGOR_MAWTRIBES}
};

struct TableEntry
{
    int m_move;
    int m_hornsAttacks;
    int m_hoovesToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, HuskardOnStonehorn::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 6, 2},
        {10,  5, 3},
        {8,  4, 3},
        {6,  3, 4},
        {4,  2, 4}
    };

bool HuskardOnStonehorn::s_registered = false;

Unit *HuskardOnStonehorn::Create(const ParameterList &parameters)
{
    auto unit = new HuskardOnStonehorn();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string HuskardOnStonehorn::ValueToString(const Parameter &parameter)
{
    return MawtribesBase::ValueToString(parameter);
}

int HuskardOnStonehorn::EnumStringToInt(const std::string &enumString)
{
    return MawtribesBase::EnumStringToInt(enumString);
}

void HuskardOnStonehorn::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Huskard on Stonehorn", factoryMethod);
    }
}

HuskardOnStonehorn::HuskardOnStonehorn() :
    MawtribesBase("Huskard on Stonehorn", 12, WOUNDS, 9, 3, false),
    m_harpoon(Weapon::Type::Missile, "Harpoon Launcher", 20, 1, 4, 3, 0, RAND_D3),
    m_chaintrap(Weapon::Type::Missile, "Chaintrap", 12, 1, 4, 3, 0, 3),
    m_kicks(Weapon::Type::Melee, "Punches and Kicks", 1, 3, 3, 4, 0, 1),
    m_horns(Weapon::Type::Melee, "Rock-hard Horns", 2, 6, 4, 3, -2, 3),
    m_hooves(Weapon::Type::Melee, "Crushing Hooves", 1, RAND_D6, 3, 2, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, OGOR, STONEHORN, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, MONSTER, HERO, HUSKARD};
    m_weapons = {&m_harpoon, &m_chaintrap, &m_kicks, &m_horns, &m_hooves};
}

bool HuskardOnStonehorn::configure()
{
    auto model = new Model(BASESIZE, wounds());

    model->addMissileWeapon(&m_harpoon);
    model->addMissileWeapon(&m_chaintrap);
    model->addMeleeWeapon(&m_kicks);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_hooves);

    addModel(model);

    m_points = HuskardOnStonehorn::ComputePoints(1);

    return true;
}

int HuskardOnStonehorn::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

int HuskardOnStonehorn::getDamageTableIndex() const
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

void HuskardOnStonehorn::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_horns.setAttacks(g_damageTable[damageIndex].m_hornsAttacks);
    m_hooves.setToWound(g_damageTable[damageIndex].m_hoovesToWound);

    MawtribesBase::onWounded();
}

} // namespace OgorMawtribes
