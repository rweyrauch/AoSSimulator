/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Bastiladon.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    Bastiladon::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    Bastiladon::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

struct TableEntry
{
    int m_save;
    int m_engineAttacs;
    int m_arkAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, Bastiladon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {1, 9, 18},
        {2,  8,  15},
        {3,  7,  12},
        {4,  6,  9},
        {4,  5,  6}
    };

bool Bastiladon::s_registered = false;

Bastiladon::Bastiladon() :
    SeraphonBase("Bastiladon", 5, WOUNDS, 6, 1, false),
    m_beam(Weapon::Type::Missile, "Solar Engine", 24, 9, 4, 3, -1, 2),
    m_javelins(Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1),
    m_ark(Weapon::Type::Melee, "Ark of Sotek", 3, 18, 4, 6, 0, 1),
    m_tail(Weapon::Type::Melee, "Bludgeoning Tail", 2, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, SERAPHON, SKINK, MONSTER, BASTILADON};
    m_weapons = {&m_beam, &m_javelins, &m_ark, &m_tail};
}

bool Bastiladon::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_beam);
    model->addMissileWeapon(&m_javelins);
    model->addMeleeWeapon(&m_ark);
    model->addMeleeWeapon(&m_tail);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *Bastiladon::Create(const ParameterList &parameters)
{
    auto unit = new Bastiladon();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bastiladon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bastiladon", factoryMethod);
    }
}

void Bastiladon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_beam.setAttacks(g_damageTable[damageIndex].m_engineAttacs);
    m_ark.setAttacks(g_damageTable[damageIndex].m_arkAttacks);
    m_save = g_damageTable[getDamageTableIndex()].m_save;
}

int Bastiladon::getDamageTableIndex() const
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

Wounds Bastiladon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Light of the Heavens
    if ((target->hasKeyword(DAEMON) && target->hasKeyword(CHAOS) && (weapon->name() == m_beam.name())))
    {
        return { weapon->damage()+1, 0};
    }

    // Tide of Snakes
    if ((hitRoll == 6) && (weapon->name() == m_ark.name()))
    {
        return {0, 1};
    }
    return SeraphonBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void Bastiladon::onRestore()
{
    Unit::onRestore();

    // Reset table attributes
    onWounded();
}

} //namespace Seraphon
