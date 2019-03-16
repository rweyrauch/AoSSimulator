/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skulltaker.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Skulltaker::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    KHORNE
};

bool Skulltaker::s_registered = false;

Skulltaker::Skulltaker() :
    Unit("Skulltaker", 5, WOUNDS, 10, 4, false),
    m_slayerSword(Weapon::Type::Melee, "The Slayer Sword", 1, 3, 3, 3, -1, 3)
{
    m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, HERO, HERALD_OF_KHORNE, SKULLTAKER};
}

bool Skulltaker::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_slayerSword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Skulltaker::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_slayerSword);
}

Unit *Skulltaker::Create(const ParameterList &parameters)
{
    auto unit = new Skulltaker();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Skulltaker::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skulltaker", factoryMethod);
    }
}

Wounds Skulltaker::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Decapitating Strike
    if (hitRoll == 6)
    {
        return {weapon->damage(), 3};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Rerolls Skulltaker::toSaveRerolls(const Weapon *weapon) const
{
    // Cloak of Skulls
    return RerollFailed;
}

Rerolls Skulltaker::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Heroes' Bane
    if (target->hasKeyword(HERO))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

Rerolls Skulltaker::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Heroes' Bane
    if (target->hasKeyword(HERO))
    {
        return RerollFailed;
    }
    return Unit::toWoundRerolls(weapon, target);
}


} // namespace Khorne