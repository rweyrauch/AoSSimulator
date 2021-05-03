/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class HellstridersWithHellscourges : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        HellstridersWithHellscourges(Host host, int numModels, bool iconBearer, bool bannerBearer, bool hornblower, int points);

        ~HellstridersWithHellscourges() override;

        bool configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower);

    protected:

        Rerolls chargeRerolls() const override;

        int braveryModifier() const override;

        Rerolls hornblowerBattleshockReroll(const Unit *unit);

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_hellscourge,
                m_hellscourgeReaver,
                m_poisonedTongue;

        lsignal::slot m_hornblowerSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Banner                  Yes
// Icon Banner                      Yes
// Hornblower                       Yes
// Hooked Tendrils                  Yes
//
} // Slannesh
