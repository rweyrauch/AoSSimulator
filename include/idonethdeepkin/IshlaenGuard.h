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

    class AkhelianIshlaenGuard : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AkhelianIshlaenGuard(Enclave enclave, int numModels, bool standardBearers, bool musicians, int points);

        ~AkhelianIshlaenGuard() override = default;

    protected:

        Rerolls battleshockRerolls() const override;

        Rerolls chargeRerolls() const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_helsabre,
                m_helsabrePrince,
                m_fangmoraFangedMaw,
                m_fangmoraLashingTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Biovoltaic Barrier               Yes
//

} // namespace IdonethDeepkin

