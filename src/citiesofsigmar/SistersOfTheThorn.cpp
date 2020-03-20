/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/SistersOfTheThorn.h"

namespace CitiesOfSigmar
{

bool SistersOfTheThorn::s_registered = false;

Unit *SistersOfTheThorn::Create(const ParameterList &parameters)
{
    auto unit = new SistersOfTheThorn();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standard = GetBoolParam("Standard Bearer", parameters, true);
    bool hornblower = GetBoolParam("Hornblower", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(numModels, standard, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string SistersOfTheThorn::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int SistersOfTheThorn::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void SistersOfTheThorn::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            SistersOfTheThorn::Create,
            SistersOfTheThorn::ValueToString,
            SistersOfTheThorn::EnumStringToInt,
            SistersOfTheThorn::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", SistersOfTheThorn::MIN_UNIT_SIZE, SistersOfTheThorn::MIN_UNIT_SIZE,
                    SistersOfTheThorn::MAX_UNIT_SIZE, SistersOfTheThorn::MIN_UNIT_SIZE
                },
                {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Sisters of the Thorn", factoryMethod);
    }
}

SistersOfTheThorn::SistersOfTheThorn() :
    CitizenOfSigmar("Sisters of the Thorn", 12, WOUNDS, 7, 5, false),
    m_javelin(Weapon::Type::Missile, "Blackbriar Javelin", 9, 2, 4, 4, -1, 1),
    m_staff(Weapon::Type::Melee, "Deepwood Coven Staff", 2, 1, 4, 4, 0, 1),
    m_antlersAndHooves(Weapon::Type::Melee, "Antlers and Horns", 1, 2, 4, 4, 0, 1),
    m_staffMaiden(Weapon::Type::Melee, "Deepwood Coven Staff", 2, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, WIZARD, SISTERS_OF_THE_THORN};
    m_weapons = {&m_javelin, &m_staff, &m_antlersAndHooves, &m_staffMaiden};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool SistersOfTheThorn::configure(int numModels, bool standardBearer, bool hornblower)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Handmaiden
    auto bossModel = new Model(BASESIZE, wounds());
    bossModel->addMissileWeapon(&m_javelin);
    bossModel->addMeleeWeapon(&m_staffMaiden);
    bossModel->addMeleeWeapon(&m_antlersAndHooves);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_javelin);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_antlersAndHooves);
        addModel(model);
    }

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = ComputePoints(numModels);

    return true;
}

int SistersOfTheThorn::runModifier() const
{
    auto mod = Unit::runModifier();
    if (m_hornblower) mod++;
    return mod;
}

int SistersOfTheThorn::chargeModifier() const
{
    auto mod = Unit::chargeModifier();
    if (m_hornblower) mod++;
    return mod;
}

int SistersOfTheThorn::braveryModifier() const
{
    auto mod = Unit::braveryModifier();
    if (m_standardBearer) mod++;
    return mod;
}

int SistersOfTheThorn::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace CitiesOfSigmar