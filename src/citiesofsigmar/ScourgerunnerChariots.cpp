/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/ScourgerunnerChariots.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    ScourgerunnerChariots::Create,
    ScourgerunnerChariots::ValueToString,
    ScourgerunnerChariots::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", ScourgerunnerChariots::MIN_UNIT_SIZE, ScourgerunnerChariots::MIN_UNIT_SIZE,
            ScourgerunnerChariots::MAX_UNIT_SIZE, ScourgerunnerChariots::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool ScourgerunnerChariots::s_registered = false;

Unit *ScourgerunnerChariots::Create(const ParameterList &parameters)
{
    auto unit = new ScourgerunnerChariots();

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

std::string ScourgerunnerChariots::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int ScourgerunnerChariots::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void ScourgerunnerChariots::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Scourgerunner Chariots", factoryMethod);
    }
}

ScourgerunnerChariots::ScourgerunnerChariots() :
    CitizenOfSigmar("Scourgerunner Chariots", 12, WOUNDS, 6, 5, false),
    m_harpoon(Weapon::Type::Missile, "Ravager Harpoon", 18, 2, 3, 3, -1, RAND_D3),
    m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 5, 4, 0, 1),
    m_hookSpear(Weapon::Type::Melee, "Hook-spear", 2, 2, 4, 4, 0, 1),
    m_bite(Weapon::Type::Melee, "Vicious Bit", 1, 4, 4, 5, 0, 1),
    m_harpoonMaster(Weapon::Type::Missile, "Ravager Harpoon", 18, 2, 2, 3, -1, RAND_D3),
    m_crossbowMaster(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, SCOURGERUNNER_CHARIOTS};
}

bool ScourgerunnerChariots::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the Master
    auto bossModel = new Model(BASESIZE, WOUNDS);
    bossModel->addMissileWeapon(&m_crossbowMaster);
    bossModel->addMissileWeapon(&m_harpoonMaster);
    bossModel->addMeleeWeapon(&m_hookSpear);
    bossModel->addMeleeWeapon(&m_bite);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
            auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_crossbow);
        model->addMissileWeapon(&m_harpoon);
        model->addMeleeWeapon(&m_hookSpear);
        model->addMeleeWeapon(&m_bite);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void ScourgerunnerChariots::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_harpoon);
    visitor(m_crossbow);
    visitor(m_hookSpear);
    visitor(m_bite);
    visitor(m_harpoonMaster);
    visitor(m_crossbowMaster);
}

Wounds ScourgerunnerChariots::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Lay the Beast Low
    if ((hitRoll == 6) && (weapon->name() == m_harpoon.name()))
    {
        Dice dice;
        return { 0, dice.rollD3()};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace CitiesOfSigmar