/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {
    class BatSwarms : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BatSwarms(Legion legion, int numModels, int points);

        ~BatSwarms() override;

    protected:

        int cloudOfHorrors(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_teeth;

        lsignal::slot m_cloudSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cloud of Horror                  Yes
// Blood Suckers                    TODO
//

} //namespace Soulblight
