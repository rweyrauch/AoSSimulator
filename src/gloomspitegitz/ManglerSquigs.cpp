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
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 2, 4, 7, 9, ManglerSquigs::WOUNDS };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
{
    { RAND_3D6, 3, 7 },
    { RAND_2D6, 4, 6 },
    { RAND_D6, 5, 5 },
    { RAND_2D6, 4, 6 },
    { RAND_3D6, 3, 7 }
};

Weapon ManglerSquigs::s_hugeFangFilledGob(Weapon::Type::Melee, "Huge Fang-filled Gobs", 2, 4, 3, 3, -1, RAND_D6);
Weapon ManglerSquigs::s_ballsAndChains(Weapon::Type::Melee, "Balls and Chains", 2, 7, 3, 3, -2, RAND_D3);
Weapon ManglerSquigs::s_grotsBashinStikk(Weapon::Type::Melee, "Grots' Bashin' Stikks", 1, 4, 4, 4, 0, 1);

ManglerSquigs::ManglerSquigs() :
    Unit("Mangler Squigs", RAND_3D6, WOUNDS, 10, 4, true)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, MANGLER_SQUIG };
}

int ManglerSquigs::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

bool ManglerSquigs::configure()
{
    Model model(BASESIZE, WOUNDS);

    m_pHugeFangFilledGob = new Weapon(s_hugeFangFilledGob);
    m_pBallsAndChains = new Weapon(s_ballsAndChains);

    model.addMeleeWeapon(m_pHugeFangFilledGob);
    model.addMeleeWeapon(m_pBallsAndChains);
    model.addMeleeWeapon(&s_grotsBashinStikk);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_hugeFangFilledGob.name() << ": " << s_hugeFangFilledGob.strength() << std::endl;
        std::cout << "\t" << s_ballsAndChains.name() << ": " << s_ballsAndChains.strength() << std::endl;
        std::cout << "\t" << s_grotsBashinStikk.name() << ": " << s_grotsBashinStikk.strength() << std::endl;
    }

    return true;
}

int ManglerSquigs::toHitModifier(const Weapon* weapon, const Unit* unit) const
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
    m_pHugeFangFilledGob->setToHit(g_damageTable[damageIndex].m_gobsToHit);
    m_pBallsAndChains->setAttacks(g_damageTable[damageIndex].m_ballsAndChainsAttack);
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

} // namespace GloomspiteGitz
