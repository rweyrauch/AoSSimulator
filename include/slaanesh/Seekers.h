/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class Seekers : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Seekers(Host host, int numModels, bool iconBearer, bool bannerBearer, bool hornblower, int points);

        ~Seekers() override;

    protected:

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        void restoreModels(int numModels) override;

        Rerolls chargeRerolls() const override;

        int runModifier() const override;

        Rerolls hornblowerBattleshockReroll(const Unit *unit);

    private:

        Weapon m_piercingClaws,
                m_piercingClawsHeartseeker,
                m_poisonedTongue;

        lsignal::slot m_hornblowerSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearers                     Yes
// Banner Bearers                   Yes
// Hornblowers                      Yes
// Quicksilver Speed                Yes
// Soul Hunters                     TODO

} // Slannesh
