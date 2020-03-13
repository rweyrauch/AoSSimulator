/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/DrakespawnChariots.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    DrakespawnChariots::Create,
    DrakespawnChariots::ValueToString,
    DrakespawnChariots::EnumStringToInt,
    DrakespawnChariots::ComputePoints,
    {
        {
            ParamType::Integer, "Models", DrakespawnChariots::MIN_UNIT_SIZE, DrakespawnChariots::MIN_UNIT_SIZE,
            DrakespawnChariots::MAX_UNIT_SIZE, DrakespawnChariots::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    { CITIES_OF_SIGMAR }
};

bool DrakespawnChariots::s_registered = false;

Unit *DrakespawnChariots::Create(const ParameterList &parameters)
{
    auto unit = new DrakespawnChariots();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string DrakespawnChariots::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int DrakespawnChariots::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void DrakespawnChariots::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Drakespawn Chariots", factoryMethod);
    }
}

DrakespawnChariots::DrakespawnChariots() :
    CitizenOfSigmar("Drakespawn Chariots", 10, WOUNDS, 7, 4, false),
    m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 5, 4, 0, 1),
    m_spear(Weapon::Type::Melee, "Barbed Spear", 2, 2, 3, 4, -1, 1),
    m_jaws(Weapon::Type::Melee, "Ferocious Jaws", 1, 4, 3, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, DRAKESPAWN_CHARIOTS};
    m_weapons = {&m_crossbow, &m_spear, &m_jaws};
}

bool DrakespawnChariots::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_crossbow);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int DrakespawnChariots::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

void DrakespawnChariots::onCharged()
{
    Unit::onCharged();

    // Scythed Runners
    for (int i = 0; i < remainingModels(); i++)
    {
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 1.0f)
        {
            if (Dice::rollD6() >= 2)
            {
                unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }
}

} // namespace CitiesOfSigmar