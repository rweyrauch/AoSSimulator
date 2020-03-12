/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARLOCKENGINEER_H
#define WARLOCKENGINEER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class WarlockEngineer : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    WarlockEngineer();

    ~WarlockEngineer() override = default;

    bool configure();

protected:

private:

    Weapon m_pistol,
        m_blade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// More-more Warp-energy!
// Warp Lightning
//

} // namespace Skaven

#endif //WARLOCKENGINEER_H