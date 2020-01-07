/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ENDRINRIGGERS_H
#define ENDRINRIGGERS_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class Endrinriggers : public KharadronBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    static Unit *Create(const ParameterList &parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Endrinriggers();

    ~Endrinriggers() override = default;

    bool configure();

protected:

private:

    Weapon m_volleyGun,
        m_skyhook,
        m_drillLauncher,
        m_rivetGun,
        m_saw,
        m_gunButt;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Drill Launcher                   No
// Endrincraft                      No
// Grapnel Launcher                 No
// Hitchers                         No
// Skyhook                          No
//

} // namespace KharadronOverlords

#endif //ENDRINRIGGERS_H
