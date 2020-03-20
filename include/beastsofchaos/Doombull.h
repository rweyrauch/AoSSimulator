/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DOOMBULL_H
#define DOOMBULL_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Doombull : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Doombull();

        ~Doombull() override = default;

        bool configure();

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_bullgorHorns,
                m_slaughtererAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodgreed                       Yes
// Slaughterer's Call               TODO
//

} // namespace BeastsOfChaos

#endif //DOOMBULL_H