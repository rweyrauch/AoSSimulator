/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/FreeguildGreatswords.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    FreeguildGreatswords::Create,
    FreeguildGreatswords::ValueToString,
    FreeguildGreatswords::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", FreeguildGreatswords::MIN_UNIT_SIZE, FreeguildGreatswords::MIN_UNIT_SIZE,
            FreeguildGreatswords::MAX_UNIT_SIZE, FreeguildGreatswords::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool FreeguildGreatswords::s_registered = false;

Unit *FreeguildGreatswords::Create(const ParameterList &parameters)
{
    auto unit = new FreeguildGreatswords();

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

std::string FreeguildGreatswords::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int FreeguildGreatswords::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void FreeguildGreatswords::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Freeguild Greatswords", factoryMethod);
    }
}

FreeguildGreatswords::FreeguildGreatswords() :
    CitizenOfSigmar("Freeguild Greatswords", 5, WOUNDS, 6, 4, false),
    m_zweihander(Weapon::Type::Melee, "Zweihander", 1, 2, 3, 3, -1, 1),
    m_zweihanderChampion(Weapon::Type::Melee, "Zweihander", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, FREEGUILD_GREATSWORDS};
}

bool FreeguildGreatswords::configure(int numModels, bool standardBearer, bool hornblower)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    // Add the Champion
    Model bossModel(BASESIZE, WOUNDS);
    bossModel.addMeleeWeapon(&m_zweihanderChampion);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_zweihander);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void FreeguildGreatswords::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_zweihander);
    visitor(m_zweihanderChampion);
}

} // namespace CitiesOfSigmar