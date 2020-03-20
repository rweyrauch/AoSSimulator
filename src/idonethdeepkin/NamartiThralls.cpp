/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/NamartiThralls.h>
#include <UnitFactory.h>
#include <iostream>

namespace IdonethDeepkin
{
static const int BASESIZE = 32;
static const int WOUNDS = 1;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 30;
static const int POINTS_PER_BLOCK = 130;
static const int POINTS_MAX_UNIT_SIZE = 360;

bool NamartiThralls::s_registered = false;

NamartiThralls::NamartiThralls() :
    IdonethDeepkinBase("Namarti Thralls", 6, WOUNDS, 6, 5, false),
    m_lanmariBlade(Weapon::Type::Melee, "Lanmari Blade", 1, 2, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, NAMARTI, THRALLS};
    m_weapons = {&m_lanmariBlade};
    m_battleFieldRole = Role::Battleline;
}

bool NamartiThralls::configure(int numModels, int numIconBearers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    if (numIconBearers > MAX_UNIT_SIZE / 10)
    {
        return false;
    }

    m_numIconBearers = numIconBearers;

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_lanmariBlade);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *NamartiThralls::Create(const ParameterList &parameters)
{
    auto unit = new NamartiThralls();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numIconBearers = GetIntParam("Icon Bearers", parameters, 0);

    auto enclave = (Enclave)GetEnumParam("Enclave", parameters, Enclave::None);
    unit->setEnclave(enclave);

    bool ok = unit->configure(numModels, numIconBearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void NamartiThralls::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            IdonethDeepkinBase::ValueToString,
            IdonethDeepkinBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Integer, "Icon Bearers", 0, 0, MAX_UNIT_SIZE / 10, 1},
                {ParamType::Enum, "Enclave", IdonethDeepkinBase::None, IdonethDeepkinBase::None, IdonethDeepkinBase::Briomdar, 1},
            },
            ORDER,
            { IDONETH_DEEPKIN }
        };

        s_registered = UnitFactory::Register("Namarti Thralls", factoryMethod);
    }
}

int NamartiThralls::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace IdonethDeepkin