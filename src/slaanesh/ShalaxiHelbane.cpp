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
    ShalaxiHelbane::ValueToString,
    ShalaxiHelbane::EnumStringToInt,
    {
        {ParamType::Enum, "Weapon", ShalaxiHelbane::LivingWhip, ShalaxiHelbane::LivingWhip, ShalaxiHelbane::ShiningAegis, 1},
        {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders, SlaaneshBase::Godseekers, 1},
    },
    CHAOS,
    { SLAANESH }
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
    SlaaneshBase("Shalaxi Helbane", 14, WOUNDS, 10, 4, false),
    m_livingWhip(Weapon::Type::Missile, "Living Whip", 6, 1, 3, 3, -1, 1),
    m_soulpiercer(Weapon::Type::Melee, "Soulpiercer", 3, 1, 2, 2, -3, RAND_D6),
    m_impalingClaws(Weapon::Type::Melee, "Impaling Claws", 3, 2, 3, 3, -2, 5)
{
    m_keywords = {CHAOS, DAEMON, GREATER_DAEMON, SLAANESH, HEDONITE, MONSTER, HERO, WIZARD, KEEPER_OF_SECRETS, SHALAXI_HELBANE};
    m_weapons = {&m_livingWhip, &m_soulpiercer, &m_impalingClaws};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool ShalaxiHelbane::configure(WeaponOption weapon)
{
    auto model = new Model(BASESIZE, WOUNDS);

    m_weapon = weapon;

    if (m_weapon == LivingWhip)
    {
        model->addMissileWeapon(&m_livingWhip);
    }
    model->addMeleeWeapon(&m_soulpiercer);
    model->addMeleeWeapon(&m_impalingClaws);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *ShalaxiHelbane::Create(const ParameterList &parameters)
{
    auto unit = new ShalaxiHelbane();
    auto weapon = (WeaponOption)GetEnumParam("Weapon", parameters, LivingWhip);

    bool ok = unit->configure(weapon);
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

Wounds ShalaxiHelbane::applyWoundSave(const Wounds &wounds)
{
    Dice dice;

    if (m_weapon == ShiningAegis)
    {
        // Shining Aegis
        Dice::RollResult woundSaves, mortalSaves;
        dice.rollD6(wounds.normal, woundSaves);
        dice.rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(6);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(6);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }
    return Unit::applyWoundSave(wounds);
}

Wounds ShalaxiHelbane::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    if (!weapon->isMissile())
    {
        int damage = weapon->damage();
        // The Killing Stroke
        if ((target == m_meleeTarget) && (weapon->name() == m_soulpiercer.name()))
        {
            damage = 6;
        }

        // Delicate Precision
        if (woundRoll == 6)
        {
            return {0, damage};
        }
        else
        {
            return {damage, 0};
        }
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

std::string ShalaxiHelbane::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == LivingWhip) return "LivingWhip";
        else if (parameter.intValue == ShiningAegis) return "Shining Aegis";
    }
    return SlaaneshBase::ValueToString(parameter);
}

int ShalaxiHelbane::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Living Whip") return LivingWhip;
    else if (enumString == "Shining Aegis") return ShiningAegis;
    return SlaaneshBase::EnumStringToInt(enumString);
}

} // Slannesh
