/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Deathmaster.h>
#include <UnitFactory.h>

namespace Skaven
{
bool Deathmaster::s_registered = false;

Unit *Deathmaster::Create(const ParameterList &parameters)
{
    auto unit = new Deathmaster();
    WeaponOption option = WeepingBlades;

    bool ok = unit->configure(option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Deathmaster::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Deathmaster", *factoryMethod);
    }
}

Deathmaster::Deathmaster() :
    Skaventide("Deathmaster", 7, WOUNDS, 5, 4, false),
    m_stars(Weapon::Type::Missile, "Eshin Throwing Stars", 12, 4, 4, 5, 0, 1),
    m_blades(Weapon::Type::Melee, "Weeping Blades", 1, 3, 3, 3, -1, RAND_D3),
    m_claws(Weapon::Type::Melee, "Fighting Claws", 1, 7, 3, 3, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_ESHIN, HERO, DEATHMASTER};
    m_weapons = {&m_stars, &m_blades, &m_claws};
}

bool Deathmaster::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_stars);
    if (option == WeepingBlades)
        model->addMeleeWeapon(&m_blades);
    else if (option == FightingClaws)
        model->addMeleeWeapon(&m_claws);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

std::string Deathmaster::ValueToString(const Parameter &parameter)
{
    return Skaventide::ValueToString(parameter);
}

int Deathmaster::EnumStringToInt(const std::string &enumString)
{
    return Skaventide::EnumStringToInt(enumString);
}

} //namespace Skaven
