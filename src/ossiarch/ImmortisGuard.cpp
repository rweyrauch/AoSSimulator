/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/ImmortisGuard.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    ImmortisGuard::Create,
    ImmortisGuard::ValueToString,
    ImmortisGuard::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", ImmortisGuard::MIN_UNIT_SIZE, ImmortisGuard::MIN_UNIT_SIZE,
            ImmortisGuard::MAX_UNIT_SIZE, ImmortisGuard::MIN_UNIT_SIZE
        },
    },
    DEATH,
    { OSSIARCH_BONEREAPERS }
};

bool ImmortisGuard::s_registered = false;

Unit *ImmortisGuard::Create(const ParameterList &parameters)
{
    auto unit = new ImmortisGuard();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string ImmortisGuard::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int ImmortisGuard::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void ImmortisGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Immortis Guard", factoryMethod);
    }
}

ImmortisGuard::ImmortisGuard() :
    OssiarchBonereaperBase("Immortis Guard", 5, WOUNDS, 10, 3, false),
    m_halberd(Weapon::Type::Melee, "Dread Halberd", 2, 2, 3, 3, -2, 2),
    m_battleShield(Weapon::Type::Melee, "Battle Shield", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, HEKATOS, IMMORTIS_GUARD};
    m_weapons = {&m_halberd, &m_battleShield};
}

bool ImmortisGuard::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_halberd);
        model->addMeleeWeapon(&m_battleShield);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Wounds ImmortisGuard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Deadly Combination
    if ((hitRoll == 6) && (weapon->name() == m_battleShield.name()))
    {
        return { weapon->damage(), 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace OssiarchBonereapers
