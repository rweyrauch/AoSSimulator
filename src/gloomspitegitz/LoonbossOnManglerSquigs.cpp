/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/LoonbossOnManglerSquigs.h>

namespace GloomspiteGitz
{

struct TableEntry
{
    int m_move;
    int m_gobsToHig;
    int m_ballsAndChainsAttack;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 0, 3, 5, 8, 10 };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
{
    { RAND_3D6, 3, 7 },
    { RAND_2D6, 4, 6 },
    { RAND_D6, 5, 5 },
    { RAND_2D6, 4, 6 },
    { RAND_3D6, 3, 7 }
};

Weapon LoonbossOnManglerSquigs::s_moonCutta("Moon-cutta", 1, 5, 3, 4, 0, 1);
Weapon LoonbossOnManglerSquigs::s_hugeFangFilledGob("Huge Fang-filled Gobs", 2, 4, 3, 3, -1, RAND_D6);
Weapon LoonbossOnManglerSquigs::s_ballsAndChains("Balls and Chains", 2, 7, 3, 3, -2, RAND_D3);
Weapon LoonbossOnManglerSquigs::s_grotsBashinStikk("Grots' Bashin' Stikks", 1, 4, 4, 4, 0, 1);

LoonbossOnManglerSquigs::LoonbossOnManglerSquigs() :
    Unit("Loonboss on Mangler Squigs", RAND_3D6, WOUNDS, 10, 4, true)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, MONSTER, HERO, LOONBOSS, MANGLER_SQUIG };
}

bool LoonbossOnManglerSquigs::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&s_moonCutta);
    model.addMeleeWeapon(&s_hugeFangFilledGob);
    model.addMeleeWeapon(&s_ballsAndChains);
    model.addMeleeWeapon(&s_grotsBashinStikk);

    addModel(model);

    return true;
}

int LoonbossOnManglerSquigs::toHitModifier(const Weapon* weapon, const Unit* unit) const
{
    int modifier = Unit::toHitModifier(weapon, unit);

    // Ker-splat!
    if (m_charged)
    {
        modifier++;
    }

    return modifier;
}

} // namespace GloomspiteGitz
