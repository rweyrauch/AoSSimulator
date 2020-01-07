/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARKANAUTFRIGATE_H
#define ARKANAUTFRIGATE_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class ArkanautFrigate : public KharadronBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 200;

    static Unit *Create(const ParameterList &parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    ArkanautFrigate();

    ~ArkanautFrigate() override = default;

    bool configure();

protected:

private:

    Weapon m_cannonShrapnel,
        m_cannonShell,
        m_skyhook,
        m_carbines,
        m_boardingWeapons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Garrison                         No
// Aetheric Navigator/Endrinrigger  No
// Bomb Racks                       No
// Disengage                        No
// Fly High                         No
// Heavy Skyhook                    No
// Heavy Sky Cannon                 No
//

} // namespace KharadronOverlords

#endif //ARKANAUTFRIGATE_H
