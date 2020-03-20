/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosLordOnManticore.h"

namespace SlavesToDarkness
{
static const int BASESIZE = 90; // x52 oval
static const int WOUNDS = 12;
static const int POINTS_PER_UNIT = 280;

struct TableEntry
{
    int m_move;
    int m_fangsToWound;
    int m_tailAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 1, 5},
        {10, 2,4},
        {8, 3,3},
        {6, 4,2},
        {6, 5,1}
    };

bool ChaosLordOnManticore::s_registered = false;

Unit *ChaosLordOnManticore::Create(const ParameterList &parameters)
{
    auto unit = new ChaosLordOnManticore();

    auto weapon = (WeaponOption)GetEnumParam("Weapon", parameters, BladeAndShield);

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    auto mark = (MarkOfChaos)GetEnumParam("Mark of Chaos", parameters, Undivided);
    unit->setMarkOfChaos(mark);

    bool ok = unit->configure(weapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosLordOnManticore::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            ChaosLordOnManticore::Create,
            ChaosLordOnManticore::ValueToString,
            ChaosLordOnManticore::EnumStringToInt,
            ChaosLordOnManticore::ComputePoints,
            {
                {
                    ParamType::Enum, "Weapon", ChaosLordOnManticore::BladeAndShield, ChaosLordOnManticore::BladeAndLance,
                    ChaosLordOnManticore::FlailAndDaggerfist, 1
                },
                {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
            },
            CHAOS,
            { SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE }
        };
        s_registered = UnitFactory::Register("Chaos Lord On Manticore", factoryMethod);
    }
}

ChaosLordOnManticore::ChaosLordOnManticore() :
    SlavesToDarknessBase("Chaos Lord On Manticore", 12, WOUNDS, 8, 4, true),
    m_blade(Weapon::Type::Melee, "Daemon Blade", 1, 3, 3, 3, -1, RAND_D3),
    m_lance(Weapon::Type::Melee, "Chaos Lance", 2, 3, 3, 3, 0, 2),
    m_flail(Weapon::Type::Melee, "Chaos Flail", 2, 6, 3, 3, 0, 1),
    m_fangsAndClaws(Weapon::Type::Melee, "Honed Fangs and Claws", 1, 5, 3, 1, -1, 2),
    m_tail(Weapon::Type::Melee, "Shredding Tail", 3, 5, 4, 4, 0, 1)
{
    m_keywords = { CHAOS, MORTAL, MANTICORE, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, MONSTER, HERO, CHAOS_LORD };
    m_weapons = {&m_blade, &m_lance, &m_flail, &m_fangsAndClaws, &m_tail};
}

bool ChaosLordOnManticore::configure(WeaponOption weapon)
{
    m_weapon = weapon;

    auto model = new Model(BASESIZE, wounds());

    switch (weapon)
    {
        case WeaponOption::BladeAndLance:
            model->addMeleeWeapon(&m_blade);
            model->addMeleeWeapon(&m_lance);
            break;
        case WeaponOption::BladeAndShield:
        case WeaponOption::BladeAndDaggerfist:
            model->addMeleeWeapon(&m_blade);
            break;
        case WeaponOption::FlailAndLance:
            model->addMeleeWeapon(&m_flail);
            model->addMeleeWeapon(&m_lance);
            break;
        case WeaponOption::FlailAndShield:
        case WeaponOption::FlailAndDaggerfist:
            model->addMeleeWeapon(&m_flail);
            break;
    }

    model->addMeleeWeapon(&m_fangsAndClaws);
    model->addMeleeWeapon(&m_tail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ChaosLordOnManticore::onRestore()
{
    // Reset table-drive attributes
    onWounded();
}

void ChaosLordOnManticore::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_fangsAndClaws.setToWound(g_damageTable[damageIndex].m_fangsToWound);
    m_tail.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

int ChaosLordOnManticore::getDamageTableIndex() const
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

std::string ChaosLordOnManticore::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        switch (parameter.intValue)
        {
            case BladeAndLance: return "Daemon Blade and Chaos Lance";
            case FlailAndLance: return "Chaos Flail and Chaos Lance";
            case BladeAndShield: return "Daemon Blade and Chaos Runeshield";
            case FlailAndShield: return "Chaos Flail and Chaos Runeshield";
            case BladeAndDaggerfist: return "Daemon Blade and Daggerfist";
            case FlailAndDaggerfist: return "Chaos Flail and Daggerfist";
            default:
                break;
        }
    }
    return SlavesToDarknessBase::ValueToString(parameter);
}

int ChaosLordOnManticore::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Daemon Blade and Chaos Lance") return BladeAndLance;
    else if (enumString == "Chaos Flail and Chaos Lance") return FlailAndLance;
    else if (enumString == "Daemon Blade and Chaos Runeshield") return BladeAndShield;
    else if (enumString == "Chaos Flail and Chaos Runeshield") return FlailAndShield;
    else if (enumString == "Daemon Blade and Daggerfist") return BladeAndDaggerfist;
    else if (enumString == "Chaos Flail and Daggerfist") return FlailAndDaggerfist;

    return SlavesToDarknessBase::EnumStringToInt(enumString);
}

Wounds ChaosLordOnManticore::applyWoundSave(const Wounds &wounds)
{
    auto savedWounds = Unit::applyWoundSave(wounds);
    Dice::RollResult result;
    // Chaos Runeshield
    Dice::rollD6(savedWounds.mortal, result);
    savedWounds.mortal -= result.rollsGE(5);
    return savedWounds;
}

Wounds ChaosLordOnManticore::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    // Daemonbound
    if ((hitRoll == 6) && (weapon->name() == m_blade.name()))
    {
        damage.mortal++;
    }
    // Chaos Lance
    else if (m_charged && (weapon->name() == m_lance.name()))
    {
        damage.normal++;
    }
    return damage;
}

Wounds ChaosLordOnManticore::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    auto damage = Unit::computeReturnedDamage(weapon, saveRoll);

    // Daggerfist
    if ((saveRoll == 6) && (!weapon->isMissile()) &&
        (m_weapon == WeaponOption::FlailAndDaggerfist || m_weapon == WeaponOption::BladeAndDaggerfist))
    {
        damage.mortal++;
    }
    return damage;
}

int ChaosLordOnManticore::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);

    // Chaos Lance
    if (m_charged && (weapon->name() == m_lance.name()))
    {
        rend -= 2;
    }
    return rend;
}

Rerolls ChaosLordOnManticore::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Territorial Predator
    if ((weapon->name() == m_fangsAndClaws.name()) && (target->hasKeyword(MONSTER)))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

int ChaosLordOnManticore::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} //namespace SlavesToDarkness
