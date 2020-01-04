/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildPistoliers.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildPistoliers::Create,
    FreeguildPistoliers::ValueToString,
    FreeguildPistoliers::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", FreeguildPistoliers::MIN_UNIT_SIZE, FreeguildPistoliers::MIN_UNIT_SIZE,
            FreeguildPistoliers::MAX_UNIT_SIZE, FreeguildPistoliers::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Outrider Weapon", FreeguildPistoliers::RepeaterHandgun, FreeguildPistoliers::RepeaterHandgun,
            FreeguildPistoliers::BraceOfPistols, 1
        },
        {ParamType::Boolean, "Trumpeter", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildPistoliers::s_registered = false;

Unit *FreeguildPistoliers::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildPistoliers();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool trumpeter = GetBoolParam("Trumpeter", parameters, true);
    auto outriderWeapon = (WeaponOption) GetEnumParam("Outrider Weapon", parameters, RepeaterHandgun);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, trumpeter, outriderWeapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string FreeguildPistoliers::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Outrider Weapon")
    {
        if (parameter.m_intValue == RepeaterHandgun)
        {
            return "Repeater Handgun";
        }
        else if (parameter.m_intValue == BraceOfPistols)
        {
            return "Brace of Pistols";
        }
    }
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildPistoliers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Repeater Handgun")
    {
        return RepeaterHandgun;
    }
    else if (enumString == "Brace of Pistols")
    {
        return BraceOfPistols;
    }
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildPistoliers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild Pistoliers", factoryMethod);
    }
}

FreeguildPistoliers::FreeguildPistoliers() :
    CitizenOfSigmar("Freeguild Pistoliers", 12, WOUNDS, 5, 5, false),
    m_pistols(Weapon::Type::Missile, "Brace of Pistols", 9, 2, 4, 3, -1, 1),
    m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
    m_sabreAndPistolButt(Weapon::Type::Melee, "Sabre and Pistol Butt", 1, 2, 4, 4, 0, 1),
    m_hooves(Weapon::Type::Melee, "Stamping Hooves", 1, 2, 4, 5, 0, 1),
    m_sabreAndPistolButtOutrider(Weapon::Type::Melee, "Sabre and Pistol Butt", 1, 3, 4, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_PISTOLIERS};
}

bool FreeguildPistoliers::configure(int numModels, bool trumpeter, WeaponOption outriderWeapon)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_trumpeter = trumpeter;

    // Add the Sharpshooter
    auto bossModel = new Model(BASESIZE, WOUNDS);
    if (outriderWeapon == RepeaterHandgun)
    {
        bossModel->addMissileWeapon(&m_handgun);
    }
    else if (outriderWeapon == BraceOfPistols)
    {
        bossModel->addMeleeWeapon(&m_pistols);
    }
    bossModel->addMeleeWeapon(&m_sabreAndPistolButtOutrider);
    bossModel->addMeleeWeapon(&m_hooves);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
            auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_pistols);
        model->addMeleeWeapon(&m_sabreAndPistolButt);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void FreeguildPistoliers::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_pistols);
    visitor(m_handgun);
    visitor(m_sabreAndPistolButt);
    visitor(m_hooves);
    visitor(m_sabreAndPistolButtOutrider);
}

int FreeguildPistoliers::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_trumpeter) mod++;
    return mod;
}

int FreeguildPistoliers::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_trumpeter) mod++;
    return mod;
}

Rerolls FreeguildPistoliers::runRerolls() const
{
    // Reckless Riders
    return RerollFailed;
}

Rerolls FreeguildPistoliers::chargeRerolls() const
{
    // Reckless Riders
    return RerollFailed;
}

} // namespace CitiesOfSigmar