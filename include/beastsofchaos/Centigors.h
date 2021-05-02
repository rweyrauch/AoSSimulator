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

    class Centigors : public BeastsOfChaosBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        Centigors() = delete;

        Centigors(Greatfray fray, int numModels, bool brayhorn, bool bannerBearer, int points);

        ~Centigors() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int runModifier() const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        bool m_drunkenRevelry = false;

        Weapon m_centigorSpear{Weapon::Type::Melee, "Centigor Spear", 2, 2, 4, 4, 0, 1};
        Weapon m_centigorSpearGorehoof{Weapon::Type::Melee, "Centigor Spear", 2, 3, 4, 4, 0, 1};
        Weapon m_clawedForelimbs{Weapon::Type::Melee, "Clawed Forelimbs", 1, 2, 5, 5, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brayhorn                         Yes
// Banner Bearer                    Yes
// Beastbucklers                    Yes
// Charging Spear                   Yes
// Drunken Revelry                  Yes
//

} // namespace BeastsOfChaos
