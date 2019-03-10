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
    },
    ORDER,
    SYLVANETH
};

bool SpiritOfDurthu::s_registered = false;

struct TableEntry
{
    int m_verdantBlastAttacks;
    int m_guardianSwordDamage;
    int m_talonsToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, SpiritOfDurthu::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {6, 6,       2},
        {5, RAND_D6, 2},
        {4, RAND_D6, 3},
        {3, RAND_D6, 3},
        {2, RAND_D3, 4}
    };

SpiritOfDurthu::SpiritOfDurthu() :
    Unit("Spirit of Durthu", 5, WOUNDS, 9, 3, false),
    m_verdantBlast(Weapon::Type::Missile, "Verdant Blast", 15, 6, 4, 3, -1, RAND_D3),
    m_guardianSword(Weapon::Type::Melee, "Guardian Sword", 3, 3, 3, 3, -2, 6),
    m_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1)
{
    m_keywords = {ORDER, SYLVANETH, MONSTER, HERO, SPIRIT_OF_DURTHU};
}

bool SpiritOfDurthu::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_verdantBlast);
    model.addMeleeWeapon(&m_guardianSword);
    model.addMeleeWeapon(&m_massiveImpalingTalons);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void SpiritOfDurthu::onStartHero(PlayerId id)
{
}

void SpiritOfDurthu::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_verdantBlast.setAttacks(g_damageTable[damageIndex].m_verdantBlastAttacks);
    m_guardianSword.setDamage(g_damageTable[damageIndex].m_guardianSwordDamage);
    m_massiveImpalingTalons.setToWound(g_damageTable[damageIndex].m_talonsToWound);
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

void SpiritOfDurthu::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_verdantBlast);
    visitor(&m_guardianSword);
    visitor(&m_massiveImpalingTalons);
}

} // namespace Sylvaneth
