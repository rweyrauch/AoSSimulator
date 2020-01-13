/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/CypherLords.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    CypherLords::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", CypherLords::MIN_UNIT_SIZE, CypherLords::MIN_UNIT_SIZE,
            CypherLords::MAX_UNIT_SIZE, CypherLords::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    { SLAVES_TO_DARKNESS }
};

bool CypherLords::s_registered = false;

Unit *CypherLords::Create(const ParameterList &parameters)
{
    auto unit = new CypherLords();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CypherLords::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Cypher Lords", factoryMethod);
    }
}

CypherLords::CypherLords() :
    Unit("Cypher Lords", 6, WOUNDS, 5, 6, false),
    m_throwingStars(Weapon::Type::Missile, "Throwing Stars and Chakrams", 8, 1, 4, 5, 0, 1),
    m_exoticBlades(Weapon::Type::Melee, "Exotic Blades", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, CYPHER_LORDS};
    m_weapons = {&m_throwingStars, &m_exoticBlades};
}

bool CypherLords::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto thrallmaster = new Model(BASESIZE, WOUNDS);
    thrallmaster->addMissileWeapon(&m_throwingStars);
    thrallmaster->addMeleeWeapon(&m_exoticBlades);
    thrallmaster->setName("Thrallmaster");
    addModel(thrallmaster);

    auto luminate = new Model(BASESIZE, WOUNDS);
    luminate->addMissileWeapon(&m_throwingStars);
    luminate->addMeleeWeapon(&m_exoticBlades);
    luminate->setName("Luminate");
    addModel(luminate);

    for (auto i = 2; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_throwingStars);
        model->addMeleeWeapon(&m_exoticBlades);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

int CypherLords::chargeModifier() const
{
    int modifier = Unit::chargeModifier();
    if (m_hasLuminate)
    {
        modifier += 1;
    }
    return modifier;
}

void CypherLords::onWounded()
{
    Unit::onWounded();

    // Check for Thrallmaster and Luminate
    for (const auto& ip : m_models)
    {
        if (ip->slain() && (ip->getName() == "Thrallmaster"))
        {
            m_hasThrallmaster = false;
        }
        if (ip->slain() && (ip->getName() == "Luminate"))
        {
            m_hasLuminate = false;
        }
    }
}

} //SlavesToDarkness