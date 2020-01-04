/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/SplinteredFang.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    SplinteredFang::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", SplinteredFang::MIN_UNIT_SIZE, SplinteredFang::MIN_UNIT_SIZE,
            SplinteredFang::MAX_UNIT_SIZE, SplinteredFang::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool SplinteredFang::s_registered = false;

Unit *SplinteredFang::Create(const ParameterList &parameters)
{
    auto unit = new SplinteredFang();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SplinteredFang::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Splintered Fang", factoryMethod);
    }
}

SplinteredFang::SplinteredFang() :
    Unit("Splintered Fang", 6, WOUNDS, 5, 5, false),
    m_poisonedWeapons(Weapon::Type::Melee, "Poisoned Weapons", 1, 1, 4, 4, 0, 1),
    m_poisonedWeaponsLeader(Weapon::Type::Melee, "Poisoned Weapons (Trueblood)", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, SPLINTERED_FANG};
}

bool SplinteredFang::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto trueblood = new Model(BASESIZE, WOUNDS);
    trueblood->addMeleeWeapon(&m_poisonedWeaponsLeader);
    trueblood->setName("Trueblood");
    addModel(trueblood);

    auto serpentCaller = new Model(BASESIZE, WOUNDS);
    serpentCaller->addMeleeWeapon(&m_poisonedWeapons);
    serpentCaller->setName("Serpent Caller");
    addModel(serpentCaller);

    auto serpent = new Model(BASESIZE, WOUNDS+1);
    serpent->addMeleeWeapon(&m_poisonedWeapons);
    serpent->setName("Serpent");
    addModel(serpent);

    for (auto i = 3; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_poisonedWeapons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SplinteredFang::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_poisonedWeapons);
    visitor(m_poisonedWeaponsLeader);
}

Wounds SplinteredFang::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // One Cut, One Kill
    if (hitRoll == 6)
    {
        return {0, 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void SplinteredFang::onWounded()
{
    Unit::onWounded();

    // Check for Serpent Caller
    for (const auto& ip : m_models)
    {
        if (ip->slain() && (ip->getName() == "Serpent Caller"))
        {
            m_hasSerpentCaller = false;
            break;
        }
    }
}

} //SlavesToDarkness