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
    int m_jawsToWound;
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
    SeraphonBase("Skink Oracle on Troglodon", 10, WOUNDS, 6, 4, false),
    m_spittle(Weapon::Type::Missile, "Noxious Spittle", 18, RAND_D3, 3, 3, 0, 2),
    m_jaws(Weapon::Type::Melee, "Venomous Jaws", 2, 3, 4, 2, 0, 2),
    m_forelimbs(Weapon::Type::Melee, "Clawed Forelimbs", 2, 2,4, 3, 0, 2),
    m_rod(Weapon::Type::Melee, "Divining Rod", 1, 2, 4, 4, -1, RAND_D3)
{
    m_keywords = {ORDER, SERAPHON, SKINK, MONSTER, HERO, WIZARD, TROGLODON, ORACLE};
    m_weapons = {&m_spittle, &m_jaws, &m_forelimbs, &m_rod};
}

bool Troglodon::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_spittle);
    model->addMeleeWeapon(&m_jaws);
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
    m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
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

void Troglodon::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    if (player == owningPlayer())
    {
        // Regeneration
        Dice dice;
        if (dice.rollD6() >= 2)
        {
            heal(dice.rollD3());
        }
    }
}

Wounds Troglodon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Venomous Spittle
    if ((woundRoll == 6) && ((weapon->name() == m_spittle.name()) || (weapon->name() == m_jaws.name())))
    {
        return {weapon->damage(), 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace Seraphon
