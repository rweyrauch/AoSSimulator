/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "nurgle/Nurgle.h"

namespace Nurgle {

    class MorbidexTwiceborn : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        MorbidexTwiceborn(PlagueLegion legion, bool isGeneral);

        ~MorbidexTwiceborn() override;

    protected:

        int maliciousMitesWoundMod(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_tongues,
                m_scythe,
                m_claws;

        lsignal::slot m_maliciousMitesSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lord of Nurglings                TODO
// Malicious Mites                  Yes
// Nurgle's Rot                     TODO
// Repugnant Regrowth               TODO
//

} // Nurgle

