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

static FactoryMethod factoryMethod = {
    NamartiThralls::Create,
    IdonethDeepkinBase::ValueToString,
    IdonethDeepkinBase::EnumStringToInt,
    NamartiThralls::ComputePoints,
    {
        {
            ParamType::Integer, "Models", NamartiThralls::MIN_UNIT_SIZE, NamartiThralls::MIN_UNIT_SIZE,
            NamartiThralls::MAX_UNIT_SIZE, NamartiThralls::MIN_UNIT_SIZE
        },
        {ParamType::Integer, "Icon Bearers", 0, 0, NamartiThralls::MAX_UNIT_SIZE / 10, 1},
        {ParamType::Enum, "Enclave", IdonethDeepkinBase::None, IdonethDeepkinBase::None, IdonethDeepkinBase::Briomdar, 1},
    },
    ORDER,
    { IDONETH_DEEPKIN }
};


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
        auto model = new Model(BASESIZE, WOUNDS);
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