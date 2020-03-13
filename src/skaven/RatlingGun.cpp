/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/RatlingGun.h>
#include <UnitFactory.h>

namespace Skaven
{

bool RatlingGun::s_registered = false;

RatlingGun::RatlingGun() :
    Skaventide("Ratling Gun", 6, WOUNDS, 4, 6, false),
    m_ratlingGun(Weapon::Type::Missile, "Ratling Gun", 12, 0, 4, 4, -1, 1),
    m_rustyKnives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, RATLING_GUN};
    m_weapons = {&m_ratlingGun, &m_rustyKnives};
}

bool RatlingGun::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_ratlingGun);
    model->addMeleeWeapon(&m_rustyKnives);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *RatlingGun::Create(const ParameterList &parameters)
{
    auto unit = new RatlingGun();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void RatlingGun::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };
        s_registered = UnitFactory::Register("Ratling Gun", *factoryMethod);
    }
}

int RatlingGun::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // More-more Warplead!
    if (weapon->name() == m_ratlingGun.name())
    {
        // Decide to overcharge
        if (moreMore())
        {
            auto roll1 = Dice::rollD6();
            auto roll2 = Dice::rollD6();
            if (roll1 == roll2)
            {
                m_moreMoreFailed = true;
            }
            return (roll1+roll2) * 2;
        }

        m_moreMoreFailed = false;
        return RAND_2D6;
    }

    return Unit::extraAttacks(attackingModel, weapon, target);
}

void RatlingGun::onRestore()
{
    Unit::onRestore();

    m_moreMoreFailed = false;
}

Wounds RatlingGun::onEndShooting(PlayerId player)
{
    auto wounds = Unit::onEndCombat(player);

    if (m_moreMoreFailed)
    {
        slay(1);
        m_moreMoreFailed = false;
    }
    return wounds;
}

} // namespace Skaven
