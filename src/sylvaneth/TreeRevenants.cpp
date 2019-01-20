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

namespace Sylvaneth {

static FactoryMethod factoryMethod = {
    TreeRevenants::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = TreeRevenants::MIN_UNIT_SIZE}, TreeRevenants::MIN_UNIT_SIZE,
         TreeRevenants::MAX_UNIT_SIZE},
        {ParamType::Boolean, "scionGlaive", {.m_boolValue = false}, false, false},
        {ParamType::Boolean, "gladeBanners", {.m_boolValue = false}, false, false},
        {ParamType::Boolean, "waypipes", {.m_boolValue = false}, false, false},
    }
};

bool TreeRevenants::s_registered = false;

Weapon TreeRevenants::s_enchantedBlade(Weapon::Type::Melee, "Enchanted Blade", 1, 2, 4, 3, -1, 1);
Weapon TreeRevenants::s_enchantedBladeScion(Weapon::Type::Melee, "Enchanted Blade", 1, 4, 4, 3, -1, 1);
Weapon TreeRevenants::s_protectorGlaive(Weapon::Type::Melee, "Protector Glaive", 1, 2, 4, 3, -1, 2);

TreeRevenants::TreeRevenants() :
    Unit("Tree Revenants", 5, WOUNDS, 6, 5, false)
{
    m_keywords = {ORDER, SYLVANETH, TREE_REVENANTS };
}

bool TreeRevenants::configure(int numModels, bool scionGlaive, bool gladeBanners, bool waypipes)
{
    return false;
}

Unit *TreeRevenants::Create(const ParameterList &parameters)
{
    return nullptr;
}

void TreeRevenants::Init()
{

}

} // namespace Sylvaneth