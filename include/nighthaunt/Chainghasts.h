/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt {

    class Chainghasts : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit Chainghasts(int numModels, int points);

        Chainghasts() = delete;

        ~Chainghasts() override;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls anotherLinkInTheChain(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon  m_ghastflailsMissile{Weapon::Type::Missile, "Ghastflails (Missile)", 15, RAND_D3, 4, 3, -2, 1},
                m_ghastflails{Weapon::Type::Melee, "Ghastflails", 2, 0, 4, 3, -1, 1};

        lsignal::slot m_linkInTheChainSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Another Link in the Chain        Yes
// Sweeping Blows                   Yes


} // namespace Nighthaunt

