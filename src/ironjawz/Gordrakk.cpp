/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/Gordrakk.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    GordrakkTheFistOfGork::Create,
    Ironjawz::ValueToString,
    Ironjawz::EnumStringToInt,
    {
        {ParamType::Enum, "Warclan", Ironjawz::Ironsunz, Ironjawz::Ironsunz, Ironjawz::DaChoppas, 1},
    },
    DEATH,
    IRONJAWZ
};

bool GordrakkTheFistOfGork::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_fistsAttacks;
    int m_bulkDice;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 10, 13, GordrakkTheFistOfGork::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 9, 9},
        {10, 8, 8},
        {8,  7, 7},
        {6, 6, 6},
        {4, 5, 5}
    };

GordrakkTheFistOfGork::GordrakkTheFistOfGork() :
    Ironjawz("Gordrakk the Fist of Gork", 12, WOUNDS, 8, 3, true),
    m_bellow(Weapon::Type::Missile, "Innard-bursting Bellow", 8, 6, 2, 3, -1, 1),
    m_smasha(Weapon::Type::Melee, "Smasha", 1, 5, 2, 3, -1, RAND_D3),
    m_kunnin(Weapon::Type::Melee, "Kunnin'", 1, 5, 2, 3, -1, 1),
    m_fistsAndTail(Weapon::Type::Melee, "Mighty Fists and Tail", 1, 9, 3, 3, -2, 2)
{
    m_keywords = {DESTRUCTION, ORRUK, MAW_KRUSHA, IRONJAWZ, MONSTER, HERO, MEGABOSS, GORDRAKK};
}

bool GordrakkTheFistOfGork::configure()
{
        auto model = new Model(BASESIZE, WOUNDS);

    model->addMissileWeapon(&m_bellow);
    model->addMeleeWeapon(&m_smasha);
    model->addMeleeWeapon(&m_kunnin);
    model->addMeleeWeapon(&m_fistsAndTail);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void GordrakkTheFistOfGork::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_bellow);
    visitor(m_smasha);
    visitor(m_kunnin);
    visitor(m_fistsAndTail);
}

Unit *GordrakkTheFistOfGork::Create(const ParameterList &parameters)
{
    auto unit = new GordrakkTheFistOfGork();

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
    unit->setWarclan(warclan);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GordrakkTheFistOfGork::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gordrakk the Fist of Gork", factoryMethod);
    }
}

int GordrakkTheFistOfGork::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void GordrakkTheFistOfGork::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_fistsAndTail.setAttacks(g_damageTable[damageIndex].m_fistsAttacks);
}

int GordrakkTheFistOfGork::getDamageTableIndex() const
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

void GordrakkTheFistOfGork::onCharged()
{
    Unit::onCharged();

    // Massively Destructive Bulk
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
    if (!units.empty())
    {
        auto unit = units.front();
        Dice dice;
        Dice::RollResult result;
        dice.rollD6(g_damageTable[getDamageTableIndex()].m_bulkDice, result);
        Wounds bulkWounds = {0, result.rollsGE(5)};
        unit->applyDamage(bulkWounds);
    }
}

Wounds GordrakkTheFistOfGork::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    if ((woundRoll >= 4) && (weapon->name() == m_kunnin.name()))
    {
        if (target->hasKeyword(WIZARD))
        {
            Dice dice;
            return {0, dice.rollD3()};
        }
    }
    else if ((woundRoll >= 4) && (weapon->name() == m_smasha.name()))
    {
        if (target->hasKeyword(HERO) && !target->hasKeyword(WIZARD))
        {
            Dice dice;
            return {0, dice.rollD3()};
        }
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void GordrakkTheFistOfGork::onStartCombat(PlayerId player)
{
    m_modelsSlainAtStartOfCombat = m_currentRecord.m_enemyModelsSlain;

    Ironjawz::onStartCombat(player);
}

Wounds GordrakkTheFistOfGork::onEndCombat(PlayerId player)
{
    // Strength from Victory
    if (m_currentRecord.m_enemyModelsSlain > m_modelsSlainAtStartOfCombat)
    {
        heal(1);
        m_smasha.setAttacks(m_smasha.attacks()+1);
        m_kunnin.setAttacks(m_kunnin.attacks()+1);
    }
    return Ironjawz::onEndCombat(player);
}

} //namespace Ironjawz