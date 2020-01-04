/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Cockatrice.h>
#include <UnitFactory.h>
#include <iostream>

namespace BeastsOfChaos
{

static FactoryMethod factoryMethod = {
    Cockatrice::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    {
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool Cockatrice::s_registered = false;

Cockatrice::Cockatrice() :
    BeastsOfChaosBase("Cockatrice", 12, WOUNDS, 5, 6, true),
    m_petrifyingGaze(Weapon::Type::Missile, "Petrifying Gaze", 10, 0, 0, 0, 0, 0),
    m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 2, 2, 4, 3, -1, RAND_D3),
    m_swordlikeTalons(Weapon::Type::Melee, "Sword-like Talons", 1, 4, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, COCKATRICE};
}

bool Cockatrice::configure()
{
        auto model = new Model(BASESIZE, WOUNDS);

    // NOTE: Petrifying Gaze attack is special, do not treat it as a weapon

    model->addMeleeWeapon(&m_viciousBeak);
    model->addMeleeWeapon(&m_swordlikeTalons);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Cockatrice::Create(const ParameterList &parameters)
{
    auto unit = new Cockatrice();

    auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
    unit->setGreatfray(fray);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Cockatrice::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Cockatrice", factoryMethod);
    }
}

void Cockatrice::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_petrifyingGaze);
    visitor(m_viciousBeak);
    visitor(m_swordlikeTalons);
}

int Cockatrice::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Maddened Ferocity
    if (m_charged && (weapon->name() == m_swordlikeTalons.name()))
    {
        return 4;
    }
    return Unit::extraAttacks(attackingModel, weapon, target);
}

void Cockatrice::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    if (player == m_owningPlayer)
    {
        // Petrifying Gaze
        if (m_shootingTarget)
        {
            if (distanceTo(m_shootingTarget) <= m_petrifyingGaze.range())
            {
                Dice dice;
                if (dice.rollD6() >= 4)
                {
                    Wounds gazeDamage = {0, dice.rollD6()};
                    m_shootingTarget->applyDamage(gazeDamage);
                }
            }
        }
    }
}

} // namespace BeastsOfChaos
