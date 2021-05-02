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

    class DragonOgors : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        DragonOgors() = delete;

        ~DragonOgors() override = default;

        DragonOgors(Greatfray fray, int numModels, int numPairedWeapons, int numGlaives, int numCrushers, int points);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_pairedAncientWeapons,
                m_draconicWarglaive,
                m_draconicCrusher,
                m_rakingForeclaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Storm Rage                       Yes
//

} // namespace BeastsOfChaos
