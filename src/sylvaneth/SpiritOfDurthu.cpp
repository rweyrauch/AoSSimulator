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
#include <UnitFactory.h>
#include <iostream>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    SpiritOfDurthu::Create,
    nullptr,
    nullptr,
    {
    }
};

bool SpiritOfDurthu::s_registered = false;

struct TableEntry
{
    int m_verdantBlastAttacks;
    int m_guardianSwordDamage;
    int m_talonsToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 2, 4, 7, 9, SpiritOfDurthu::WOUNDS };
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

    m_pVerdantBlast = new Weapon(s_verdantBlast);
    m_pGuardianSword = new Weapon(s_guardianSword);
    m_pMassiveImpalingTalons = new Weapon(s_massiveImpalingTalons);

    model.addMissileWeapon(m_pVerdantBlast);
    model.addMeleeWeapon(m_pGuardianSword);
    model.addMeleeWeapon(m_pMassiveImpalingTalons);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_verdantBlast.name() << ": " << s_verdantBlast.strength() << std::endl;
        std::cout << "\t" << s_guardianSword.name() << ": " << s_guardianSword.strength() << std::endl;
        std::cout << "\t" << s_massiveImpalingTalons.name() << ": " << s_massiveImpalingTalons.strength() << std::endl;
    }

    return true;
}

void SpiritOfDurthu::hero(PlayerId id)
{
}

void SpiritOfDurthu::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_pVerdantBlast->setAttacks(g_damageTable[damageIndex].m_verdantBlastAttacks);
    m_pGuardianSword->setDamage(g_damageTable[damageIndex].m_guardianSwordDamage);
    m_pMassiveImpalingTalons->setToWound(g_damageTable[damageIndex].m_talonsToWound);
}

int SpiritOfDurthu::getDamageTableIndex() const
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

Unit *SpiritOfDurthu::Create(const ParameterList &parameters)
{
    auto unit = new SpiritOfDurthu();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SpiritOfDurthu::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Spirit of Durthu", factoryMethod);
    }
}

} // namespace Sylvaneth
