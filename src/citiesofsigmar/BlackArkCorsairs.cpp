/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/BlackArkCorsairs.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    BlackArkCorsairs::Create,
    BlackArkCorsairs::ValueToString,
    BlackArkCorsairs::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", BlackArkCorsairs::MIN_UNIT_SIZE, BlackArkCorsairs::MIN_UNIT_SIZE,
            BlackArkCorsairs::MAX_UNIT_SIZE, BlackArkCorsairs::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", BlackArkCorsairs::WickedCutlass, BlackArkCorsairs::RepeaterHandbow,
            BlackArkCorsairs::WickedCutlass, 1
        },
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool BlackArkCorsairs::s_registered = false;

Unit *BlackArkCorsairs::Create(const ParameterList &parameters)
{
    auto unit = new BlackArkCorsairs();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool hornblower = GetBoolParam("Hornblower", parameters, true);
    auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, WickedCutlass);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, hornblower, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string BlackArkCorsairs::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == RepeaterHandbow)
        {
            return "Repeater Handbow";
        }
        else if (parameter.m_intValue == WickedCutlass)
        {
            return "Wicked Cutlass";
        }
    }
    return CitizenOfSigmar::ValueToString(parameter);
}

int BlackArkCorsairs::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Repeater Handbow")
    {
        return RepeaterHandbow;
    }
    else if (enumString == "Wicked Cutlass")
    {
        return WickedCutlass;
    }
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void BlackArkCorsairs::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Black Ark Corsairs", factoryMethod);
    }
}

BlackArkCorsairs::BlackArkCorsairs() :
    CitizenOfSigmar("Black Ark Corsairs", 6, WOUNDS, 6, 5, false),
    m_handbow(Weapon::Type::Missile, "Repeater Handbow", 9, 2, 5, 4, 0, 1),
    m_cutlass(Weapon::Type::Melee, "Wicked Cutlass", 1, 1, 4, 4, 0, 1),
    m_blade(Weapon::Type::Melee, "Vicious Blade", 1, 1, 4, 5, 0, 1),
    m_handbowReaver(Weapon::Type::Missile, "Repeater Handbow", 9, 2, 4, 4, 0, 1),
    m_cutlassReaver(Weapon::Type::Melee, "Wicked Cutlass", 1, 1, 3, 4, 0, 1),
    m_bladeReaver(Weapon::Type::Melee, "Vicious Blade", 1, 1, 3, 5, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, BLACK_ARK_CORSAIRS};
}

bool BlackArkCorsairs::configure(int numModels, bool standardBearer, bool hornblower, WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Reaver
    auto bossModel = new Model(BASESIZE, WOUNDS);
    if (weapons == RepeaterHandbow)
    {
        bossModel->addMissileWeapon(&m_handbowReaver);
    }
    else if (weapons == WickedCutlass)
    {
        bossModel->addMeleeWeapon(&m_cutlassReaver);
    }
    bossModel->addMeleeWeapon(&m_bladeReaver);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == RepeaterHandbow)
        {
            model->addMissileWeapon(&m_handbow);
        }
        else if (weapons == WickedCutlass)
        {
            model->addMeleeWeapon(&m_cutlass);
        }
        model->addMeleeWeapon(&m_blade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void BlackArkCorsairs::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_handbow);
    visitor(m_cutlass);
    visitor(m_blade);
    visitor(m_handbowReaver);
    visitor(m_cutlassReaver);
    visitor(m_bladeReaver);
}

int BlackArkCorsairs::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_hornblower) mod++;
    return mod;
}

int BlackArkCorsairs::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_hornblower) mod++;
    return mod;
}

int BlackArkCorsairs::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

int BlackArkCorsairs::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Flashing Steel
    if (remainingModels() >= 15) mod++;

    return mod;
}

}//namespace CitiesOfSigmar