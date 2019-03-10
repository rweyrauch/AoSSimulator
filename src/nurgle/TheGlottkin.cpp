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
#include <spells/ArcaneBolt.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    TheGlottkin::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    NURGLE
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
    Unit("The Glottkin", 8, WOUNDS, 9, 4, false),
    m_pestilentTorrent(Weapon::Type::Missile, "Pestilent Torrent", 12, 1, 3, 4, -2, RAND_2D6),
    m_flailingTentacle(Weapon::Type::Melee, "Ghurk's Flailing Tentacle", 3, 6, 4, 2, -2, 2),
    m_lampreyMaw(Weapon::Type::Melee, "Ghurk's Lamprey Maw", 2, 1, 3, 2, -1, RAND_D3),
    m_poisonScythe(Weapon::Type::Melee, "Otto's Poison-slick Scythe", 2, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, WIZARD, THE_GLOTTKIN};
}

bool TheGlottkin::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMissileWeapon(&m_pestilentTorrent);
    model.addMeleeWeapon(&m_flailingTentacle);
    model.addMeleeWeapon(&m_lampreyMaw);
    model.addMeleeWeapon(&m_poisonScythe);
    addModel(model);

    m_knownSpells.push_back(std::make_unique<ArcaneBolt>(this));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void TheGlottkin::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_pestilentTorrent);
    visitor(&m_flailingTentacle);
    visitor(&m_lampreyMaw);
    visitor(&m_poisonScythe);
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

int TheGlottkin::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void TheGlottkin::onStartHero(PlayerId player)
{
    if (player == m_owningPlayer)
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            Dice dice;
            // Blessing of Nurgle
            int woundsHealed = dice.rollD3();
            for (auto &m : m_models)
            {
                m.woundsRemaining() += woundsHealed;
                if (m.woundsRemaining() > WOUNDS)
                {
                    m.woundsRemaining() = WOUNDS;
                }
            }
        }
    }
}

void TheGlottkin::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_pestilentTorrent.setDamage(g_damageTable[damageIndex].m_torrentDamage);
    m_flailingTentacle.setAttacks(g_damageTable[damageIndex].m_tentacleAttacks);
}

int TheGlottkin::getDamageTableIndex() const
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

void TheGlottkin::onCharged()
{
    // Mountain of Loathsome Flesh
    if (m_charged)
    {
        Dice dice;
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
        for (auto ip : units)
        {
            int roll = dice.rollD6();
            if (roll >= 4)
            {
                ip->applyDamage({dice.rollD3(), 0});
            }
        }
    }
    Unit::onCharged();
}

} // namespace Nurgle

