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

    class Chimera : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Chimera();

        ~Chimera() override = default;

        bool configure();

    protected:

        void onWounded() override;

        int getDamageTableIndex() const;

        int chargeModifier() const override;

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_fieryBreath,
                m_avianHead,
                m_draconicHead,
                m_leonineHead,
                m_maulingClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Draconic Head's Fiery Breath     Yes
// Vicious Charge                   Yes
//

} // namespace BeastsOfChaos
