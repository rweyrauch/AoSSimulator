/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/SkryeAcolytes.h>
#include <UnitFactory.h>

namespace Skaven
{
bool SkryeAcolytes::s_registered = false;

Unit *SkryeAcolytes::Create(const ParameterList &parameters)
{
    auto unit = new SkryeAcolytes();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int SkryeAcolytes::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

void SkryeAcolytes::Init()
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

        s_registered = UnitFactory::Register("Skrye Acolytes", *factoryMethod);
    }
}

SkryeAcolytes::SkryeAcolytes() :
    Skaventide("Skrye Acolytes", 6, WOUNDS, 4, 6, false),
    m_globe(Weapon::Type::Missile, "Poisoned Wind Globe", 8, 1, 4, 4, -2, RAND_D3),
    m_knife(Weapon::Type::Melee, "Rusty Knife", 1, 1, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, SKRYRE_ACOLYTES};
    m_weapons = {&m_globe, &m_knife};
}

bool SkryeAcolytes::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_globe);
        model->addMeleeWeapon(&m_knife);
        addModel(model);
    }
    m_points = ComputePoints(numModels);

    return true;
}
} //namespace Skaven
