/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-21 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class HellstridersWithClawspears : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        HellstridersWithClawspears();

        ~HellstridersWithClawspears() override;

        bool configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower);

    protected:

        Rerolls chargeRerolls() const override;

        int braveryModifier() const override;

        Rerolls hornblowerBattleshockReroll(const Unit *unit);

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_clawSpear,
                m_clawSpearReaver,
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
// Piercing Strike                  Yes
//
} // Slannesh
