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
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Fiends::MIN_UNIT_SIZE}, Fiends::MIN_UNIT_SIZE,
            Fiends::MAX_UNIT_SIZE, Fiends::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    SLAANESH
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

Rerolls Fiends::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Locus of Grace
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 12.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(DAEMON) && ip->hasKeyword(SLAANESH) && ip->hasKeyword(HERO))
        {
            return RerollOnes;
        }
    }
    return Unit::toHitRerolls(weapon, target);
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

} // namespace Slaanesh