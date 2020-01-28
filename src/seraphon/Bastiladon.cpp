/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Bastiladon.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    Bastiladon::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    Bastiladon::ComputePoints,
    {
    },
    ORDER,
    { SERAPHON }
};

bool Bastiladon::s_registered = false;

Bastiladon::Bastiladon() :
    SeraphonBase("Bastiladon", 5, WOUNDS, 10, 3, false),
    m_beam(Weapon::Type::Missile, "Searing Beam", 20, RAND_2D6, 4, 3, -1, 2),
    m_javelins(Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1),
    m_tail(Weapon::Type::Melee, "Bludgeoning Tail", 2, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SKINK, MONSTER, BASTILADON};
    m_weapons = {&m_beam, &m_javelins, &m_tail};
}

bool Bastiladon::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_beam);
    model->addMissileWeapon(&m_javelins);
    model->addMeleeWeapon(&m_tail);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *Bastiladon::Create(const ParameterList &parameters)
{
    auto unit = new Bastiladon();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bastiladon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bastiladon", factoryMethod);
    }
}

} //namespace Seraphon
