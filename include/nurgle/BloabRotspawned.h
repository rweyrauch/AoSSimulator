/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOABROTSPAWNED_H
#define BLOABROTSPAWNED_H

#include "nurgle/Nurgle.h"

namespace Nurgle {

    class BloabRotspawned : public NurgleBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloabRotspawned();

        ~BloabRotspawned() override;

        bool configure();

    protected:

        int windspeakerBellsCastingMod(const Unit *caster);

    private:

        Weapon m_bile,
                m_scythe,
                m_claws;

        lsignal::slot m_windspeakerSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Daemon-flies                     TODO
// Windspeaker Bells                Yes
// Miasma of Pestilence             TODO
//

} // Nurgle

#endif //BLOABROTSPAWNED_H
