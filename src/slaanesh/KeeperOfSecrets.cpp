/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/KeeperOfSecrets.h>
#include <UnitFactory.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    KeeperOfSecrets::Create,
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
    int m_greatbladeAttacks;
    int m_clawDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, KeeperOfSecrets::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {14, 4, 5},
        {12, 3, 4},
        {10, 3, 3},
        {8,  2, 3},
        {6,  2, 2}
    };

bool KeeperOfSecrets::s_registered = false;

KeeperOfSecrets::KeeperOfSecrets() :
    Unit("Keeper of Secrets",14, WOUNDS, 10, 4, false),
    m_livingWhip(Weapon::Type::Missile, "Living Whip", 6, 1, 3, 3, -1, 1),
    m_ritualKnife(Weapon::Type::Melee, "Ritual Knife or Sinistrous Hand", 1, 1, 2, 3, -1, 1),
    m_greatblade(Weapon::Type::Melee, "Elegant Greatblade", 2, 4, 3, 3, -1, 2),
    m_impalingClaws(Weapon::Type::Melee, "Impaling Claws", 3, 2, 3, 3, -2, 5)
{
    m_keywords = {CHAOS, DAEMON, SLAANESH, MONSTER, HERO, WIZARD, KEEPER_OF_SECRETS};
}

bool KeeperOfSecrets::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_livingWhip);
    model.addMeleeWeapon(&m_ritualKnife);
    model.addMeleeWeapon(&m_greatblade);
    model.addMeleeWeapon(&m_impalingClaws);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void KeeperOfSecrets::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_livingWhip);
    visitor(&m_ritualKnife);
    visitor(&m_greatblade);
    visitor(&m_impalingClaws);
}

Unit *KeeperOfSecrets::Create(const ParameterList &parameters)
{
    auto unit = new KeeperOfSecrets();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KeeperOfSecrets::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Keeper of Secrets", factoryMethod);
    }
}

void KeeperOfSecrets::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_greatblade.setAttacks(g_damageTable[damageIndex].m_greatbladeAttacks);
    m_impalingClaws.setDamage(g_damageTable[damageIndex].m_clawDamage);
    Unit::onWounded();
}

int KeeperOfSecrets::getDamageTableIndex() const
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

int KeeperOfSecrets::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

} // Slannesh
