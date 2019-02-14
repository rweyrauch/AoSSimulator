/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/PinkHorrors.h>

namespace Tzeentch
{

bool PinkHorrors::s_registered = false;

PinkHorrors::PinkHorrors() :
    Unit("Pink Horrors", 5, WOUNDS, 10, 5, false),
    m_magicalFlames(Weapon::Type::Missile, "Magical Flames", 18, 1, 4, 4, 0, 1),
    m_graspingHands(Weapon::Type::Melee, "Grasping Hands",  1, 1, 5, 4, 0, 1),
    m_graspingHandsHorror(Weapon::Type::Melee, "Grasping Hands (Iridescent Horror)", 1, 2, 5, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, TZEENTCH, WIZARD, PINK_HORRORS};
}

bool PinkHorrors::configure(int numModels, bool iconBearer, bool hornblower)
{
    return false;
}

void PinkHorrors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_magicalFlames);
    visitor(&m_graspingHands);
    visitor(&m_graspingHandsHorror);
}

Unit *PinkHorrors::Create(const ParameterList &parameters)
{
    return nullptr;
}

void PinkHorrors::Init()
{

}
} //namespace Tzeentch