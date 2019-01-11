/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Alarielle.h>

namespace Sylvaneth
{

struct TableEntry
{
    int m_move;
    int m_spearKurnothRange;
    int m_greatAntlerDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 0, 5, 8, 11, 14 };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
{
    { 16, 30, 5 },
    { 14, 25, 4 },
    { 10, 20, 3 },
    { 8, 15, 2 },
    { 6, 10, 1 }
};

Weapon Alarielle::s_spearOfKurnoth(Weapon::Type::Missile, "Spear of Kurnoth", 30, 1, 3, 2, -2, RAND_D6);
Weapon Alarielle::s_talonOfDwindling(Weapon::Type::Melee, "Talon of Dwindling", 1, 4, 3, 4, 0, 1);
Weapon Alarielle::s_beetleGreatAntlers(Weapon::Type::Melee, "Wardroth Beetle's Great Antlers", 2, 5, 4, 3, -2, 5);

Alarielle::Alarielle() :
    Unit("Alarielle", 16, WOUNDS, 10, 3, true)
{
    m_keywords = { ORDER, SYLVANETH, MONSTER, HERO, WIZARD, ALARIELLE_THE_EVERQUEEN };
}

bool Alarielle::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&s_spearOfKurnoth);
    model.addMeleeWeapon(&s_talonOfDwindling);
    model.addMeleeWeapon(&s_beetleGreatAntlers);
    addModel(model);

    return true;
}

int Alarielle::toHitModifier(const Weapon* weapon, const Unit* unit) const
{
    // Sweeping Blows
    // TODO: need to limit this buff to the Antler's only.
    if (unit->remainingModels() >= 5)
    {
        return 1;
    }
    return Unit::toHitModifier(weapon, unit);
}

void Alarielle::hero(PlayerId player)
{
    if (remainingWounds() < WOUNDS && remainingWounds() > 0)
    {
        Dice dice;
        // Lifebloom - heal herself D3
        int woundsHealed = dice.rollD3();
        // Soul Amphorae (healing part).  TODO: only do this if not summoning
        woundsHealed += dice.rollD3();
        for (auto& m : m_models)
        {
            m.woundsRemaining() += woundsHealed;
            if (m.woundsRemaining() > WOUNDS)
                m.woundsRemaining() = WOUNDS;
        }
    }
}

int Alarielle::getDamageTableIndex() const
{
    return 0;
}

} // namespace Sylvaneth
