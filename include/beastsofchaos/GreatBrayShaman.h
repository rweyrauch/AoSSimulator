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

    class GreatBrayShaman : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        GreatBrayShaman() = delete;

        ~GreatBrayShaman() override;

        GreatBrayShaman(Greatfray fray, Lore lore, CommandTrait trait, Artefact artefact, bool general);

    protected:

        int infuseWithBestialVigour(const Unit *unit);

    private:

        Weapon m_fetishStaff;
        Lore m_lore = Lore::None;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Infuse with Bestial Vigour       Yes
// Devolve                          TODO
//

} // namespace BeastsOfChaos
