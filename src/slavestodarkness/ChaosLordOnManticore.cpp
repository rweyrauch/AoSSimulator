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
static FactoryMethod factoryMethod = {
    ChaosLordOnManticore::Create,
    ChaosLordOnManticore::ValueToString,
    ChaosLordOnManticore::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapon", ChaosLordOnManticore::BladeAndShield, ChaosLordOnManticore::BladeAndLance,
            ChaosLordOnManticore::FlailAndDaggerfist, 1
        },
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

struct TableEntry
{
    int m_move;
    int m_fangsToWound;
    int m_tailAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, ChaosLordOnManticore::WOUNDS};
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
        s_registered = UnitFactory::Register("Chaos Lord On Manticore", factoryMethod);
    }
}

ChaosLordOnManticore::ChaosLordOnManticore() :
    Unit("Chaos Lord On Manticore", 12, WOUNDS, 8, 4, true),
    m_blade(Weapon::Type::Melee, "Daemon Blade", 1, 3, 3, 3, -1, RAND_D3),
    m_lance(Weapon::Type::Melee, "Chaos Lance", 2, 3, 3, 3, 0, 2),
    m_flail(Weapon::Type::Melee, "Chaos Flail", 2, 6, 3, 3, 0, 1),
    m_fangsAndClaws(Weapon::Type::Melee, "Honed Fangs and Claws", 1, 5, 3, 1, -1, 2),
    m_tail(Weapon::Type::Melee, "Shredding Tail", 3, 5, 4, 4, 0, 1)
{
    m_keywords = { CHAOS, MORTAL, MANTICORE, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, MONSTER, HERO, CHAOS_LORD };
}

bool ChaosLordOnManticore::configure(WeaponOption weapon)
{
    m_weapon = weapon;

    Model model(BASESIZE, WOUNDS);

    switch (weapon)
    {
        case WeaponOption::BladeAndLance:
            model.addMeleeWeapon(&m_blade);
            model.addMeleeWeapon(&m_lance);
            break;
        case WeaponOption::BladeAndShield:
            model.addMeleeWeapon(&m_blade);
            break;
        case WeaponOption::BladeAndDaggerfist:
            model.addMeleeWeapon(&m_blade);
            break;
        case WeaponOption::FlailAndLance:
            model.addMeleeWeapon(&m_flail);
            model.addMeleeWeapon(&m_lance);
            break;
        case WeaponOption::FlailAndShield:
            model.addMeleeWeapon(&m_flail);
            break;
        case WeaponOption::FlailAndDaggerfist:
            model.addMeleeWeapon(&m_flail);
            break;
    }

    model.addMeleeWeapon(&m_fangsAndClaws);
    model.addMeleeWeapon(&m_tail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ChaosLordOnManticore::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_lance);
    visitor(m_blade);
    visitor(m_flail);
    visitor(m_fangsAndClaws);
    visitor(m_tail);
}

int ChaosLordOnManticore::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void ChaosLordOnManticore::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_fangsAndClaws.setToWound(g_damageTable[damageIndex].m_fangsToWound);
    m_tail.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
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
    if (parameter.m_name == "Weapon")
    {
        switch (parameter.m_intValue)
        {
            case BladeAndLance: return "Daemon Blade and Chaos Lance";
            case FlailAndLance: return "Chaos Flail and Chaos Lance";
            case BladeAndShield: return "Daemon Blade and Chaos Runeshield";
            case FlailAndShield: return "Chaos Flail and Chaos Runeshield";
            case BladeAndDaggerfist: return "Daemon Blade and Daggerfist";
            case FlailAndDaggerfist: return "Chaos Flail and Daggerfist";
        }
    }
    return ParameterValueToString(parameter);
}

int ChaosLordOnManticore::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Daemon Blade and Chaos Lance") return BladeAndLance;
    else if (enumString == "Chaos Flail and Chaos Lance") return FlailAndLance;
    else if (enumString == "Daemon Blade and Chaos Runeshield") return BladeAndShield;
    else if (enumString == "Chaos Flail and Chaos Runeshield") return FlailAndShield;
    else if (enumString == "Daemon Blade and Daggerfist") return BladeAndDaggerfist;
    else if (enumString == "Chaos Flail and Daggerfist") return FlailAndDaggerfist;

    return 0;
}

} //namespace SlavesToDarkness
