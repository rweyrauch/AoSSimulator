/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VERMINLORDWARPSEER_H
#define VERMINLORDWARPSEER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class VerminlordWarpseer : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 320;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    VerminlordWarpseer();

    ~VerminlordWarpseer() override = default;

    bool configure();

protected:

private:

    Weapon m_tails,
        m_glaive;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// The Great Manipulators
// Protection of the Horned Rat
// Scry-orb
// Terrifying
// Dreaded Warpgale
// Forth-forth, Children of the Horned Rat!
//

} // namespace Skaven

#endif //VERMINLORDWARPSEER_H