/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosChosen.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    ChaosChosen::Create,
    SlavesToDarknessBase::ValueToString,
    SlavesToDarknessBase::EnumStringToInt,
    ChaosChosen::ComputePoints,
    {
        {
            ParamType::Integer, "Models", ChaosChosen::MIN_UNIT_SIZE, ChaosChosen::MIN_UNIT_SIZE,
            ChaosChosen::MAX_UNIT_SIZE, ChaosChosen::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Drummer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
        {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
    },
    CHAOS,
    { SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE }
};

bool ChaosChosen::s_registered = false;

Unit *ChaosChosen::Create(const ParameterList &parameters)
{
    auto unit = new ChaosChosen();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool drummer = GetBoolParam("Drummer", parameters, false);

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    auto mark = (MarkOfChaos)GetEnumParam("Mark of Chaos", parameters, Undivided);
    unit->setMarkOfChaos(mark);

    bool ok = unit->configure(numModels, iconBearer, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChaosChosen::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chaos Chosen", factoryMethod);
    }
}

ChaosChosen::ChaosChosen() :
    SlavesToDarknessBase("Chaos Chosen", 6, WOUNDS, 7, 4, false),
    m_greataxe(Weapon::Type::Melee, "Soul Splitter", 1, 3, 3, 3, -1, 1),
    m_greataxeChampion(Weapon::Type::Melee, "Soul Splitter", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_CHOSEN};
    m_weapons = {&m_greataxe, &m_greataxeChampion};
}

bool ChaosChosen::configure(int numModels, bool iconBearer, bool drummer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_drummer = drummer;

    auto champion = new Model(BASESIZE, WOUNDS);
    champion->addMeleeWeapon(&m_greataxeChampion);
    champion->setName("Exalted Champion");
    addModel(champion);

    if (m_iconBearer)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Icon Bearer");
        model->addMeleeWeapon(&m_greataxe);
        addModel(model);
    }

    if (m_drummer)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->setName("Drummer");
        model->addMeleeWeapon(&m_greataxe);
        addModel(model);
    }

    for (auto i = (int)m_models.size(); i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_greataxe);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

void ChaosChosen::onWounded()
{
    Unit::onWounded();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->slain() && (ip->getName() == "Drummer"))
        {
            m_drummer = false;
        }
        if (ip->slain() && (ip->getName() == "Icon Bearer"))
        {
            m_iconBearer = false;
        }
    }
}

int ChaosChosen::runModifier() const
{
    auto modifier = Unit::runModifier();
    if (m_drummer) modifier += 1;
    return modifier;
}

int ChaosChosen::chargeModifier() const
{
    auto modifier = Unit::chargeModifier();
    if (m_drummer) modifier += 1;
    return modifier;
}

void ChaosChosen::onRestore()
{
    Unit::onRestore();

    // Check for special models
    for (const auto& ip : m_models)
    {
        if (ip->getName() == "Drummer")
        {
            m_drummer = true;
        }
        if (ip->getName() == "Icon Bearer")
        {
            m_iconBearer = true;
        }
    }
}

Wounds ChaosChosen::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Soul splitter
    if ((hitRoll >= 6) && (weapon->name() == m_greataxe.name()))
    {
        return { weapon->damage(), 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int ChaosChosen::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //SlavesToDarkness