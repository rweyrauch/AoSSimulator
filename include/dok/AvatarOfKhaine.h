/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AVATAROFKHAINE_H
#define AVATAROFKHAINE_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class AvatarOfKhaine : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    AvatarOfKhaine();
    ~AvatarOfKhaine() override = default;

    bool configure();

protected:

private:

    Weapon m_torrentOfBurningBlood,
        m_sword;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Animated                         TODO
// Idol of Worship                  TODO
//

} // namespace DaughtersOfKhaine

#endif //AVATAROFKHAINE_H