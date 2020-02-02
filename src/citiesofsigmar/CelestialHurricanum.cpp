/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/CelestialHurricanum.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    CelestialHurricanum::Create,
    CelestialHurricanum::ValueToString,
    CelestialHurricanum::EnumStringToInt,
    CelestialHurricanum::ComputePoints,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
        {ParamType::Boolean, "Battlemage", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    ORDER,
    { CITIES_OF_SIGMAR }
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
    m_weapons = {&m_stormOfShemtek, &m_wizardStaff, &m_arcaneTools, &m_hooves};
}

bool CelestialHurricanum::configure(bool battlemage)
{
    if (battlemage)
    {
        addKeyword(WIZARD);
        addKeyword(HERO);

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_stormOfShemtek);
    model->addMeleeWeapon(&m_arcaneTools);
    model->addMeleeWeapon(&m_hooves);
    if (battlemage)
    {
        model->addMeleeWeapon(&m_wizardStaff);
    }
    addModel(model);

    if (battlemage)
    {
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_points = POINTS_PER_UNIT_WITH_BATTLEMAGE;
    }
    else
    {
        m_points = POINTS_PER_UNIT;
    }

    return true;
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

void CelestialHurricanum::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    if (m_shootingTarget)
    {
        // Storm of Shemtek
        auto dist = distanceTo(m_shootingTarget);
        if (dist <= m_stormOfShemtek.range())
        {
            Dice dice;
            Dice::RollResult result;
            dice.rollD6(g_damageTable[getDamageTableIndex()].m_stormOfShemtek, result);
            auto numHits = result.rollsGE(2);
            for (auto i = 0; i < numHits; i++)
            {
                auto mw = dice.rollD3();
                m_shootingTarget->applyDamage({0, mw});
            }
        }
    }
}

int CelestialHurricanum::castingModifier() const
{
    auto mod = Unit::castingModifier();

    // Celestial Battlemage
    if (Board::Instance()->getRealm() == Azyr) mod++;

    return mod;
}

} //namespace CitiesOfSigmar