/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Fiends.h>
#include <Weapon.h>
#include <UnitFactory.h>

namespace Slaanesh
{

static FactoryMethod factoryMethod = {
    Fiends::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Fiends::MIN_UNIT_SIZE}, Fiends::MIN_UNIT_SIZE,
            Fiends::MAX_UNIT_SIZE, Fiends::MIN_UNIT_SIZE
        },
    }
};

bool Fiends::s_registered = false;

Fiends::Fiends() :
    Unit("Fiends", 12, WOUNDS, 10, 5, false),
    m_deadlyPincers(Weapon::Type::Melee, "Deadly Pincers", 1, 4, 3, 3, -1, 1),
    m_barbedStinger(Weapon::Type::Melee, "Barbed Stinger", 2, 1, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, SLAANESH, FIENDS};
}

bool Fiends::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    // TODO: Blissbringer can re-roll 1's for attacks with Pincers.
    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_deadlyPincers);
        model.addMeleeWeapon(&m_barbedStinger);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Fiends::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_deadlyPincers);
    visitor(&m_barbedStinger);
}

Unit *Fiends::Create(const ParameterList &parameters)
{
    auto unit = new Fiends();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Fiends::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Fiends", factoryMethod);
    }
}

} // namespace Slaanesh