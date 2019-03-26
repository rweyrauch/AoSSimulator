/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/DrychaHamadreth.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    DrychaHamadreth::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    SYLVANETH
};

bool DrychaHamadreth::s_registered = false;

struct TableEntry
{
    int m_flitterfuriesRange;
    int m_squirmlingsHit;
    int m_talonAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, DrychaHamadreth::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {18, 3, 6},
        {15, 4, 5},
        {12, 4, 4},
        { 9, 5, 3},
        { 6, 5, 2}
    };

DrychaHamadreth::DrychaHamadreth() :
    Unit("Drycha Hamadreth", 9, WOUNDS, 8, 3, false),
    m_colonyOfFlitterfuries(Weapon::Type::Missile, "Colony of Flitterfuries", 18, 0, 0, 0, 0, 0),
    m_swarmOfSquirmlings(Weapon::Type::Missile, "Swarm of Squirmlings", 10, 0, 3, 0, 0, 0),
    m_slashingTalons(Weapon::Type::Melee, "Slashing Talons", 2, 6, 4, 3, -1, 2),
    m_thornedSlendervines(Weapon::Type::Melee, "Thorned Slendervines", 2, RAND_2D6, 4, 4, 0, 1)
{
    m_keywords = {ORDER, SYLVANETH, MONSTER, HERO, WIZARD, DRYCHA_HAMADRETH};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool DrychaHamadreth::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_colonyOfFlitterfuries);
    model.addMissileWeapon(&m_swarmOfSquirmlings);
    model.addMeleeWeapon(&m_slashingTalons);
    model.addMeleeWeapon(&m_thornedSlendervines);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void DrychaHamadreth::onStartHero(PlayerId id)
{
}

void DrychaHamadreth::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_colonyOfFlitterfuries.setRange(g_damageTable[damageIndex].m_flitterfuriesRange);
    m_swarmOfSquirmlings.setToHit(g_damageTable[damageIndex].m_squirmlingsHit);
    m_slashingTalons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);
}

int DrychaHamadreth::getDamageTableIndex() const
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

Unit *DrychaHamadreth::Create(const ParameterList &parameters)
{
    auto unit = new DrychaHamadreth();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DrychaHamadreth::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Drycha Hamadreth", factoryMethod);
    }
}

void DrychaHamadreth::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_colonyOfFlitterfuries);
    visitor(&m_swarmOfSquirmlings);
    visitor(&m_slashingTalons);
    visitor(&m_thornedSlendervines);
}

} // namespace Sylvaneth
