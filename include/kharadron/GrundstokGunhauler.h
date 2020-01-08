/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GRUNDSTOKGUNHAULER_H
#define GRUNDSTOKGUNHAULER_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class GrundstokGunhauler : public KharadronBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 0;
    static const int POINTS_PER_UNIT = 130;

    enum WeaponOption
    {
        SkyCannon,
        DrillCannon
    };

    static Unit *Create(const ParameterList &parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    GrundstokGunhauler();

    ~GrundstokGunhauler() override = default;

    bool configure(WeaponOption option);

protected:

private:

    Weapon m_cannonShrapnel,
        m_cannonShell,
        m_drillCannon,
        m_carbines,
        m_boardingWeapons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ahead Full                       No
// Escort Vessel                    No
// Bomb Racks                       No
// Disengage                        No
// Fly High                         No
// Drill Cannon                     No
// Sky Cannon                       No
//

} // namespace KharadronOverlords

#endif //GRUNDSTOKGUNHAULER_H
