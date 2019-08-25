/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/CorvusCabal.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    CorvusCabal::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", CorvusCabal::MIN_UNIT_SIZE, CorvusCabal::MIN_UNIT_SIZE,
            CorvusCabal::MAX_UNIT_SIZE, CorvusCabal::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool CorvusCabal::s_registered = false;

Unit *CorvusCabal::Create(const ParameterList &parameters)
{
    auto unit = new CorvusCabal();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CorvusCabal::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Corvus Cabal", factoryMethod);
    }
}

CorvusCabal::CorvusCabal() :
    Unit("Corvus Cabal", 8, WOUNDS, 5, 6, true),
    m_ravenDarts(Weapon::Type::Missile, "Raven Darts", 8, 1, 4, 5, 0, 1),
    m_corvusWeapons(Weapon::Type::Melee, "Corvus Weapons", 1, 1, 4, 4, 0, 1),
    m_corvusWeaponsLeader(Weapon::Type::Melee, "Corvus Weapons (Shadow Piercer)", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, CORVUS_CABAL};
}

bool CorvusCabal::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model piercer(BASESIZE, WOUNDS);
    piercer.addMissileWeapon(&m_ravenDarts);
    piercer.addMeleeWeapon(&m_corvusWeaponsLeader);
    piercer.setName("Shadow Piercer");
    addModel(piercer);

    Model talon(BASESIZE, WOUNDS);
    talon.addMissileWeapon(&m_ravenDarts);
    talon.addMeleeWeapon(&m_corvusWeapons);
    talon.setName("Shrike Talon");
    addModel(talon);

    for (auto i = 2; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_ravenDarts);
        model.addMeleeWeapon(&m_corvusWeapons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void CorvusCabal::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_ravenDarts);
    visitor(&m_corvusWeapons);
    visitor(&m_corvusWeaponsLeader);
}

Rerolls CorvusCabal::chargeRerolls() const
{
    if (m_hasShrikeTalon)
    {
        return RerollOnes;
    }
    return Unit::chargeRerolls();
}

void CorvusCabal::onWounded()
{
    Unit::onWounded();

    // Check for Shrike Talon
    for (const auto& ip : m_models)
    {
        if (ip.slain() && (ip.getName() == "Shrike Talon"))
        {
            m_hasShrikeTalon = false;
            break;
        }
    }
}

} //SlavesToDarkness