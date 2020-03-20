/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "mawtribes/IcebrowHunter.h"

namespace OgorMawtribes
{
static const int BASESIZE = 50;
static const int WOUNDS = 7;
static const int POINTS_PER_UNIT = 120;

bool IcebrowHunter::s_registered = false;

Unit *IcebrowHunter::Create(const ParameterList &parameters)
{
    auto unit = new IcebrowHunter();

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

void IcebrowHunter::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            IcebrowHunter::Create,
            IcebrowHunter::ValueToString,
            IcebrowHunter::EnumStringToInt,
            IcebrowHunter::ComputePoints,
            {
                {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
            },
            DESTRUCTION,
            { OGOR_MAWTRIBES }
        };
        s_registered = UnitFactory::Register("Icebrow Hunter", factoryMethod);
    }
}

IcebrowHunter::IcebrowHunter() :
    MawtribesBase("Icebrow Hunter", 6, WOUNDS, 7, 5, false),
    m_spear(Weapon::Type::Missile, "Great Throwing Spear", 9, 1, 4, 3, -1, RAND_D3),
    m_crossbow(Weapon::Type::Missile, "Hunter's Crossbow", 12, 1, 4, 3, 0, RAND_D3),
    m_club(Weapon::Type::Melee, "Hunter's Culling Club", 1, 4, 3, 3, 0, 2),
    m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, HERO, ICEBROW_HUNTER};
    m_weapons = {&m_spear, &m_crossbow, &m_club, &m_bite};
}

bool IcebrowHunter::configure()
{
    auto model = new Model(BASESIZE, wounds());

    model->addMissileWeapon(&m_spear);
    model->addMissileWeapon(&m_crossbow);
    model->addMeleeWeapon(&m_club);
    model->addMeleeWeapon(&m_bite);

    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

std::string IcebrowHunter::ValueToString(const Parameter &parameter)
{
    return MawtribesBase::ValueToString(parameter);
}

int IcebrowHunter::EnumStringToInt(const std::string &enumString)
{
    return MawtribesBase::EnumStringToInt(enumString);
}

int IcebrowHunter::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace OgorMawtribes
