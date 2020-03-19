/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "idonethdeepkin/EidolonStorm.h"

namespace IdonethDeepkin
{

bool EidolonOfMathlannAspectOfTheStorm::s_registered = false;

Unit *EidolonOfMathlannAspectOfTheStorm::Create(const ParameterList &parameters)
{
    auto unit = new EidolonOfMathlannAspectOfTheStorm();

    auto enclave = (Enclave)GetEnumParam("Enclave", parameters, Enclave::None);
    unit->setEnclave(enclave);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void EidolonOfMathlannAspectOfTheStorm::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            IdonethDeepkinBase::ValueToString,
            IdonethDeepkinBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Enclave", IdonethDeepkinBase::None, IdonethDeepkinBase::None, IdonethDeepkinBase::Briomdar, 1},
            },
            ORDER,
            { IDONETH_DEEPKIN }
        };

        s_registered = UnitFactory::Register("Eidolon of Mathlann Aspect of the Storm", factoryMethod);
    }
}

EidolonOfMathlannAspectOfTheStorm::EidolonOfMathlannAspectOfTheStorm() :
    IdonethDeepkinBase("Eidolon of Mathlann Aspect of the Storm", 12, WOUNDS, 10, 3, true),
    m_spear(Weapon::Type::Melee, "Fuathtar, Spear of Repressed Fury", 2, 4, 3, 2, -2, 2),
    m_crulhook(Weapon::Type::Melee, "Crulhook", 1, 4, 2, 3, -1, 1),
    m_stormshoal(Weapon::Type::Melee, "Stormshoal", 3, RAND_2D6, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, EIDOLON, HERO, ASPECT_OF_THE_STORM};
    m_weapons = {&m_spear, &m_crulhook, &m_stormshoal};
}

bool EidolonOfMathlannAspectOfTheStorm::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_spear);
    model->addMeleeWeapon(&m_crulhook);
    model->addMeleeWeapon(&m_stormshoal);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Rerolls EidolonOfMathlannAspectOfTheStorm::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Crashing Upon the Foe
    if (m_charged && (weapon->name() == m_spear.name()))
    {
        return RerollOnes;
    }

    return Unit::toHitRerolls(weapon, target);
}

Wounds EidolonOfMathlannAspectOfTheStorm::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Crashing Upon the Foe
    if (m_charged && (weapon->name() == m_spear.name()))
    {
        return {weapon->damage() + 1, 0};
    }

    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void EidolonOfMathlannAspectOfTheStorm::onCharged()
{
    Unit::onCharged();

    // Crashing Upon the Foe
    if (m_charged)
    {
        heal(Dice::rollD3());
    }
}

} // IdonethDeepkin

