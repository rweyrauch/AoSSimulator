/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/ShalaxiHelbane.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    ShalaxiHelbane::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAANESH
};

struct TableEntry
{
    int m_move;
    int m_soulpiercerWound;
    int m_clawDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, ShalaxiHelbane::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {14, 2, 5},
        {12, 2, 4},
        {10, 3, 3},
        {8,  3, 3},
        {6,  4, 2}
    };

bool ShalaxiHelbane::s_registered = false;

ShalaxiHelbane::ShalaxiHelbane() :
    Unit("Shalaxi Helbane", 14, WOUNDS, 10, 4, false),
    m_livingWhip(Weapon::Type::Missile, "Living Whip", 6, 1, 3, 3, -1, 1),
    m_soulpiercer(Weapon::Type::Melee, "Soulpiercer", 3, 1, 2, 2, -3, RAND_D6),
    m_impalingClaws(Weapon::Type::Melee, "Impaling Claws", 3, 2, 3, 3, -2, 5)
{
    m_keywords = {CHAOS, DAEMON, GREATER_DAEMON, SLAANESH, HEDONITE, MONSTER, HERO, WIZARD, KEEPER_OF_SECRETS, SHALAXI_HELBANE};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool ShalaxiHelbane::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_livingWhip);
    model.addMeleeWeapon(&m_soulpiercer);
    model.addMeleeWeapon(&m_impalingClaws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void ShalaxiHelbane::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_livingWhip);
    visitor(&m_soulpiercer);
    visitor(&m_impalingClaws);
}

Unit *ShalaxiHelbane::Create(const ParameterList &parameters)
{
    auto unit = new ShalaxiHelbane();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ShalaxiHelbane::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Shalaxi Helbane", factoryMethod);
    }
}

void ShalaxiHelbane::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_soulpiercer.setToWound(g_damageTable[damageIndex].m_soulpiercerWound);
    m_impalingClaws.setDamage(g_damageTable[damageIndex].m_clawDamage);
    Unit::onWounded();
}

int ShalaxiHelbane::getDamageTableIndex() const
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

int ShalaxiHelbane::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

} // Slannesh
