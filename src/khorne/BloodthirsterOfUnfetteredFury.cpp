/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/BloodthirsterOfUnfetteredFury.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    BloodthirsterOfUnfetteredFury::Create,
    nullptr,
    nullptr,
    {
    }
};

struct TableEntry
{
    int m_move;
    int m_lashAttacks;
    int m_axeToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = { 3, 6, 9, 12, BloodthirsterOfUnfetteredFury::WOUNDS };
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        { 10, 4, 2 },
        { 9, 3, 3 },
        { 8, 3, 3 },
        { 7, 2, 4 },
        { 6, 1, 4 }
    };

bool BloodthirsterOfUnfetteredFury::s_registered = false;

BloodthirsterOfUnfetteredFury::BloodthirsterOfUnfetteredFury() :
    Unit("Bloodthirster Of Unfettered Fury", 10, WOUNDS, 10, 4, true),
    m_lashOfKhorne(Weapon::Type::Missile, "Lash of Khorne", 8, 4, 4, 3, -1, RAND_D3),
    m_mightyAxeOfKhorne(Weapon::Type::Melee, "Mighty Axe of Khorne", 2, 6, 3, 2, -2, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, BLOODTHIRSTER_OF_UNFETTERED_FURY};
}

bool BloodthirsterOfUnfetteredFury::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_lashOfKhorne);
    model.addMeleeWeapon(&m_mightyAxeOfKhorne);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void BloodthirsterOfUnfetteredFury::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_lashOfKhorne);
    visitor(&m_mightyAxeOfKhorne);
}

Unit *BloodthirsterOfUnfetteredFury::Create(const ParameterList &parameters)
{
    auto unit = new BloodthirsterOfUnfetteredFury();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BloodthirsterOfUnfetteredFury::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodthirster Of Unfettered Fury", factoryMethod);
    }
}

int BloodthirsterOfUnfetteredFury::getDamageTableIndex() const
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

int BloodthirsterOfUnfetteredFury::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void BloodthirsterOfUnfetteredFury::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_lashOfKhorne.setAttacks(g_damageTable[damageIndex].m_lashAttacks);
    m_mightyAxeOfKhorne.setToWound(g_damageTable[damageIndex].m_axeToWound);

    Unit::onWounded();
}

void BloodthirsterOfUnfetteredFury::hero(PlayerId player)
{
    Unit::hero(player);

    // The Land Rebels
    Dice dice;
    auto enemyUnits = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 8.0f);
    for (auto ip : enemyUnits)
    {
        int roll = dice.rollD6();
        if (roll == 6)
        {
            ip->applyDamage({0, 1});
            // TODO: also movement of affected unit is halved until your next hero phase
        }
    }
}

} // namespace Khorne