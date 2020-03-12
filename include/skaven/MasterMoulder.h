/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MASTERMOULDER_H
#define MASTERMOULDER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class MasterMoulder : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    MasterMoulder();

    ~MasterMoulder() override = default;

    bool configure();

protected:

private:

    Weapon m_lash,
        m_catcher;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Master Moulder
// Crack the Whip
// Unleash More-more Beasts!
//

} // namespace Skaven

#endif //MASTERMOULDER_H