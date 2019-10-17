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
#include <Board.h>

namespace Slaanesh
{

static FactoryMethod factoryMethod = {
    Fiends::Create,
    SlaaneshBase::ValueToString,
    SlaaneshBase::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Fiends::MIN_UNIT_SIZE, Fiends::MIN_UNIT_SIZE,
            Fiends::MAX_UNIT_SIZE, Fiends::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders, SlaaneshBase::Godseekers, 1},
    },
    CHAOS,
    SLAANESH
};

bool Fiends::s_registered = false;

Fiends::Fiends() :
    SlaaneshBase("Fiends", 12, WOUNDS, 10, 5, false),
    m_deadlyPincers(Weapon::Type::Melee, "Deadly Pincers", 1, 4, 3, 3, -1, 1),
    m_deadlyPincersBlissbringer(Weapon::Type::Melee, "Deadly Pincers", 1, 5, 3, 3, -1, 1),
    m_barbedStinger(Weapon::Type::Melee, "Barbed Stinger", 2, 1, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, SLAANESH, HEDONITE, FIENDS};
}

bool Fiends::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model blissbringer(BASESIZE, WOUNDS);
    blissbringer.addMeleeWeapon(&m_deadlyPincersBlissbringer);
    blissbringer.addMeleeWeapon(&m_barbedStinger);
    addModel(blissbringer);

    for (auto i = 1; i < numModels; i++)
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

void Fiends::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_deadlyPincers);
    visitor(m_deadlyPincersBlissbringer);
    visitor(m_barbedStinger);
}

Unit *Fiends::Create(const ParameterList &parameters)
{
    auto unit = new Fiends();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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

int Fiends::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = Unit::targetHitModifier(weapon, attacker);

    // Soporific Musk
    if (!weapon->isMissile())
    {
        modifier -= 1;
    }
    return modifier;
}

int Fiends::targetWoundModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = Unit::targetWoundModifier(weapon, attacker);

    // Soporific Musk
    if (!weapon->isMissile() && remainingModels() >= 4)
    {
        modifier -= 1;
    }
    return modifier;
}

Wounds Fiends::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Crushing Grip
    if ((woundRoll == 6) && (weapon->name() == m_deadlyPincers.name()))
    {
        return {RAND_D3, 0};
    }
    // Deadly Venom
    if (weapon->name() == m_barbedStinger.name())
    {
        if (target->wounds() == 1)
            return {1, 0};
        else if (target->wounds() <= 3)
            return {RAND_D3, 0};
        else
            return {RAND_D6, 0};
    }

    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Slaanesh