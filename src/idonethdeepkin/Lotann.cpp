/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "idonethdeepkin/Lotann.h"

namespace IdonethDeepkin
{
static FactoryMethod factoryMethod = {
    Lotann::Create,
    nullptr,
    nullptr,
    Lotann::ComputePoints,
    {
    },
    ORDER,
    { IDONETH_DEEPKIN }
};

bool Lotann::s_registered = false;

Unit *Lotann::Create(const ParameterList &parameters)
{
    auto unit = new Lotann();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Lotann::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lotann", factoryMethod);
    }
}

Lotann::Lotann() :
    Unit("Lotann", 6, WOUNDS, 7, 6, false),
    m_quill(Weapon::Type::Melee, "Bone Quill", 1, 1, 3, 5, 0, 1),
    m_cudgel(Weapon::Type::Melee, "Ochtar's Cudgel", 3, 1, 4, 3, -1, 2),
    m_blade(Weapon::Type::Melee, "Ochtar's Blade", 3, 1, 3, 4, -1, 1),
    m_tentacles(Weapon::Type::Melee, "Ochtar's Tentacles", 3, 6, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, SOUL_WARDEN, HERO, LOTANN};
    m_weapons = {&m_quill, &m_cudgel, &m_blade, &m_tentacles};
}

bool Lotann::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_quill);
    model->addMeleeWeapon(&m_cudgel);
    model->addMeleeWeapon(&m_blade);
    model->addMeleeWeapon(&m_tentacles);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds Lotann::applyWoundSave(const Wounds &wounds)
{
    Dice dice;
    Dice::RollResult normalSaves, mortalSaves;
    dice.rollD6(wounds.normal, normalSaves);
    dice.rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= normalSaves.rollsGE(5);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(5);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

} //IdonethDeepkin