/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/MegabossOnMawKrusha.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Ironjawz
{
static FactoryMethod factoryMethod = {
    MegabossOnMawKrusha::Create,
    MegabossOnMawKrusha::ValueToString,
    MegabossOnMawKrusha::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapons", MegabossOnMawKrusha::HackaAndChoppa, MegabossOnMawKrusha::HackaAndChoppa,
            MegabossOnMawKrusha::ChoppaAndRiptoofFist, 1
        },
        {ParamType::Enum, "Warclan", Ironjawz::Ironsunz, Ironjawz::Ironsunz, Ironjawz::DaChoppas, 1},
    },
    DEATH,
    IRONJAWZ
};

bool MegabossOnMawKrusha::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_fistsAttacks;
    int m_bulkDice;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 10, 13, MegabossOnMawKrusha::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 8, 8},
        {10, 7, 7},
        {8,  6, 6},
        {6, 5,5},
        {4, 4, 4}
    };

MegabossOnMawKrusha::MegabossOnMawKrusha() :
    Ironjawz("Megaboss on Maw-Krusha", 12, WOUNDS, 8, 3, true),
    m_bellow(Weapon::Type::Missile, "Innard-bursting Bellow", 8, RAND_D6, 2, 3, -1, 1),
    m_hackaAndChoppa(Weapon::Type::Melee, "Boss Gore-hacka and Choppa", 2, 8, 3, 3, -1, 2),
    m_ripToofFist(Weapon::Type::Melee, "Boss Choppa and Rip-toof Fist", 1, 6, 3, 3, -1, 2),
    m_fistsAndTail(Weapon::Type::Melee, "Mighty Fists and Tail", 1, 8, 3, 3, -2, 2)
{
    m_keywords = {DESTRUCTION, ORRUK, MAW_KRUSHA, IRONJAWZ, MONSTER, HERO, MEGABOSS, GORDRAKK};
    m_weapons = {&m_bellow, &m_hackaAndChoppa, &m_ripToofFist, &m_fistsAndTail};
}

bool MegabossOnMawKrusha::configure(WeaponOption weapons)
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_bellow);

    if (weapons == HackaAndChoppa)
    {
        model->addMeleeWeapon(&m_hackaAndChoppa);
    }
    else if (weapons == ChoppaAndRiptoofFist)
    {
        model->addMeleeWeapon(&m_ripToofFist);
    }
    model->addMeleeWeapon(&m_fistsAndTail);

    addModel(model);

    m_weaponOption = weapons;

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *MegabossOnMawKrusha::Create(const ParameterList &parameters)
{
    auto unit = new MegabossOnMawKrusha();
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, HackaAndChoppa);

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
    unit->setWarclan(warclan);

    bool ok = unit->configure(weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void MegabossOnMawKrusha::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Megaboss on Maw-Krusha", factoryMethod);
    }
}

int MegabossOnMawKrusha::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void MegabossOnMawKrusha::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_fistsAndTail.setAttacks(g_damageTable[damageIndex].m_fistsAttacks);
}

int MegabossOnMawKrusha::getDamageTableIndex() const
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

void MegabossOnMawKrusha::onCharged()
{
    Unit::onCharged();

    // Destructive Bulk
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

void MegabossOnMawKrusha::onStartCombat(PlayerId player)
{
    m_modelsSlainAtStartOfCombat = m_currentRecord.m_enemyModelsSlain;

    Ironjawz::onStartCombat(player);
}

Wounds MegabossOnMawKrusha::onEndCombat(PlayerId player)
{
    // Strength from Victory
    if (m_currentRecord.m_enemyModelsSlain > m_modelsSlainAtStartOfCombat)
    {
        heal(1);
        m_hackaAndChoppa.setAttacks(m_hackaAndChoppa.attacks()+1);
        m_ripToofFist.setAttacks(m_ripToofFist.attacks()+1);
    }
    return Ironjawz::onEndCombat(player);
}

Wounds MegabossOnMawKrusha::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    auto wounds = Ironjawz::computeReturnedDamage(weapon, saveRoll);
    // Rip-toof Fist
    if ((saveRoll == 6) && (m_weaponOption == ChoppaAndRiptoofFist))
    {
        wounds += {0, 1};
    }
    return wounds;
}

std::string MegabossOnMawKrusha::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        if (parameter.m_intValue == HackaAndChoppa)
        {
            return "Boss Gore-hacka and Choppa";
        }
        else if (parameter.m_intValue == ChoppaAndRiptoofFist)
        {
            return "Boss Choppa and Rip-toof Fist";
        }
    }

    return Ironjawz::ValueToString(parameter);
}

int MegabossOnMawKrusha::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Boss Gore-hacka and Choppa")
    {
        return HackaAndChoppa;
    }
    else if (enumString == "Boss Choppa and Rip-toof Fist")
    {
        return ChoppaAndRiptoofFist;
    }
    return Ironjawz::EnumStringToInt(enumString);
}

} //namespace Ironjawz