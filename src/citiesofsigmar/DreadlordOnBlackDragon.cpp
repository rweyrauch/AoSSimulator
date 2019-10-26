/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/DreadlordOnBlackDragon.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    DreadlordOnBlackDragon::Create,
    DreadlordOnBlackDragon::ValueToString,
    DreadlordOnBlackDragon::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapon", DreadlordOnBlackDragon::LanceAndShield, DreadlordOnBlackDragon::ExileBladeAndShield,
            DreadlordOnBlackDragon::PairExileBlades, 1
        },
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

struct TableEntry
{
    int m_move;
    int m_jawsToWound;
    int m_clawAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, DreadlordOnBlackDragon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 2, 6},
        {10, 3, 5},
        {8, 3, 4},
        {6,  4, 3},
        {4,  4, 2}
    };

bool DreadlordOnBlackDragon::s_registered = false;

Unit *DreadlordOnBlackDragon::Create(const ParameterList &parameters)
{
    auto unit = new DreadlordOnBlackDragon();

    auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, LanceAndShield);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(weapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string DreadlordOnBlackDragon::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        if (parameter.m_intValue == ExileBladeAndShield)
        {
            return "Exile Blade and Tyrant Shield";
        }
        else if (parameter.m_intValue == ExileBladeAndCrossbow)
        {
            return "Exile Blade and Repeater Crossbow";
        }
        else if (parameter.m_intValue == LanceAndShield)
        {
            return "Lance of Spite and Tyrant Shield";
        }
        else if (parameter.m_intValue == LanceAndCrossbow)
        {
            return "Lance of Spite and Repeater Crossbow";
        }
        else if (parameter.m_intValue == PairExileBlades)
        {
            return "Pair of Exile Blades";
        }
    }
    return CitizenOfSigmar::ValueToString(parameter);
}

int DreadlordOnBlackDragon::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Exile Blade and Tyrant Shield")
    {
        return ExileBladeAndShield;
    }
    else if (enumString == "Exile Blade and Repeater Crossbow")
    {
        return ExileBladeAndCrossbow;
    }
    else if (enumString == "Lance of Spite and Tyrant Shield")
    {
        return LanceAndShield;
    }
    else if (enumString == "Lance of Spite and Repeater Crossbow")
    {
        return LanceAndCrossbow;
    }
    else if (enumString == "Pair of Exile Blades")
    {
        return PairExileBlades;
    }
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void DreadlordOnBlackDragon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dreadlord on Black Dragon", factoryMethod);
    }
}

DreadlordOnBlackDragon::DreadlordOnBlackDragon() :
    CitizenOfSigmar("Dreadlord on Black Dragon", 12, WOUNDS, 8, 4, true),
    m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 4, 4, 0, 1),
    m_noxiousBreath(Weapon::Type::Missile, "Noxious Breath", 6, 1, 0, 0, 0, 0),
    m_blade(Weapon::Type::Melee, "Exile Blade", 1, 6, 3, 4, 0, 1),
    m_lance(Weapon::Type::Melee, "Lance of Spite", 2, 3, 3, 3, -1, 1),
    m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 3, 3, 4, 2, -2, RAND_D6),
    m_claws(Weapon::Type::Melee, "Razor-sharp Claws", 2, 6, 4, 3, -1, 2)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, MONSTER, HERO, DREADLORD};
}

bool DreadlordOnBlackDragon::configure(WeaponOption weapon)
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_noxiousBreath);
    model.addMeleeWeapon(&m_jaws);
    model.addMeleeWeapon(&m_claws);
    if (weapon == LanceAndShield)
    {
        model.addMeleeWeapon(&m_lance);
    }
    else if (weapon == LanceAndCrossbow)
    {
        model.addMeleeWeapon(&m_lance);
        model.addMissileWeapon(&m_crossbow);
    }
    else if (weapon == ExileBladeAndShield)
    {
        model.addMeleeWeapon(&m_blade);
    }
    else if (weapon == ExileBladeAndCrossbow)
    {
        model.addMeleeWeapon(&m_blade);
        model.addMissileWeapon(&m_crossbow);
    }
    else if (weapon == PairExileBlades)
    {
        model.addMeleeWeapon(&m_blade);
    }
    addModel(model);

    m_weaponOption = weapon;
    m_points = POINTS_PER_UNIT;

    return true;
}

void DreadlordOnBlackDragon::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_crossbow);
    visitor(m_noxiousBreath);
    visitor(m_blade);
    visitor(m_lance);
    visitor(m_jaws);
    visitor(m_claws);
}

int DreadlordOnBlackDragon::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void DreadlordOnBlackDragon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_jaws.setToWound(g_damageTable[damageIndex].m_jawsToWound);
    m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);

    Unit::onWounded();
}

int DreadlordOnBlackDragon::getDamageTableIndex() const
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

Wounds DreadlordOnBlackDragon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Lance of Spite
    if (m_charged && (weapon->name() == m_lance.name()))
    {
        return {2, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int DreadlordOnBlackDragon::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Lance of Spite
    if (m_charged && (weapon->name() == m_lance.name()))
    {
        return -2;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

} // namespace CitiesOfSigmar