/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt {

    class AbhorrantArchregent : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AbhorrantArchregent(GrandCourt court, Delusion delusion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~AbhorrantArchregent() override = default;

    protected:

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_goryTalonsAndFangs;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Imperial Blood                   Yes
// Ferocious Hunger                 Yes
// Summon Imperial Guard            Yes
//

} // namespace FleshEaterCourt
