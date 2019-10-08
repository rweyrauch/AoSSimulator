/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/Gordrakk.h>
#include <UnitFactory.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    GordrakkTheFistOfGork::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    IRONJAWZ
};

bool GordrakkTheFistOfGork::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_fistsAttacks;
    int m_bulkDice;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 10, 13, GordrakkTheFistOfGork::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 9, 9},
        {10, 8, 8},
        {8,  7, 7},
        {6, 6, 6},
        {4, 5, 5}
    };

GordrakkTheFistOfGork::GordrakkTheFistOfGork() :
    Ironjawz("Gordrakk the Fist of Gork", 12, WOUNDS, 8, 3, true),
    m_bellow(Weapon::Type::Missile, "Innard-bursting Bellow", 8, 6, 2, 3, -1, 1),
    m_smasha(Weapon::Type::Melee, "Smasha", 1, 5, 2, 3, -1, RAND_D3),
    m_kunnin(Weapon::Type::Melee, "Kunnin'", 1, 5, 2, 3, -1, 1),
    m_fistsAndTail(Weapon::Type::Melee, "Mighty Fists and Tail", 1, 9, 3, 3, -2, 2)
{
    m_keywords = {DESTRUCTION, ORRUK, MAW_KRUSHA, IRONJAWZ, MONSTER, HERO, MEGABOSS, GORDRAKK};
}

bool GordrakkTheFistOfGork::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_bellow);
    model.addMeleeWeapon(&m_smasha);
    model.addMeleeWeapon(&m_kunnin);
    model.addMeleeWeapon(&m_fistsAndTail);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void GordrakkTheFistOfGork::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bellow);
    visitor(&m_smasha);
    visitor(&m_kunnin);
    visitor(&m_fistsAndTail);
}

Unit *GordrakkTheFistOfGork::Create(const ParameterList &parameters)
{
    auto unit = new GordrakkTheFistOfGork();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GordrakkTheFistOfGork::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gordrakk the Fist of Gork", factoryMethod);
    }
}

int GordrakkTheFistOfGork::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void GordrakkTheFistOfGork::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_fistsAndTail.setAttacks(g_damageTable[damageIndex].m_fistsAttacks);
}

int GordrakkTheFistOfGork::getDamageTableIndex() const
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