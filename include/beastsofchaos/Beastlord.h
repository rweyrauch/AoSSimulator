/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BEASTLORD_H
#define BEASTLORD_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Beastlord : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int /*numModels*/) { return POINTS_PER_UNIT; };

    Beastlord();
    ~Beastlord() override = default;

    bool configure();

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_pairedAxes;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Dual Axes                        Yes
// Hatred of Heroes                 Yes
// Grisly Trophy                    TODO
//

} // namespace BeastsOfChaos

#endif //BEASTLORD_H