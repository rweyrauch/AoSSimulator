/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "Ironjawz.h"
#include <Weapon.h>

namespace Ironjawz {

    class OrrukWeirdnobShaman : public Ironjawz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        OrrukWeirdnobShaman(Warclan warclan, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~OrrukWeirdnobShaman() override = default;

    protected:

        void onEndHero(PlayerId player) override;

    private:

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brutal Power                     Yes
// Green Puke                       Yes

} // namespace Ironjawz

