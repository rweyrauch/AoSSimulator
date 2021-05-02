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

    class Gors : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        Gors() = delete;

        ~Gors() override = default;

        Gors(Greatfray fray, int numModels, bool pairedBlades, bool brayhorn, bool bannerBearer, int points);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        bool m_pairedBlades = false;

        Weapon m_gorBlade,
                m_gorBladeFoeRender;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rend and Tear                    Yes
// Beastshields                     Yes
// Anarchy and Mayhem               Yes
//

} // namespace BeastsOfChaos
