/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDSKREECHVERMINKING_H
#define LORDSKREECHVERMINKING_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class LordSkreechVerminking : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 320;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    LordSkreechVerminking();

    ~LordSkreechVerminking() override = default;

    bool configure();

protected:

private:

    Weapon m_tails,
        m_glaive,
        m_plaguereaper;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Protection of the Horned Rat     No
// Terrifying                       No
// The Thirteen-headed One          No
// THe Rat King                     No
//

} // namespace Skaven

#endif //LORDSKREECHVERMINKING_H