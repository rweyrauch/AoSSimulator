/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/ColossalSquig.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    ColossalSquig::Create,
    nullptr,
    nullptr,
    {
    }
};

bool ColossalSquig::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_jawsToHit;
    int m_tramplingAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 7, 10, 13, ColossalSquig::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {RAND_4D6, 2, 10},
        {RAND_3D6, 3, 8},
        {RAND_2D6, 4, 6},
        {RAND_2D6, 5, 4},
        {RAND_D6,  6, 2}
    };

ColossalSquig::ColossalSquig() :
    Unit("Colossal Squig", RAND_4D6, WOUNDS, 10, 5, false),
    m_puffSpores(Weapon::Type::Missile, "Puff Spores", 8, 1, 5, 5, 0, RAND_D3),
    m_enormousJaws(Weapon::Type::Melee, "Enormous Jaws", 3, 8, 2, 3, -2, RAND_D3),
    m_tramplingFeet(Weapon::Type::Melee, "Trampling Feet", 1, 10, 5, 3, -1, 1)
{
    m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, COLOSSAL_SQUIG};
}

int ColossalSquig::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

bool ColossalSquig::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_enormousJaws);
    model.addMeleeWeapon(&m_tramplingFeet);
    model.addMeleeWeapon(&m_puffSpores);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ColossalSquig::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_enormousJaws.setToHit(g_damageTable[damageIndex].m_jawsToHit);
    m_tramplingFeet.setAttacks(g_damageTable[damageIndex].m_tramplingAttacks);
}

void ColossalSquig::onSlain()
{
    // Fungoid Squig Explosion
    // TODO: Roll D6 for each enemy unit w/in 3", on 2+ unit suffers D3 mortal wounds.

    // TODO: Setup 5 cave squigs w/in 9" of this model and outside of 3" from enemy models.
}

Wounds ColossalSquig::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Swallowed Whole
    if ((hitRoll == 6) && (weapon->name() == m_enormousJaws.name()))
    {
        Dice dice;
        Wounds wounds = {0, dice.rollD3()};
        return wounds;
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int ColossalSquig::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();
    for (auto i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}


Unit *ColossalSquig::Create(const ParameterList &parameters)
{
    auto unit = new ColossalSquig();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ColossalSquig::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Colossal Squig", factoryMethod);
    }
}

void ColossalSquig::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_enormousJaws);
    visitor(&m_tramplingFeet);
    visitor(&m_puffSpores);
}

int ColossalSquig::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = Unit::targetHitModifier(weapon, attacker);
    // Puff Spores
    if (!weapon->isMissile())
    {
        modifier -= 1;
    }
    return modifier;
}

void ColossalSquig::onCharged()
{
    Dice dice;
    // Crazed Charge
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1);
    for (auto ip : units)
    {
        if (dice.rollD6() == 6)
        {
            ip->applyDamage({0, 1});
        }
    }
}

} // namespace GloomspiteGitz
