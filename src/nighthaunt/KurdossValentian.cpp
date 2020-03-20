/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/KurdossValentian.h"

namespace Nighthaunt
{

bool KurdossValentian::s_registered = false;

Unit *KurdossValentian::Create(const ParameterList &parameters)
{
    auto unit = new KurdossValentian();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KurdossValentian::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            KurdossValentian::Create,
            nullptr,
            nullptr,
            KurdossValentian::ComputePoints,
            {
            },
            DEATH,
            { NIGHTHAUNT }
        };
        s_registered = UnitFactory::Register("Kurdoss Valentian", factoryMethod);
    }
}

KurdossValentian::KurdossValentian() :
    Nighthaunt("Kurdoss Valentian", 6, WOUNDS, 10, 4, true),
    m_sceptre(Weapon::Type::Melee, "Sepulchral Sceptre", 1, 5, 3, 3, -2, RAND_D3),
    m_claws(Weapon::Type::Melee, "Wraith Herald's Spectral Claws", 1, 6, 4, 4, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KURDOSS_VALENTIAN};
    m_weapons = {&m_sceptre, &m_claws};
}

bool KurdossValentian::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_sceptre);
    model->addMeleeWeapon(&m_claws);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Wounds KurdossValentian::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Frightful Touch
    if ((hitRoll == 6) && (weapon->name() == m_claws.name()))
    {
        return {0, 1};
    }

    // Soul-crushing Smite
    if ((woundRoll == 6) && (weapon->name() == m_sceptre.name()))
    {
        return { Dice::rollD6(), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls KurdossValentian::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if (target->isGeneral() && (weapon->name() == m_sceptre.name()))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

} // namespace Nighthaunt
