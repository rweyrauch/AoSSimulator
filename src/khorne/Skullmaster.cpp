/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skullmaster.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Skullmaster::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    KHORNE
};

bool Skullmaster::s_registered = false;

Skullmaster::Skullmaster() :
    Unit("Skullmaster", 8, WOUNDS, 10, 4, false),
    m_bladeOfBlood(Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1),
    m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, HERO, HERALD_OF_KHORNE, SKULLMASTER};
}

bool Skullmaster::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_bladeOfBlood);
    model.addMeleeWeapon(&m_brazenHooves);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Skullmaster::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bladeOfBlood);
    visitor(&m_brazenHooves);
}

Unit *Skullmaster::Create(const ParameterList &parameters)
{
    auto unit = new Skullmaster();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Skullmaster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skullmaster", factoryMethod);
    }
}

Wounds Skullmaster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Decapitating Blow
    if (hitRoll == 6)
    {
        return {weapon->damage(), 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void Skullmaster::onCharged()
{
    // Murderous Charge
    if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0f))
    {
        Dice dice;
        Dice::RollResult rolls;
        dice.rollD6(remainingModels(), rolls);
        Wounds wounds = {0, 0};
        if (remainingModels() >= 6)
        {
            for (int i = 0; i < rolls.rollsGE(2); i++)
            {
                wounds.mortal += dice.rollD3();
            }
        }
        else
        {
            wounds.mortal = rolls.rollsGE(2);
        }

        SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
               name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

        m_meleeTarget->applyDamage(wounds);
    }
    Unit::onCharged();
}

Rerolls Skullmaster::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Slaughter and Ruin
    if (m_charged)
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

} // namespace Khorne