/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VERMINLORDDECEIVER_H
#define VERMINLORDDECEIVER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class VerminlordDeceiver : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 320;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels){ return POINTS_PER_UNIT; };

    static void Init();

    VerminlordDeceiver();

    ~VerminlordDeceiver() override = default;

    bool configure();

protected:

private:

    Weapon m_doomstar,
        m_tails,
        m_warpstiletto;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Doomstar
// Protection of the Horned Rat
// Shrouded In Darkness
// Terrifying
// Dreaded Skitterleap
// Lord of Assassins
//

} // namespace Skaven

#endif //VERMINLORDDECEIVER_H