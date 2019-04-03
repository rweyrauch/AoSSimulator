/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/FlamersOfTzeentch.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{
static FactoryMethod factoryMethod = {
    FlamersOfTzeentch::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", FlamersOfTzeentch::MIN_UNIT_SIZE, FlamersOfTzeentch::MIN_UNIT_SIZE,
            FlamersOfTzeentch::MAX_UNIT_SIZE, FlamersOfTzeentch::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    TZEENTCH
};

bool FlamersOfTzeentch::s_registered = false;

FlamersOfTzeentch::FlamersOfTzeentch() :
    Unit("Flamers of Tzeentch", 9, WOUNDS, 10, 5, true),
    m_warpflame(Weapon::Type::Missile, "Warpflame", 18, 3, 4, 3, 0, RAND_D3),
    m_warpflamePyrocaster(Weapon::Type::Missile, "Warpflame (Pyrocaster)",  18, 4, 4, 3, 0, RAND_D3),
    m_flamingMaw(Weapon::Type::Melee, "Flaming Maw", 1, 2, 5, 3, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, TZEENTCH, FLAMERS};
}

bool FlamersOfTzeentch::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model pyrocaster(BASESIZE, WOUNDS);
    pyrocaster.addMissileWeapon(&m_warpflamePyrocaster);
    pyrocaster.addMeleeWeapon(&m_flamingMaw);
    addModel(pyrocaster);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_warpflame);
        model.addMeleeWeapon(&m_flamingMaw);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void FlamersOfTzeentch::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_warpflame);
    visitor(&m_warpflamePyrocaster);
    visitor(&m_flamingMaw);
}

Unit *FlamersOfTzeentch::Create(const ParameterList &parameters)
{
    auto unit = new FlamersOfTzeentch();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void FlamersOfTzeentch::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Flamers Of Tzeentch", factoryMethod);
    }
}

} //namespace Tzeentch