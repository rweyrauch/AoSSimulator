/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightQuestor.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    KnightQuestor::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool KnightQuestor::s_registered = false;

KnightQuestor::KnightQuestor() :
    StormcastEternal("Knight-Questor", 5, WOUNDS, 8, 3, false),
    m_warblade(Weapon::Type::Melee, "Questor Warblade", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, KNIGHT_QUESTOR};
}

bool KnightQuestor::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_warblade);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *KnightQuestor::Create(const ParameterList &parameters)
{
    auto unit = new KnightQuestor();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KnightQuestor::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Knight-Questor", factoryMethod);
    }
}

Rerolls KnightQuestor::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Heroic Challenge
    if (unit->hasKeyword(HERO))
    {
        return RerollFailed;
    }
    return StormcastEternal::toHitRerolls(weapon, unit);
}

Rerolls KnightQuestor::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shield
    return RerollFailed;
}

void KnightQuestor::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_warblade);
}

Wounds KnightQuestor::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Thundercharged Strike
    if (woundRoll == 6 && weapon->name() == m_warblade.name())
    {
        return {2, 0};
    }
    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}


} // namespace StormcastEternals