/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAUGHTERMASTER_H
#define SLAUGHTERMASTER_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Slaughtermaster : public MawtribesBase
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Slaughtermaster();
    ~Slaughtermaster() override = default;

    bool configure();

protected:

private:

    Weapon m_stumpBlades,
        m_bite,
        m_assortedWeapons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Bloodgruel                       No
// Great Cauldron                   No
//    Bad Meat                      No
//    Troggoth Guts                 No
//    Spinemarrow                   No
//    Bonecrusher                   No
// Rockchomper                      No
//

} // namespace OgorMawtribes

#endif //SLAUGHTERMASTER_H