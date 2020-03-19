/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGeneralOnGriffon.h"

namespace CitiesOfSigmar
{

struct TableEntry
{
    int m_move;
    int m_clawAttacks;
    int m_beakDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, FreeguildGeneralOnGriffon::WOUNDS};
const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {15, 6, 4},
        {13, 5, 3},
        {11, 4, 2},
        {9,  3, 1},
        {7,  2, 1}
    };

bool FreeguildGeneralOnGriffon::s_registered = false;

Unit *FreeguildGeneralOnGriffon::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildGeneralOnGriffon();

    bool shield = GetBoolParam("Freeguild Shield", parameters, true);
    auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Lance);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(weapon, shield);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FreeguildGeneralOnGriffon::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildGeneralOnGriffon::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildGeneralOnGriffon::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Weapon", Lance, RuneSword, Lance, 1},
                {ParamType::Boolean, "Freeguild Shield", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Freeguild General on Griffon", factoryMethod);
    }
}

FreeguildGeneralOnGriffon::FreeguildGeneralOnGriffon() :
    CitizenOfSigmar("Freeguild General on Griffon", 15, WOUNDS, 7, 4, true),
    m_runesword(Weapon::Type::Melee, "Sigmarite Runesword", 1, 5, 3, 4, -1, 2),
    m_greathammer(Weapon::Type::Melee, "Sigmarite Greathammer", 1, 3, 3, 3, -2, RAND_D3),
    m_lance(Weapon::Type::Melee, "Freeguild Lance", 2, 4, 3, 4, -1, 2),
    m_claws(Weapon::Type::Melee, "Razor Claws", 2, 6, 4, 3, -1, 2),
    m_beak(Weapon::Type::Melee, "Deadly Beak", 2, 2, 3, 3, -2, 4)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, MONSTER, HERO,FREEGUILD_GENERAL};
    m_weapons = {&m_runesword, &m_greathammer, &m_lance, &m_claws, &m_beak};

    s_globalBraveryMod.connect(this, &FreeguildGeneralOnGriffon::piercingBloodroar, &m_connection);
}

FreeguildGeneralOnGriffon::~FreeguildGeneralOnGriffon()
{
    m_connection.disconnect();
}

bool FreeguildGeneralOnGriffon::configure(WeaponOption weapon, bool hasShield)
{
    auto model = new Model(BASESIZE, WOUNDS);
    if (weapon == RuneSword)
        model->addMeleeWeapon(&m_runesword);
    else if (weapon == Greathammer)
        model->addMeleeWeapon(&m_greathammer);
    else if (weapon == Lance)
        model->addMeleeWeapon(&m_lance);
    addModel(model);

    m_shield = hasShield;

    m_points = POINTS_PER_UNIT;

    return true;
}

int FreeguildGeneralOnGriffon::toSaveModifier(const Weapon *weapon) const
{
    auto mod = Unit::toSaveModifier(weapon);
    if (m_shield) mod++;
    return mod;
}

int FreeguildGeneralOnGriffon::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Charging Lance
    if (m_charged && (weapon->name() == m_lance.name()))
    {
        return -2;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

int FreeguildGeneralOnGriffon::runModifier() const
{
    // Skilled Rider
    auto mod = Unit::runModifier();
    if (!m_shield) mod++;
    return mod;
}

int FreeguildGeneralOnGriffon::chargeModifier() const
{
    // Skilled Rider
    auto mod = Unit::chargeModifier();
    if (!m_shield) mod++;
    return mod;
}

void FreeguildGeneralOnGriffon::onRestore()
{
    // Restore table-driven attributes
    onWounded();
}

void FreeguildGeneralOnGriffon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
    m_beak.setDamage(g_damageTable[damageIndex].m_beakDamage);
    m_move = g_damageTable[getDamageTableIndex()].m_move;

    Unit::onWounded();
}

int FreeguildGeneralOnGriffon::getDamageTableIndex() const
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

int FreeguildGeneralOnGriffon::piercingBloodroar(const Unit *target)
{
    // Piercing Bloodroar
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 8.0f))
    {
        return -1;
    }

    return 0;
}


} // namespace CitiesOfSigmar