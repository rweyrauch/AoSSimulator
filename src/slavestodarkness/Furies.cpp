/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Furies.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    Furies::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Furies::MIN_UNIT_SIZE, Furies::MIN_UNIT_SIZE,
            Furies::MAX_UNIT_SIZE, Furies::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool Furies::s_registered = false;

Unit *Furies::Create(const ParameterList &parameters)
{
    auto unit = new Furies();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Furies::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Furies", factoryMethod);
    }
}

bool Furies::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
            auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_daggerAndClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Furies::Furies() :
    Unit("Furies", 12, WOUNDS, 10, NoSave, true),
    m_daggerAndClaws(Weapon::Type::Melee, "Razor-sharp Dagger and Claws", 1, 2, 4, 3, -1, 1)
{
    m_keywords = { CHAOS, DAEMON, SLAVES_TO_DARKNESS, FURIES };
}

void Furies::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_daggerAndClaws);
}

} //SlavesToDarkness