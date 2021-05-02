/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace EldritchCouncil {

    class Swordmasters : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Swordmasters(int points);

        ~Swordmasters() override = default;

        bool configure(int numModels, bool hornblower, bool standardBearer);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_greatsword,
                m_greatswordLord;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// A Blur of Blades                 Yes
// Deflect Shots                    Yes
//

} // namespace EldritchCouncil
