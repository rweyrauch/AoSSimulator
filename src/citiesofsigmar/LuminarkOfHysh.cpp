/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/LuminarkOfHysh.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    LuminarkOfHysh::Create,
    LuminarkOfHysh::ValueToString,
    LuminarkOfHysh::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

struct TableEntry
{
    int m_move;
    int m_auraRange;
    int m_beamOfLight;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, LuminarkOfHysh::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 10, 2},
        {9, 8, 3},
        {8, 6, 4},
        {7,  4, 5},
        {6,  2, 6}
    };

bool LuminarkOfHysh::s_registered = false;

Unit *LuminarkOfHysh::Create(const ParameterList &parameters)
{
    auto unit = new LuminarkOfHysh();

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

std::string LuminarkOfHysh::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int LuminarkOfHysh::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void LuminarkOfHysh::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Luminark of Hysh", factoryMethod);
    }
}

LuminarkOfHysh::LuminarkOfHysh() :
    CitizenOfSigmar("Luminark of Hysh", 10, WOUNDS, 6, 4, false),
    m_beamOfLight(Weapon::Type::Missile, "Searing Beam of Light", 30, 1, 0, 0, 0, 0),
    m_wizardsStaff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3),
    m_arcaneTools(Weapon::Type::Melee, "Arcane Tools", 1, 4, 5, 5, 0, 1),
    m_hooves(Weapon::Type::Melee, "Steel-shod Hooves", 1, 4, 4, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, LUMINARK_OF_HYSH};
}

bool LuminarkOfHysh::configure(bool battlemage)
{
    if (battlemage)
    {
        addKeyword(WIZARD);
        addKeyword(HERO);
    }

    return false;
}

void LuminarkOfHysh::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_beamOfLight);
    visitor(m_wizardsStaff);
    visitor(m_arcaneTools);
    visitor(m_hooves);
}

int LuminarkOfHysh::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void LuminarkOfHysh::onWounded()
{
    Unit::onWounded();
}

int LuminarkOfHysh::getDamageTableIndex() const
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