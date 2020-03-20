/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Nurglings.h>
#include <UnitFactory.h>

namespace Nurgle
{
static const int BASESIZE = 40;
static const int WOUNDS = 4;
static const int MIN_UNIT_SIZE = 3;
static const int MAX_UNIT_SIZE = 12;
static const int POINTS_PER_BLOCK = 100;
static const int POINTS_MAX_UNIT_SIZE = 400;

bool Nurglings::s_registered = false;

Nurglings::Nurglings() :
    NurgleBase("Nurglings", 5, WOUNDS, 10, 6, false),
    m_teeth(Weapon::Type::Melee, "Tiny Razor-sharp Teeth", 1, 5, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, NURGLE, NURGLINGS};
    m_weapons = {&m_teeth};
}

bool Nurglings::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_teeth);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Nurglings::Create(const ParameterList &parameters)
{
    auto unit = new Nurglings();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Nurglings::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Nurglings::Create,
            NurgleBase::ValueToString,
            NurgleBase::EnumStringToInt,
            Nurglings::ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
            },
            CHAOS,
            { NURGLE }
        };
        s_registered = UnitFactory::Register("Nurglings", factoryMethod);
    }
}

int Nurglings::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Nurgle

