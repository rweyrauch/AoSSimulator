/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FlamespyrePhoenix.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FlamespyrePhoenix::Create,
    FlamespyrePhoenix::ValueToString,
    FlamespyrePhoenix::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
        {ParamType::Boolean, "Anointed", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    ORDER,
    { CITIES_OF_SIGMAR }
};

struct TableEntry
{
    int m_move;
    int m_talonAttacks;
    int m_wakeOfFire;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 7, FlamespyrePhoenix::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {16, 6, 5},
        {14, 5, 4},
        {12, 4, 3},
        {10,  3, 2},
        {8,  2, 61}
    };

bool FlamespyrePhoenix::s_registered = false;

Unit *FlamespyrePhoenix::Create(const ParameterList &parameters)
{
    auto unit = new FlamespyrePhoenix();

    auto anointed = GetBoolParam("Anointed", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(anointed);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FlamespyrePhoenix::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FlamespyrePhoenix::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FlamespyrePhoenix::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Flamespyre Phoenix", factoryMethod);
    }
}

FlamespyrePhoenix::FlamespyrePhoenix() :
    CitizenOfSigmar("Flamespyre Phoenix", 16, WOUNDS, 8, 4, true),
    m_talons(Weapon::Type::Melee, "Flaming Talons", 2, 6, 4, 3, -1, 2),
    m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, MONSTER, FLAMESPYRE_PHOENIX};
    m_weapons = {&m_talons, &m_halberd};
}

bool FlamespyrePhoenix::configure(bool anointed)
{
    if (anointed)
    {
        addKeyword(HERO);
    }

    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_talons);
    if (anointed)
    {
        model->addMeleeWeapon(&m_halberd);
    }
    addModel(model);

    if (anointed)
    {
        m_points = POINTS_PER_UNIT_WITH_ANOINTED;
    }
    else
    {
        m_points = POINTS_PER_UNIT;
    }

    return true;
}

int FlamespyrePhoenix::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void FlamespyrePhoenix::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_talons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);

    Unit::onWounded();
}

int FlamespyrePhoenix::getDamageTableIndex() const
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

} // namespace CitiesOfSigmar