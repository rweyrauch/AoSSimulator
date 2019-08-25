/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/TheUnmade.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    TheUnmade::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", TheUnmade::MIN_UNIT_SIZE, TheUnmade::MIN_UNIT_SIZE,
            TheUnmade::MAX_UNIT_SIZE, TheUnmade::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool TheUnmade::s_registered = false;

Unit *TheUnmade::Create(const ParameterList &parameters)
{
    auto unit = new TheUnmade();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TheUnmade::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("The Unmade", factoryMethod);
    }
}

TheUnmade::TheUnmade() :
    Unit("The Unmade", 6, WOUNDS, 5, 6, false),
    m_maimingWeapons(Weapon::Type::Melee, "Maiming Weapons", 1, 1, 4, 4, 0, 1),
    m_maimingWeaponsLeader(Weapon::Type::Melee, "Maiming Weapons (Joyous One)", 1, 2, 4, 4, 0, 1),
    m_nigthmareSickles(Weapon::Type::Melee, "Nightmare Sickles", 1, 3, 4, 3, -1, 2)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, THE_UNMADE};
}

bool TheUnmade::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model joyousOne(BASESIZE, WOUNDS);
    joyousOne.addMeleeWeapon(&m_maimingWeaponsLeader);
    joyousOne.setName("Joyous One");
    addModel(joyousOne);

    Model blissfulOne(BASESIZE, WOUNDS);
    blissfulOne.addMeleeWeapon(&m_nigthmareSickles);
    blissfulOne.setName("Blissful One");
    addModel(blissfulOne);

    for (auto i = 2; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_maimingWeapons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void TheUnmade::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_maimingWeapons);
    visitor(&m_maimingWeaponsLeader);
    visitor(&m_nigthmareSickles);
}

} //SlavesToDarkness