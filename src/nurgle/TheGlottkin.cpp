/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/TheGlottkin.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    TheGlottkin::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    TheGlottkin::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

struct TableEntry
{
    int m_move;
    int m_torrentDamage;
    int m_tentacleAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, TheGlottkin::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, RAND_2D6, 6},
        {7, RAND_D6, 5},
        {6, RAND_D3, 4},
        {5, 2, 3},
        {4, 1, 2}
    };

bool TheGlottkin::s_registered = false;

TheGlottkin::TheGlottkin() :
    NurgleBase("The Glottkin", 8, WOUNDS, 9, 4, false),
    m_pestilentTorrent(Weapon::Type::Missile, "Pestilent Torrent", 12, 1, 3, 4, -2, RAND_2D6),
    m_flailingTentacle(Weapon::Type::Melee, "Ghurk's Flailing Tentacle", 3, 6, 4, 2, -2, 2),
    m_lampreyMaw(Weapon::Type::Melee, "Ghurk's Lamprey Maw", 2, 1, 3, 2, -1, RAND_D3),
    m_poisonScythe(Weapon::Type::Melee, "Otto's Poison-slick Scythe", 2, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, WIZARD, THE_GLOTTKIN};
    m_weapons = {&m_pestilentTorrent, &m_flailingTentacle, &m_lampreyMaw, &m_poisonScythe};

    m_totalUnbinds = 1;
    m_totalSpells = 2;
}

bool TheGlottkin::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_pestilentTorrent);
    model->addMeleeWeapon(&m_flailingTentacle);
    model->addMeleeWeapon(&m_lampreyMaw);
    model->addMeleeWeapon(&m_poisonScythe);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *TheGlottkin::Create(const ParameterList &parameters)
{
    auto unit = new TheGlottkin();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TheGlottkin::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("The Glottkin", factoryMethod);
    }
}

void TheGlottkin::onRestore()
{
    // Reset table-drive attributes
    onWounded();
}

void TheGlottkin::onStartHero(PlayerId player)
{
    if (player == owningPlayer())
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            // Blessing of Nurgle
            int woundsHealed = Dice::rollD3();
            for (auto &m : m_models)
            {
                m->applyHealing(woundsHealed);
            }
        }
    }
}

void TheGlottkin::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_pestilentTorrent.setDamage(g_damageTable[damageIndex].m_torrentDamage);
    m_flailingTentacle.setAttacks(g_damageTable[damageIndex].m_tentacleAttacks);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

int TheGlottkin::getDamageTableIndex() const
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

void TheGlottkin::onCharged()
{
    // Mountain of Loathsome Flesh
    if (m_charged)
    {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
        for (auto ip : units)
        {
            int roll = Dice::rollD6();
            if (roll >= 4)
            {
                ip->applyDamage({Dice::rollD3(), 0});
            }
        }
    }
    Unit::onCharged();
}

} // namespace Nurgle

