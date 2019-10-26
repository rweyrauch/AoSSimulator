/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/CelestialHurricanum.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    CelestialHurricanum::Create,
    CelestialHurricanum::ValueToString,
    CelestialHurricanum::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

struct TableEntry
{
    int m_move;
    int m_portentsOfBattle;
    int m_stormOfShemtek;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, CelestialHurricanum::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 10, 3},
        {9, 8, 2},
        {8, 6, 2},
        {7,  4, 1},
        {6,  2, 1}
    };

bool CelestialHurricanum::s_registered = false;

Unit *CelestialHurricanum::Create(const ParameterList &parameters)
{
    auto unit = new CelestialHurricanum();

    auto battlemage = GetBoolParam("Battlemage", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(battlemage);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string CelestialHurricanum::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int CelestialHurricanum::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void CelestialHurricanum::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Celestial Hurricanum", factoryMethod);
    }
}

CelestialHurricanum::CelestialHurricanum() :
    CitizenOfSigmar("Celestial Hurricanum", 10, WOUNDS, 6, 4, false),
    m_stormOfShemtek(Weapon::Type::Missile, "Storm of Shemtek", 18, 1, 0, 0, 0, 0),
    m_wizardStaff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3),
    m_arcaneTools(Weapon::Type::Melee, "Arcane Tools", 1, 4, 5, 5, 0, 1),
    m_hooves(Weapon::Type::Melee, "Steel-shod Hooves", 1, 4, 4, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, CELESTIAL_HURRICANUM};
}

bool CelestialHurricanum::configure(bool battlemage)
{
    if (battlemage)
    {
        addKeyword(WIZARD);
        addKeyword(HERO);
    }

    return false;
}

void CelestialHurricanum::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_stormOfShemtek);
    visitor(m_wizardStaff);
    visitor(m_arcaneTools);
    visitor(m_hooves);
}

int CelestialHurricanum::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void CelestialHurricanum::onWounded()
{
    Unit::onWounded();
}

int CelestialHurricanum::getDamageTableIndex() const
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

} //namespace CitiesOfSigmar