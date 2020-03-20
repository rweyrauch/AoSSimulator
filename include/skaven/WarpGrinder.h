/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARPGRINDER_H
#define WARPGRINDER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class WarpGrinder : public Skaventide
{
public:

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels);

    static void Init();

    WarpGrinder();

    ~WarpGrinder() override = default;

    bool configure();

protected:

private:

    Weapon m_warpGrinder;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Tunnel Skulkers                  TODO
//

} // namespace Skaven

#endif //WARPGRINDER_H