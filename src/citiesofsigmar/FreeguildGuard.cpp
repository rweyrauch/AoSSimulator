/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGuard.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildGuard::Create,
    FreeguildGuard::ValueToString,
    FreeguildGuard::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", FreeguildGuard::MIN_UNIT_SIZE, FreeguildGuard::MIN_UNIT_SIZE,
            FreeguildGuard::MAX_UNIT_SIZE, FreeguildGuard::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", FreeguildGuard::Spear, FreeguildGuard::Halberd,
            FreeguildGuard::Sword, 1
        },
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildGuard::s_registered = false;

Unit *FreeguildGuard::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildGuard();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool drummer = GetBoolParam("Drummer", parameters, true);
    auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Halberd);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, drummer, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FreeguildGuard::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == Halberd)
        {
            return "Halberd";
        }
        else if (parameter.m_intValue == Spear)
        {
            return "Spear";
        }
        else if (parameter.m_intValue == Sword)
        {
            return "Sword";
        }
    }
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildGuard::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Halberd")
    {
        return Halberd;
    }
    else if (enumString == "Spear")
    {
        return Spear;
    }
    else if (enumString == "Sword")
    {
        return Sword;
    }
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild Guard", factoryMethod);
    }
}

FreeguildGuard::FreeguildGuard() :
    CitizenOfSigmar("Freeguild Guard", 5, WOUNDS, 5, 5, false),
    m_halberd(Weapon::Type::Melee, "Freeguild Halberd", 1, 1, 4, 3, -1, 1),
    m_spear(Weapon::Type::Melee, "Freeguild Spear", 2, 1, 4, 4, 0, 1),
    m_sword(Weapon::Type::Melee, "Freeguild Sword", 1, 1, 4, 4, 0, 1),
    m_halberdSergeant(Weapon::Type::Melee, "Freeguild Halberd", 1, 2, 4, 3, -1, 1),
    m_spearSergeant(Weapon::Type::Melee, "Freeguild Spear", 2, 2, 4, 4, 0, 1),
    m_swordSergeant(Weapon::Type::Melee, "Freeguild Sword", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_GUARD};
    m_weapons = {&m_halberd, &m_spear, &m_sword, &m_halberdSergeant, &m_spearSergeant, &m_swordSergeant};
}

bool FreeguildGuard::configure(int numModels, bool standardBearer, bool drummer, WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_drummer = drummer;

    // Add the Sergeant
    auto bossModel = new Model(BASESIZE, WOUNDS);
    if (weapons == Halberd)
    {
        bossModel->addMeleeWeapon(&m_halberdSergeant);
    }
    else if (weapons == Spear)
    {
        bossModel->addMeleeWeapon(&m_spearSergeant);
    }
    else if (weapons == Sword)
    {
        bossModel->addMeleeWeapon(&m_swordSergeant);
    }
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == Halberd)
        {
            model->addMeleeWeapon(&m_halberd);
        }
        else if (weapons == Spear)
        {
            model->addMeleeWeapon(&m_spearSergeant);
        }
        else if (weapons == Sword)
        {
            model->addMeleeWeapon(&m_sword);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

int FreeguildGuard::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_drummer) mod++;
    return mod;
}

int FreeguildGuard::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_drummer) mod++;
    return mod;
}

int FreeguildGuard::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

int FreeguildGuard::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Massed Ranks
    if (remainingModels() >= 20) mod += 2;
    else if (remainingModels() >= 10) mod++;

    // Wall of Spears
    if (target->charged() && (weapon->name() == m_spear.name())) mod++;

    return mod;
}

int FreeguildGuard::toSaveModifier(const Weapon *weapon) const
{
    auto mod = Unit::toSaveModifier(weapon);

    // Parry and Block
    if (weapon->name() == m_sword.name()) mod++;

    return mod;
}

} // namespace CitiesOfSigmar