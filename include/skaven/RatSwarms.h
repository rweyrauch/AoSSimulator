/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RATSWARMS_H
#define RATSWARMS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class RatSwarms : public Skaventide
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    RatSwarms();
    ~RatSwarms() override = default;

    bool configure(int numModels);

protected:

private:

    Weapon m_teeth;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Endless Tide of Rats             TODO
//

} // namespace Skaven

#endif //RATSWARMS_H