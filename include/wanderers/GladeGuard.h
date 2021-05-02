/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <wanderers/Wanderer.h>
#include <Weapon.h>

namespace Wanderers {

    class GladeGuard : public Wanderer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GladeGuard(int numModels, bool pennantBearer, bool hornblower, int points);

        ~GladeGuard() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls runRerolls() const override;

        int braveryModifier() const override;

        void onStartShooting(PlayerId player) override;

    private:

        bool m_usedArcaneBodkins = false;

        Weapon m_longbow,
                m_longbowLord,
                m_gladeBlade;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Peerless Archery                 Yes
// Ancane Bodkins                   Yes
// Pennant Bearer                   Yes
// Hornblower                       Yes
//

} // namespace Wanderers
