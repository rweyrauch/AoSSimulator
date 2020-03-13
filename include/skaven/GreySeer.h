/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GREYSEER_H
#define GREYSEER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class GreySeer : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    GreySeer();

    ~GreySeer() override = default;

    bool configure();

protected:

private:

    Weapon m_staff;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Warpstone Tokens                 TODO
// Wither                           TODO
//

} // namespace Skaven

#endif //GREYSEER_H