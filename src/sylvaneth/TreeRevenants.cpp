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
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = TreeRevenants::MIN_UNIT_SIZE}, TreeRevenants::MIN_UNIT_SIZE,
            TreeRevenants::MAX_UNIT_SIZE, TreeRevenants::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Scion Glaive", {.m_boolValue = false}, false, false},
        {ParamType::Boolean, "Glade Banners", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "Waypipes", {.m_boolValue = true}, false, false},
    },
    ORDER,
    SYLVANETH
};

bool TreeRevenants::s_registered = false;

TreeRevenants::TreeRevenants() :
    Unit("Tree Revenants", 5, WOUNDS, 6, 5, false),
    m_enchantedBlade(Weapon::Type::Melee, "Enchanted Blade", 1, 2, 4, 3, -1, 1),
    m_enchantedBladeScion(Weapon::Type::Melee, "Enchanted Blade (Scion)", 1, 4, 4, 3, -1, 1),
    m_protectorGlaive(Weapon::Type::Melee, "Protector Glaive", 1, 2, 4, 3, -1, 2)
{
    m_keywords = {ORDER, SYLVANETH, TREE_REVENANTS};
}

bool TreeRevenants::configure(int numModels, bool scionGlaive, bool gladeBanners, bool waypipes)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_gladeBanners = gladeBanners;
    m_waypipes = waypipes;

    Model scion(BASESIZE, WOUNDS);
    if (scionGlaive)
    {
        scion.addMeleeWeapon(&m_protectorGlaive);
    }
    else
    {
        scion.addMeleeWeapon(&m_enchantedBladeScion);
    }
    addModel(scion);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_enchantedBlade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
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

void TreeRevenants::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_enchantedBlade);
    visitor(&m_enchantedBladeScion);
    visitor(&m_protectorGlaive);
}

} // namespace Sylvaneth