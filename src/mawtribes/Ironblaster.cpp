/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "mawtribes/Ironblaster.h"

namespace OgorMawtribes
{

bool Ironblaster::s_registered = false;

Unit *Ironblaster::Create(const ParameterList &parameters)
{
    auto unit = new Ironblaster();

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Ironblaster::ValueToString(const Parameter &parameter)
{
    return MawtribesBase::ValueToString(parameter);
}

int Ironblaster::EnumStringToInt(const std::string &enumString)
{
    return MawtribesBase::EnumStringToInt(enumString);
}

void Ironblaster::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Ironblaster::Create,
            Ironblaster::ValueToString,
            Ironblaster::EnumStringToInt,
            Ironblaster::ComputePoints,
            {
                {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
            },
            DESTRUCTION,
            { OGOR_MAWTRIBES }
        };
        s_registered = UnitFactory::Register("Ironblaster", factoryMethod);
    }
}

Ironblaster::Ironblaster() :
    MawtribesBase("Ironblaster", 7, WOUNDS, 6, 4, false),
    m_cannonBall(Weapon::Type::Missile, "Ironblaster Cannon: Cannon Ball", 24, 1, 4, 2, -2, RAND_D6),
    m_hailShot(Weapon::Type::Missile, "Ironblaster Cannon: Hail Shot", 12, 6, 3, 3, -1, 1),
    m_clubber(Weapon::Type::Melee, "Gunner's Clubber", 1, 3, 3, 3, 0, 2),
    m_horns(Weapon::Type::Melee, "Rhinox's Sharp Horns", 1, 2, 4, 3, -1, RAND_D3),
    m_blade(Weapon::Type::Melee, "Scrapper's Jagged Blade", 1, 2, 5, 5, 0, 1)
{
    m_keywords = {DESTRUCTION, OGOR, RHINOX, OGOR_MAWTRIBES, GUTBUSTERS, IRONBLASTER};
    m_weapons = {&m_cannonBall, &m_hailShot, &m_clubber, &m_horns, &m_blade};
}

bool Ironblaster::configure()
{
    auto model = new Model(BASESIZE, wounds());

    m_hailShot.activate(false);

    model->addMissileWeapon(&m_cannonBall);
    model->addMissileWeapon(&m_hailShot);
    model->addMeleeWeapon(&m_clubber);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_blade);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds Ironblaster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Rhinox Charge
    if (m_charged && (weapon->name() == m_horns.name()))
    {
        return {weapon->damage()+1, 0};
    }

    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void Ironblaster::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    auto board = Board::Instance();
    PlayerId otherPlayer = PlayerId::Red;
    if (player == PlayerId::Red)
    {
        otherPlayer = PlayerId::Blue;
    }
    auto otherRoster = board->getPlayerRoster(otherPlayer);

    auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
    if (nearestUnit)
    {
        float rangeTo = distanceTo(nearestUnit);
        if (rangeTo < (float)m_hailShot.range())
        {
            m_hailShot.activate(true);
            m_cannonBall.activate(false);
        }
        else
        {
            m_hailShot.activate(false);
            m_cannonBall.activate(true);
        }
    }
}

} // namespace OgorMawtribes
