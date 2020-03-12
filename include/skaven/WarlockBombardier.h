/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARLOCKBOMBARDIER_H
#define WARLOCKBOMBARDIER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class WarlockBombardier : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    WarlockBombardier();

    ~WarlockBombardier() override = default;

    bool configure();

protected:

private:

    Weapon m_doomrocket,
        m_pole;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// More-more Doomrocket!
// Warp Lightning
//

} // namespace Skaven

#endif //WARLOCKBOMBARDIER_H