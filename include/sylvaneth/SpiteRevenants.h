/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class SpiteRevenants : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SpiteRevenants() = delete;

        ~SpiteRevenants() override;

        SpiteRevenants(Glade glade, int numModels);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int unbridledMalice(const Unit *target);

        Rerolls unbridledMaliceReroll(const Unit *unit);

    private:

        Weapon m_cruelTalonsAndFangs,
                m_cruelTalonsAndFangsShadestalker;

        lsignal::slot m_maliceBraverySlot, m_maliceRerollSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unbridled Malice                 Yes
//

} // namespace Sylvaneth
