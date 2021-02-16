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

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Centigors();

        ~Centigors() override = default;

        bool configure(int numModels, bool brayhorn, bool bannerBearer);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int runModifier() const override;

        int toSaveModifier(const Weapon *weapon) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        bool m_drunkenRevelry = false;

        Weapon m_centigorSpear,
                m_centigorSpearGorehoof;
        Weapon m_clawedForelimbs;

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
