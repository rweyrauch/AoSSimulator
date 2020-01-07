/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKYWARDENS_H
#define SKYWARDENS_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class Skywardens : public KharadronBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    static Unit *Create(const ParameterList &parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Skywardens();

    ~Skywardens() override = default;

    bool configure();

protected:

private:

    Weapon m_volleyGun,
        m_skyhook,
        m_drillLauncher,
        m_pistol,
        m_gunButt,
        m_skypike;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Drill Launcher                   No
// Grapnel Launcher                 No
// Hitchers                         No
// Skyhook                          No
// Skymines                         No
// Timed Charges                    No
//

} // namespace KharadronOverlords

#endif //SKYWARDENS_H
