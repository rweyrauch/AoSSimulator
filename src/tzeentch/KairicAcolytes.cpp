/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/KairicAcolytes.h>

namespace Tzeentch
{

bool KairicAcolytes::s_registered = false;

KairicAcolytes::KairicAcolytes() :
    Unit("Kairic Acolytes", 6, WOUNDS, 5, 6, false),
    m_sorcerousBolt(Weapon::Type::Missile, "Sorcerous Bolt", 12, 1, 5, 4, 0, 1),
    m_sorcerousBoldAdept(Weapon::Type::Missile, "Sorcerous Bolt (Adept)", 12, 2, 5, 4, 0, 1),
    m_cursedBlade(Weapon::Type::Melee, "Cursed Blade", 1, 1, 4, 4, 0, 1),
    m_cursedGlaive(Weapon::Type::Melee, "Cursed Glaive", 1, 1, 4, 4, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, KAIRIC_ACOLYTES};
}

bool KairicAcolytes::configure(int numModels, WeaponOptions weapons, int numCursedGlaives, int numScrollsOfDarkArts, int numVulcharcs)
{
    return false;
}

void KairicAcolytes::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_sorcerousBolt);
    visitor(&m_sorcerousBoldAdept);
    visitor(&m_cursedBlade);
    visitor(&m_cursedGlaive);
}

void KairicAcolytes::Init()
{

}

Unit *KairicAcolytes::Create(const ParameterList &parameters)
{
    return nullptr;
}

std::string KairicAcolytes::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int KairicAcolytes::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

} //namespace Tzeentch