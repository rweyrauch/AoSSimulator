/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class AkhelianMorrsarrGuard : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AkhelianMorrsarrGuard(Enclave enclave, int numModels, bool standardBearers, bool musicians, int points);

        ~AkhelianMorrsarrGuard() override = default;

    protected:

        Rerolls battleshockRerolls() const override;

        Rerolls chargeRerolls() const override;

        void onRestore() override { m_usedBiovoltaicBlast = false; }

        void onStartCombat(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        bool m_usedBiovoltaicBlast = false;

    private:

        Weapon m_voltspear,
                m_voltspearPrince,
                m_fangmoraFangedMaw,
                m_fangmoraLashingTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Biovoltaic Blast                 Yes
// Wave Riders                      Yes
//

} // namespace IdonethDeepkin

