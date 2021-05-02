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

    class Bestigors : public BeastsOfChaosBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList &parameters);

        Bestigors() = delete;

        Bestigors(Greatfray fray, int numModels, bool brayhorn, bool bannerBearer, int points);

        ~Bestigors() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int runModifier() const override;

    private:

        Weapon m_despoilerAxe{Weapon::Type::Melee, "Despoiler Axe", 1, 2, 4, 3, -1, 1};
        Weapon m_despoilerAxeGougeHorn{Weapon::Type::Melee, "Despoiler Axe", 1, 3, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Despoilers                       Yes
// Beastial Charge                  Yes
//

} // namespace BeastsOfChaos
