/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/VandusHammerhand.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    VandusHammerhand::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool VandusHammerhand::s_registered = false;

VandusHammerhand::VandusHammerhand() :
    StormcastEternal("Vandus Hammerhand", 10, WOUNDS, 9, 3, false),
    m_heldensen(Weapon::Type::Melee, "Heldensen", 2, 3, 3, 2, -1, 3),
    m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, HAMMERS_OF_SIGMAR, LORD_CELESTANT, VANDUS_HAMMERHAND};
}

bool VandusHammerhand::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_heldensen);
    model.addMeleeWeapon(&m_clawsAndFangs);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *VandusHammerhand::Create(const ParameterList &parameters)
{
    auto unit = new VandusHammerhand();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VandusHammerhand::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Vandus Hammerhand", factoryMethod);
    }
}

int VandusHammerhand::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = StormcastEternal::extraAttacks(nullptr, weapon, target);

    // Heldensen
    if (m_charged && weapon->name() == m_heldensen.name())
    {
        Dice dice;
        attacks += dice.rollD3();
    }
    return attacks;
}

void VandusHammerhand::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_heldensen);
    visitor(&m_clawsAndFangs);
}

Wounds VandusHammerhand::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Intolerable Damage
    if ((woundRoll == 6) && (weapon->name() == m_clawsAndFangs.name()))
    {
        Dice dice;
        // D6 instead of 1
        return { dice.rollD6(), 0 };
    }
    return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace StormcastEternals