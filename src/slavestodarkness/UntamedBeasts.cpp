/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/UntamedBeasts.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    UntamedBeasts::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool UntamedBeasts::s_registered = false;

Unit *UntamedBeasts::Create(const ParameterList &parameters)
{
    auto unit = new UntamedBeasts();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void UntamedBeasts::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Untamed Beasts", factoryMethod);
    }
}

UntamedBeasts::UntamedBeasts() :
    Unit("Untamed Beasts", 6, WOUNDS, 5, 6, false),
    m_harpoonFirstFang(Weapon::Type::Missile, "Jagged Harpoon", 8, 1, 4, 3, -1, 2),
    m_huntingWeapons(Weapon::Type::Melee, "Hunting Weapons", 1, 1, 4, 4, 0, 1),
    m_huntingWeaponsHeartEater(Weapon::Type::Melee, "Hunting Weapons (Heart-eater)", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, UNTAMED_BEASTS };
}

bool UntamedBeasts::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model hearteater(BASESIZE, WOUNDS);
    hearteater.addMeleeWeapon(&m_huntingWeaponsHeartEater);
    hearteater.setName("Heart-eater");
    addModel(hearteater);

    Model firstfang(BASESIZE, WOUNDS);
    firstfang.addMissileWeapon(&m_harpoonFirstFang);
    firstfang.addMeleeWeapon(&m_huntingWeapons);
    firstfang.setName("First Fang");
    addModel(firstfang);

    Model prowler(BASESIZE, 2);
    prowler.addMeleeWeapon(&m_huntingWeapons);
    prowler.setName("Rocktusk Prowler");
    addModel(prowler);

    for (auto i = 3; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_huntingWeapons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void UntamedBeasts::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_harpoonFirstFang);
    visitor(&m_huntingWeapons);
    visitor(&m_huntingWeaponsHeartEater);
}

} //SlavesToDarkness