/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Gnoblars.h"

namespace OgorMawtribes
{

bool Gnoblars::s_registered = false;

Unit *Gnoblars::Create(const ParameterList &parameters)
{
    auto unit = new Gnoblars();

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

void Gnoblars::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Gnoblars::Create,
            MawtribesBase::ValueToString,
            MawtribesBase::EnumStringToInt,
            Gnoblars::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", Gnoblars::MIN_UNIT_SIZE, Gnoblars::MIN_UNIT_SIZE,
                    Gnoblars::MAX_UNIT_SIZE, Gnoblars::MIN_UNIT_SIZE
                },
                {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
            },
            DESTRUCTION,
            { OGOR_MAWTRIBES }
        };
        s_registered = UnitFactory::Register("Gnoblars", factoryMethod);
    }
}

Gnoblars::Gnoblars() :
    MawtribesBase("Gnoblars", 5, WOUNDS, 4, 6, false),
    m_sharpStuff(Weapon::Type::Missile, "Sharp Stuff", 8, 1, 4, 5, 0, 1),
    m_motleyWeapons(Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 1, 5, 5, 0, 1),
    m_motleyWeaponsBiter(Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 2, 5, 5, 0, 2)
{
    m_keywords = {DESTRUCTION, GROT, OGOR_MAWTRIBES, GUTBUSTERS, GNOBLARS};
    m_weapons = {&m_sharpStuff, &m_motleyWeapons, &m_motleyWeaponsBiter};
}

bool Gnoblars::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto biter = new Model(BASESIZE, wounds());
    biter->addMissileWeapon(&m_sharpStuff);
    biter->addMeleeWeapon(&m_motleyWeaponsBiter);
    addModel(biter);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_sharpStuff);
        model->addMeleeWeapon(&m_motleyWeapons);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int Gnoblars::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int Gnoblars::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Screeching Horde
    auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
    if (remainingModels() >= 10) attacks++;
    return attacks;
}

} // namespace OgorMawtribes
