/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/ManglerSquigs.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    ManglerSquigs::Create,
    nullptr,
    nullptr,
    {
    }
};

bool ManglerSquigs::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_gobsToHit;
    int m_ballsAndChainsAttack;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, ManglerSquigs::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {RAND_3D6, 3, 7},
        {RAND_2D6, 4, 6},
        {RAND_D6,  5, 5},
        {RAND_2D6, 4, 6},
        {RAND_3D6, 3, 7}
    };

ManglerSquigs::ManglerSquigs() :
    Unit("Mangler Squigs", RAND_3D6, WOUNDS, 10, 4, true),
    m_hugeFangFilledGob(Weapon::Type::Melee, "Huge Fang-filled Gobs", 2, 4, 3, 3, -1, RAND_D6),
    m_ballsAndChains(Weapon::Type::Melee, "Balls and Chains", 2, 7, 3, 3, -2, RAND_D3),
    m_grotsBashinStikk(Weapon::Type::Melee, "Grots' Bashin' Stikks", 1, 4, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, MANGLER_SQUIG};
}

int ManglerSquigs::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

bool ManglerSquigs::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_hugeFangFilledGob);
    model.addMeleeWeapon(&m_ballsAndChains);
    model.addMeleeWeapon(&m_grotsBashinStikk);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int ManglerSquigs::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    int modifier = Unit::toHitModifier(weapon, unit);

    // Ker-splat!
    if (m_charged)
    {
        modifier++;
    }

    return modifier;
}

void ManglerSquigs::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_hugeFangFilledGob.setToHit(g_damageTable[damageIndex].m_gobsToHit);
    m_ballsAndChains.setAttacks(g_damageTable[damageIndex].m_ballsAndChainsAttack);
}

int ManglerSquigs::getDamageTableIndex() const
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


Unit *ManglerSquigs::Create(const ParameterList &parameters)
{
    auto unit = new ManglerSquigs();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ManglerSquigs::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Mangler Squigs", factoryMethod);
    }
}

void ManglerSquigs::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_hugeFangFilledGob);
    visitor(&m_ballsAndChains);
    visitor(&m_grotsBashinStikk);
}

void ManglerSquigs::onSlain()
{
    Dice dice;

    // Watch Out!
    // get all units within 6" (friend and foe)
    auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 6.0f);
    for (auto ip : units)
    {
        int roll = dice.rollD6();
        if (roll >= 4)
        {
            int mortalWounds = dice.rollD3();
            ip->applyDamage({0, mortalWounds});
        }
    }

    Unit::onSlain();
}

} // namespace GloomspiteGitz
