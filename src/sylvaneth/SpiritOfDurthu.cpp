/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/SpiritOfDurthu.h>

namespace Sylvaneth
{

struct TableEntry
{
    int m_verdantBlastAttacks;
    int m_guardianSwordDamage;
    int m_talonsToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 0, 3, 5, 8, 10 };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
{
    { 6, 6, 2 },
    { 5, RAND_D6, 2 },
    { 4, RAND_D6, 3 },
    { 3, RAND_D6, 3 },
    { 2, RAND_D3, 4 }
};

Weapon SpiritOfDurthu::s_verdantBlast(Weapon::Type::Missile, "Verdant Blast", 15, 6, 4, 3, -1, RAND_D3);
Weapon SpiritOfDurthu::s_guardianSword(Weapon::Type::Melee, "Guardian Sword", 3, 3, 3, 3, -2, 6);
Weapon SpiritOfDurthu::s_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1);

SpiritOfDurthu::SpiritOfDurthu() :
    Unit("Spirit of Durthu", 5, WOUNDS, 9, 3, false)
{
    m_keywords = { ORDER, SYLVANETH, MONSTER, HERO, SPIRIT_OF_DURTHU };
}

bool SpiritOfDurthu::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&s_verdantBlast);
    model.addMeleeWeapon(&s_guardianSword);
    model.addMeleeWeapon(&s_massiveImpalingTalons);
    addModel(model);

    return true;
}

void SpiritOfDurthu::hero(PlayerId id)
{
}

int SpiritOfDurthu::getDamageTableIndex() const
{
    return 0;
}

} // namespace Sylvaneth
