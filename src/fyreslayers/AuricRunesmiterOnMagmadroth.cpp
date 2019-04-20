/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesmiterOnMagmadroth.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    AuricRunesmiterOnMagmadroth::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    FYRESLAYERS
};

struct TableEntry
{
    int m_move;
    int m_roaringFyrestream;
    int m_clawsHornsAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, AuricRunesmiterOnMagmadroth::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, RAND_D6,  6},
        {10, RAND_D6,  5},
        {8,  RAND_2D6, 4},
        {7,  RAND_2D6, 3},
        {6,  RAND_3D6, 2}
    };

bool AuricRunesmiterOnMagmadroth::s_registered = false;

AuricRunesmiterOnMagmadroth::AuricRunesmiterOnMagmadroth() :
    Unit("Auric Runesmiter on Magmadroth", 12, WOUNDS, 8, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_fyrestream(Weapon::Type::Missile, "", 12, 1, 0, 0, 0, 0),
    m_clawsAndHorns(Weapon::Type::Melee, "Claws and Horns", 1, 6, 4, 3, -1, 2),
    m_blazingMaw(Weapon::Type::Melee, "Blazing Maw", 1, 1, 4, 2, -2, RAND_D3),
    m_latchAxe(Weapon::Type::Melee, "Latch-axe", 1, 1, 3, 3, 0, 2),
    m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, MAGMADROTH, FYRESLAYERS, MONSTER, HERO, PRIEST, AURIC_RUNESMITER};
}

bool AuricRunesmiterOnMagmadroth::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_throwingAxe);
    model.addMissileWeapon(&m_fyrestream);
    model.addMeleeWeapon(&m_clawsAndHorns);
    model.addMeleeWeapon(&m_blazingMaw);
    model.addMeleeWeapon(&m_latchAxe);
    model.addMeleeWeapon(&m_runicIron);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int AuricRunesmiterOnMagmadroth::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void AuricRunesmiterOnMagmadroth::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_throwingAxe);
    visitor(&m_fyrestream);
    visitor(&m_clawsAndHorns);
    visitor(&m_blazingMaw);
    visitor(&m_latchAxe);
    visitor(&m_runicIron);
}

Unit *AuricRunesmiterOnMagmadroth::Create(const ParameterList &parameters)
{
    auto unit = new AuricRunesmiterOnMagmadroth();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AuricRunesmiterOnMagmadroth::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Auric Runesmiter on Magmadroth", factoryMethod);
    }
}

void AuricRunesmiterOnMagmadroth::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_clawsAndHorns.setAttacks(g_damageTable[damageIndex].m_clawsHornsAttacks);
}

int AuricRunesmiterOnMagmadroth::getDamageTableIndex() const
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

} // namespace Fyreslayers