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

    class Daemonettes : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Daemonettes(int points);

        ~Daemonettes() override;

        bool configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower);

    protected:

        Rerolls chargeRerolls() const override;

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        void restoreModels(int numModels) override;

        Rerolls hornblowerBattleshockReroll(const Unit *unit);

    private:

        Weapon m_piercingClaws,
                m_piercingClawsAlluress;

        lsignal::slot m_hornblowerSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearers                     Yes
// Banner Bearers                   Yes
// Hornblower                       Yes
// Lithe and Swift                  Yes
//

} // Slannesh
