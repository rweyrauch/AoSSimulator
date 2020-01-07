/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARKANAUTIRONCLAD_H
#define ARKANAUTIRONCLAD_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class ArkanautIronclad : public KharadronBase
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 18;
    static const int POINTS_PER_UNIT = 380;

    static Unit *Create(const ParameterList &parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    ArkanautIronclad();

    ~ArkanautIronclad() override = default;

    bool configure();

protected:

private:

    Weapon m_cannonShrapnel,
        m_cannonShell,
        m_skyhook,
        m_volleyCannon,
        m_torpedoes,
        m_carbines,
        m_boardingWeapons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
//

} // namespace KharadronOverlords

#endif //ARKANAUTIRONCLAD_H
