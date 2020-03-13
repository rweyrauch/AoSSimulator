/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/ArchWarlock.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Skaven
{
bool ArchWarlock::s_registered = false;

Unit *ArchWarlock::Create(const ParameterList &parameters)
{
    auto unit = new ArchWarlock();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ArchWarlock::Init()
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

        s_registered = UnitFactory::Register("Arch-Warlock", *factoryMethod);
    }
}

ArchWarlock::ArchWarlock() :
    Skaventide("Arch-Warlock", 6, WOUNDS, 6, 3, false),
    m_halberd(Weapon::Type::Melee, "Stormcage Halberd", 2, 1, 3, 3, -2, RAND_D3),
    m_claw(Weapon::Type::Melee, "Piston Claw", 1, 1, 4, 3, -2, 3)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, HERO, WIZARD, WARLOCK_ENGINEER, ARCH_WARLOCK};
    m_weapons = {&m_halberd, &m_claw};

    m_totalSpells = 2;
    m_totalUnbinds = 1;
}

bool ArchWarlock::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_halberd);
    model->addMeleeWeapon(&m_claw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds ArchWarlock::onEndCombat(PlayerId player)
{
    auto wounds = Unit::onEndCombat(player);

    if (m_moreMoreFailed)
    {
        Dice dice;
        Wounds overloadWounds = {0, dice.rollD6()};
        applyDamage(overloadWounds);
        wounds += overloadWounds;
        m_moreMoreFailed = false;
    }
    return wounds;
}

Wounds ArchWarlock::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // More-more Stormcage!
    if (weapon->name() == m_halberd.name())
    {
        // Decide to overcharge (without using the hitRoll)
        if (moreMore())
        {
            if (hitRoll == 1)
            {
                m_moreMoreFailed = true;
                return {0, 0};
            }

            m_moreMoreFailed = false;
            return {RAND_D6, 0};
        }
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void ArchWarlock::onRestore()
{
    Unit::onRestore();

    m_moreMoreFailed = false;
    m_usedGauntlet = false;
}

void ArchWarlock::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    // Warpfire Gauntlet
    if ((owningPlayer() == player) && !m_usedGauntlet)
    {
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) < 8.0f)
        {
            Dice dice;
            if (dice.rollD6() >= 2) unit->applyDamage({0, dice.rollD3()});
            m_usedGauntlet = true;
        }
    }
}

} //namespace Skaven
