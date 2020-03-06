/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Leadbelchers.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    Leadbelchers::Create,
    Leadbelchers::ValueToString,
    Leadbelchers::EnumStringToInt,
    Leadbelchers::ComputePoints,
    {
        {
            ParamType::Integer, "Models", Leadbelchers::MIN_UNIT_SIZE, Leadbelchers::MIN_UNIT_SIZE,
            Leadbelchers::MAX_UNIT_SIZE, Leadbelchers::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
    },
    DESTRUCTION,
    { OGOR_MAWTRIBES }
};

bool Leadbelchers::s_registered = false;

Unit *Leadbelchers::Create(const ParameterList &parameters)
{
    auto unit = new Leadbelchers();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Leadbelchers::ValueToString(const Parameter &parameter)
{
    return MawtribesBase::ValueToString(parameter);
}

int Leadbelchers::EnumStringToInt(const std::string &enumString)
{
    return MawtribesBase::EnumStringToInt(enumString);
}

void Leadbelchers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Leadbelchers", factoryMethod);
    }
}

Leadbelchers::Leadbelchers() :
    MawtribesBase("Leadbelchers", 6, WOUNDS, 6, 5, false),
    m_gun(Weapon::Type::Missile, "Leadbelcher Gun", 12, RAND_D3, 4, 3, -1, 1),
    m_blow(Weapon::Type::Melee, "Bludgeoning Blow", 1, 2, 3, 3, -1, 2),
    m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
    m_blowThunderfist(Weapon::Type::Melee, "Bludgeoning Blow", 1, 3, 3, 3, -1, 2)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, LEADBELCHERS};
    m_weapons = {&m_gun, &m_blow, &m_bite, &m_blowThunderfist};
}

bool Leadbelchers::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    // Gunmasters
    if (hasKeyword(UNDERGUTS))
    {
        m_gun.setRange(m_gun.range()+6.0f);
    }

    auto boss = new Model(BASESIZE, WOUNDS);
    boss->addMissileWeapon(&m_gun);
    boss->addMeleeWeapon(&m_blowThunderfist);
    boss->addMeleeWeapon(&m_bite);
    addModel(boss);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_gun);
        model->addMeleeWeapon(&m_blow);
        model->addMeleeWeapon(&m_bite);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

void Leadbelchers::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    if (m_moved)
    {
        m_gun.setAttacks(RAND_D3);
    }
    else
    {
        m_gun.setAttacks(RAND_D6);
    }
}

int Leadbelchers::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace OgorMawtribes
