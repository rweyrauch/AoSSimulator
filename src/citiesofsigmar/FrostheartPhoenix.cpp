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
static const int BASESIZE = 105;
static const int WOUNDS = 12;
static const int POINTS_PER_UNIT = 220;
static const int POINTS_PER_UNIT_WITH_ANOINTED = 320;

struct TableEntry
{
    int m_move;
    int m_talonAttacks;
    float m_blizzazdAura;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 7, WOUNDS};
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
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
                {ParamType::Boolean, "Anointed", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };

        s_registered = UnitFactory::Register("Frostheart Phoenix", factoryMethod);
    }
}

FrostheartPhoenix::FrostheartPhoenix() :
    CitizenOfSigmar("Frostheart Phoenix", 16, WOUNDS, 9, 4, true),
    m_talons(Weapon::Type::Melee, "Ice-cold Talons", 2, 8, 3, 3, -1, 2),
    m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, MONSTER, FROSTHEART_PHOENIX};
    m_weapons = {&m_talons, &m_halberd};

    s_globalToWoundMod.connect(this, &FrostheartPhoenix::blizzardAura, &m_connection);
}

FrostheartPhoenix::~FrostheartPhoenix()
{
    m_connection.disconnect();
}

bool FrostheartPhoenix::configure(bool anointed)
{
    if (anointed)
    {
        addKeyword(HERO);
    }

    auto model = new Model(BASESIZE, wounds());
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

void FrostheartPhoenix::onRestore()
{
    // Restore table-driven attributes
    onWounded();
}

void FrostheartPhoenix::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_talons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);
    m_move = g_damageTable[getDamageTableIndex()].m_move;

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

int FrostheartPhoenix::blizzardAura(const Unit* attacker, const Weapon *weapon, const Unit *target)
{
    // Blizzard Aura
    if (!weapon->isMissile() && (target->owningPlayer() != owningPlayer()) &&
        (distanceTo(target) <= g_damageTable[getDamageTableIndex()].m_blizzazdAura))
    {
        return -1;
    }

    return 0;
}

Wounds FrostheartPhoenix::applyWoundSave(const Wounds &wounds)
{
    if (hasKeyword(HERO))
    {
        // Witness to Destiny
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(4);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(4);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }
    return Unit::applyWoundSave(wounds);
}

int FrostheartPhoenix::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace CitiesOfSigmar