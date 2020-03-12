/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VERMINLORDWARBRINGER_H
#define VERMINLORDWARBRINGER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class VerminlordWarbringer : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 280;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    VerminlordWarbringer();

    ~VerminlordWarbringer() override = default;

    bool configure();

protected:

private:

    Weapon m_tails,
        m_glaive,
        m_fist;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Amidst the Seething Tide
// Fist of Verminus Supremacy
// Protection of the Horned Rat
// Terrifying
// Dreaded Death Frenzy
// Tyrand of Battle
//

} // namespace Skaven

#endif //VERMINLORDWARBRINGER_H