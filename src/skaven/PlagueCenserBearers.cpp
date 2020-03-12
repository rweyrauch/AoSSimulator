/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlagueCenserBearers.h>
#include <UnitFactory.h>

namespace Skaven
{
bool PlagueCenserBearers::s_registered = false;

Unit *PlagueCenserBearers::Create(const ParameterList &parameters)
{
    auto unit = new PlagueCenserBearers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int PlagueCenserBearers::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

void PlagueCenserBearers::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,MAX_UNIT_SIZE, MIN_UNIT_SIZE},
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Plague Censer Bearers", *factoryMethod);
    }
}

PlagueCenserBearers::PlagueCenserBearers() :
    Skaventide("Plague Censer Bearers", 6, WOUNDS, 5, 6, false),
    m_censer(Weapon::Type::Melee, "Plague Censer", 2, 2, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, PLAGUE_CENSER_BEARERS};
    m_weapons = {&m_censer};
}

bool PlagueCenserBearers::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_censer);
        addModel(model);
    }
    m_points = ComputePoints(numModels);

    return true;
}
} //namespace Skaven
