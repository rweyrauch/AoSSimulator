/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FrostheartPhoenix.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FrostheartPhoenix::Create,
    FrostheartPhoenix::ValueToString,
    FrostheartPhoenix::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

struct TableEntry
{
    int m_move;
    int m_talonAttacks;
    int m_blizzazdAura;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 7, FrostheartPhoenix::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {16, 8, 9},
        {14, 6, 6},
        {12, 5, 3},
        {10,  4, 2},
        {8,  2, 1}
    };

bool FrostheartPhoenix::s_registered = false;

Unit *FrostheartPhoenix::Create(const ParameterList &parameters)
{
    auto unit = new FrostheartPhoenix();

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

std::string FrostheartPhoenix::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FrostheartPhoenix::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FrostheartPhoenix::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Frostheart Phoenix", factoryMethod);
    }
}

FrostheartPhoenix::FrostheartPhoenix() :
    CitizenOfSigmar("Frostheart Phoenix", 16, WOUNDS, 9, 4, true),
    m_talons(Weapon::Type::Melee, "Ice-cold Talons", 2, 8, 3, 3, -1, 2),
    m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, MONSTER, FROSTHEART_PHOENIX};
}

bool FrostheartPhoenix::configure(bool anointed)
{
    if (anointed)
    {
        addKeyword(HERO);
    }

    return false;
}

void FrostheartPhoenix::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_talons);
    visitor(m_halberd);
}

int FrostheartPhoenix::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void FrostheartPhoenix::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_talons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);

    Unit::onWounded();
}

int FrostheartPhoenix::getDamageTableIndex() const
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