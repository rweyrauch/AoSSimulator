/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DOOMWHEEL_H
#define DOOMWHEEL_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Doomwheel : public Skaventide
{
public:
    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Doomwheel();
    ~Doomwheel() override = default;

    bool configure();

protected:

private:

    Weapon m_warpBolts,
        m_grindingWheel,
        m_teethAndKnives;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Rolling Doom                     No
// More-more Speed!                 No
// More-more Warp Bolts!            No
//

} // namespace Skaven

#endif //DOOMWHEEL_H