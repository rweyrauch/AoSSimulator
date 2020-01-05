/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DOOMFIREWARLOCKS_H
#define DOOMFIREWARLOCKS_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class DoomfireWarlocks : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 640;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    DoomfireWarlocks();
    ~DoomfireWarlocks() override = default;

    bool configure(int numModels, bool crossbows);

protected:

private:

    Weapon m_crossBow,
        m_scimitar,
        m_crossBowMaster,
        m_scimitarMaster,
        m_steedsBite;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Doomfire Coven                   No
// Doomfire                         No
//

} // namespace DaughtersOfKhaine

#endif //DOOMFIREWARLOCKS_H