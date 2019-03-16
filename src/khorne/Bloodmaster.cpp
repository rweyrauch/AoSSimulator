/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodmaster.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Bloodmaster::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    KHORNE
};

bool Bloodmaster::s_registered = false;

Bloodmaster::Bloodmaster() :
    Unit("Bloodmaster", 5, WOUNDS, 10, 4, false),
    m_bladeOfBlood(Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, HERO, HERALD_OF_KHORNE, BLOODMASTER};
}

bool Bloodmaster::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_bladeOfBlood);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Bloodmaster::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bladeOfBlood);
}

Unit *Bloodmaster::Create(const ParameterList &parameters)
{
    auto unit = new Bloodmaster();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bloodmaster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodmaster", factoryMethod);
    }
}

Wounds Bloodmaster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Decapitating Blow
    if (hitRoll == 6)
    {
        return {weapon->damage(), 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}


} // namespace Khorne