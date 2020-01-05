/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/TreeRevenants.h>
#include <UnitFactory.h>
#include <iostream>

namespace Sylvaneth
{

static FactoryMethod factoryMethod = {
    TreeRevenants::Create,
    SylvanethBase::ValueToString,
    SylvanethBase::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", TreeRevenants::MIN_UNIT_SIZE, TreeRevenants::MIN_UNIT_SIZE,
            TreeRevenants::MAX_UNIT_SIZE, TreeRevenants::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Scion Glaive", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Glade Banners", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Waypipes", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None, SylvanethBase::Harvestboon, 1},
    },
    ORDER,
    SYLVANETH
};

bool TreeRevenants::s_registered = false;

TreeRevenants::TreeRevenants() :
    SylvanethBase("Tree Revenants", 5, WOUNDS, 6, 5, false),
    m_enchantedBlade(Weapon::Type::Melee, "Enchanted Blade", 1, 2, 4, 3, -1, 1),
    m_enchantedBladeScion(Weapon::Type::Melee, "Enchanted Blade", 1, 4, 4, 3, -1, 1),
    m_protectorGlaive(Weapon::Type::Melee, "Protector Glaive", 1, 2, 4, 3, -1, 2)
{
    m_keywords = {ORDER, SYLVANETH, TREE_REVENANTS};
    m_weapons = {&m_enchantedBlade, &m_enchantedBladeScion, &m_protectorGlaive};
}

bool TreeRevenants::configure(int numModels, bool scionGlaive, bool gladeBanners, bool waypipes)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_gladeBanners = gladeBanners;
    m_waypipes = waypipes;

    auto scion = new Model(BASESIZE, WOUNDS);
    if (scionGlaive)
    {
        scion->addMeleeWeapon(&m_protectorGlaive);
    }
    else
    {
        scion->addMeleeWeapon(&m_enchantedBladeScion);
    }
    addModel(scion);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_enchantedBlade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    if (m_gladeBanners)
    {
        m_pileInMove = 6.0f;
    }

    return true;
}

Unit *TreeRevenants::Create(const ParameterList &parameters)
{
    auto unit = new TreeRevenants();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool scionGlaive = GetBoolParam("Scion Glaive", parameters, false);
    bool gladeBanners = GetBoolParam("Glade Banners", parameters, false);
    bool waypipes = GetBoolParam("Waypipes", parameters, false);

    auto glade = (Glade)GetEnumParam("Glade", parameters, SylvanethBase::None);
    unit->setGlade(glade);

    bool ok = unit->configure(numModels, scionGlaive, gladeBanners, waypipes);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TreeRevenants::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tree-Revenants", factoryMethod);
    }
}

void TreeRevenants::onBeginTurn(int battleRound)
{
    m_combatRerollAvailable = true;
    m_moveRerollAvailable = true;
    m_missileRerollAvailable = true;
    m_battleshockRerollAvailable = true;

    Unit::onBeginTurn(battleRound);
}

Rerolls TreeRevenants::runRerolls() const
{
    if (m_moveRerollAvailable)
    {
        m_moveRerollAvailable = false;
        return RerollFailed;
    }
    return Unit::runRerolls();
}

Rerolls TreeRevenants::chargeRerolls() const
{
    if (m_moveRerollAvailable)
    {
        m_moveRerollAvailable = false;
        return RerollFailed;
    }
    return Unit::chargeRerolls();
}

Rerolls TreeRevenants::battleshockRerolls() const
{
    return Unit::battleshockRerolls();
}

Rerolls TreeRevenants::toSaveRerolls(const Weapon *weapon) const
{
    return Unit::toSaveRerolls(weapon);
}

Rerolls TreeRevenants::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    return Unit::toHitRerolls(weapon, target);
}

Rerolls TreeRevenants::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    return Unit::toWoundRerolls(weapon, target);
}

} // namespace Sylvaneth