/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/EternalGuard.h>
#include <UnitFactory.h>

namespace CitiesOfSigmar
{

bool EternalGuard::s_registered = false;

EternalGuard::EternalGuard() :
    CitizenOfSigmar("Eternal Guard", 6, WOUNDS, 7, 4, false),
    m_spearStave(Weapon::Type::Melee, "Spear-stave", 2, 2, 3, 4, 0, 1),
    m_spearStaveWarden(Weapon::Type::Melee, "Spear-stave", 2, 3, 3, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, ETERNAL_GUARD};
    m_weapons = {&m_spearStave, &m_spearStaveWarden};
}

bool EternalGuard::configure(int numModels, bool standardBearer, bool hornblower, bool gladeShields)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;
    m_gladeShields = gladeShields;

    auto lord = new Model(BASESIZE, WOUNDS);
    lord->addMeleeWeapon(&m_spearStaveWarden);
    addModel(lord);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_spearStave);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *EternalGuard::Create(const ParameterList &parameters)
{
    auto unit = new EternalGuard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);
    bool shields = GetBoolParam("Glade Shields", parameters, false);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standardBearer, hornblower, shields);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void EternalGuard::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            EternalGuard::Create,
            CitizenOfSigmar::ValueToString,
            CitizenOfSigmar::EnumStringToInt,
            EternalGuard::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", EternalGuard::MIN_UNIT_SIZE, EternalGuard::MIN_UNIT_SIZE,
                    EternalGuard::MAX_UNIT_SIZE, EternalGuard::MIN_UNIT_SIZE
                },
                {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                {ParamType::Boolean, "Hornblower", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                {ParamType::Boolean, "Glade Shields", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Eternal Guard", factoryMethod);
    }
}

int EternalGuard::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_hornblower) mod++;
    return mod;
}

int EternalGuard::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_hornblower) mod++;
    return mod;
}

int EternalGuard::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

int EternalGuard::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Form Fortress of Boughs
    if (!m_moved) mod++;

    return mod;
}

int EternalGuard::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toWoundModifier(weapon, target);

    // Form Fortress of Boughs
    if (!m_moved) mod++;

    return mod;
}

int EternalGuard::toSaveModifier(const Weapon *weapon) const
{
    auto mod = Unit::toSaveModifier(weapon);

    // Fortress of Boughs
    if (!m_moved) mod++;

    return mod;

}

int EternalGuard::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace CitiesOfSigmar
