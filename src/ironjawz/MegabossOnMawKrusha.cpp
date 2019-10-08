/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/MegabossOnMawKrusha.h>
#include <UnitFactory.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    MegabossOnMawKrusha::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    IRONJAWZ
};

bool MegabossOnMawKrusha::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_fistsAttacks;
    int m_bulkDice;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 10, 13, MegabossOnMawKrusha::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 8, 8},
        {10, 7, 7},
        {8,  6, 6},
        {6, 5,5},
        {4, 4, 4}
    };

MegabossOnMawKrusha::MegabossOnMawKrusha() :
    Ironjawz("Megaboss on Maw-Krusha", 12, WOUNDS, 8, 3, true),
    m_bellow(Weapon::Type::Missile, "Innard-bursting Bellow", 8, RAND_D6, 2, 3, -1, 1),
    m_hackaAndChoppa(Weapon::Type::Melee, "Boss Gore-hacka and Choppa", 2, 8, 3, 3, -1, 2),
    m_ripToofFist(Weapon::Type::Melee, "Boss Choppa and Rip-toof Fist", 1, 6, 3, 3, -1, 2),
    m_fistsAndTail(Weapon::Type::Melee, "Mighty Fists and Tail", 1, 8, 3, 3, -2, 2)
{
    m_keywords = {DESTRUCTION, ORRUK, MAW_KRUSHA, IRONJAWZ, MONSTER, HERO, MEGABOSS, GORDRAKK};
}

bool MegabossOnMawKrusha::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_bellow);
    model.addMeleeWeapon(&m_hackaAndChoppa);
    model.addMeleeWeapon(&m_ripToofFist);
    model.addMeleeWeapon(&m_fistsAndTail);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void MegabossOnMawKrusha::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bellow);
    visitor(&m_hackaAndChoppa);
    visitor(&m_ripToofFist);
    visitor(&m_fistsAndTail);
}

Unit *MegabossOnMawKrusha::Create(const ParameterList &parameters)
{
    auto unit = new MegabossOnMawKrusha();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void MegabossOnMawKrusha::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Megaboss on Maw-Krusha", factoryMethod);
    }
}

int MegabossOnMawKrusha::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void MegabossOnMawKrusha::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_fistsAndTail.setAttacks(g_damageTable[damageIndex].m_fistsAttacks);
}

int MegabossOnMawKrusha::getDamageTableIndex() const
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

} //namespace Ironjawz