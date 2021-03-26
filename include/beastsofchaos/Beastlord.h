/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Beastlord : public BeastsOfChaosBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        ~Beastlord() override = default;

    protected:

        Beastlord();

        Beastlord(Greatfray fray, CommandTrait trait, Artefact artefact, bool general);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_pairedAxes{Weapon::Type::Melee, "Paired Man-ripper Axes", 1, 6, 3, 3, -1, 1};

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
