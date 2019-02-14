/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/Tzaangors.h>

namespace Tzeentch
{

bool Tzaangors::s_registered = false;

Tzaangors::Tzaangors() :
    Unit("Tzaangors", 6, WOUNDS, 5, 5, false),
    m_savageBlade(Weapon::Type::Melee, "Savage Blade", 1, 2, 4, 4, 0, 1),
    m_savageBladeTwistbray(Weapon::Type::Melee, "Savage Blade (Twistbray)", 1, 2, 3, 4, 0, 1),
    m_savageGreatblade(Weapon::Type::Melee, "Savage Greatblade", 1, 1, 4, 4, -1, 2),
    m_savageGreatbladeTwistbray(Weapon::Type::Melee, "Savage Greatblade (Twistbray)", 1, 1, 3, 4, -1, 2),
    m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
    m_viciousBeakTwistbray(Weapon::Type::Melee, "Vicious Beak (Twistbray)", 1, 1, 3, 5, 0, 1)
{
    m_keywords = {CHAOS, GOR, TZEENTCH, ARCANITE, TZAANGORS};
}

bool Tzaangors::configure(int numModels, WeaponOptions weapons, int numGreatblades, int numMutants, bool iconBearer, bool brayhorns)
{
    return false;
}

void Tzaangors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_savageBlade);
    visitor(&m_savageBladeTwistbray);
    visitor(&m_savageGreatblade);
    visitor(&m_savageGreatbladeTwistbray);
    visitor(&m_viciousBeak);
    visitor(&m_viciousBeakTwistbray);
}

Unit *Tzaangors::Create(const ParameterList &parameters)
{
    return nullptr;
}

void Tzaangors::Init()
{

}

std::string Tzaangors::ValueToString(const Parameter &parameter)
{
    return std::__cxx11::string();
}

int Tzaangors::EnumStringToInt(const std::string &enumString)
{
    return 0;
}
} //namespace Tzeentch