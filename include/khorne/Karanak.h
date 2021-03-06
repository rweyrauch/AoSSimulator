/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Karanak : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Karanak() = delete;

        ~Karanak() override = default;

    protected:

        Karanak(SlaughterHost host);

        void onBeginTurn(int battleRound, PlayerId player) override;

        // Unflagging Hunter
        Rerolls chargeRerolls() const override { return Rerolls::Failed; }

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        void onUnboundSpell(Unit *caster, int castRoll) override;

    private:

        Unit *m_pQuarry = nullptr;

        Weapon  m_goreSlickClaws{Weapon::Type::Melee, "Gore-slick Claws", 1, 4, 3, 4, 0, 1},
                m_savageMaws{Weapon::Type::Melee, "Three Savage Maws", 1, 6, 4, 3, -1, RAND_D3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brass Collar of Bloody Vengeance Yes
// Prey of the Blood God            Yes
// Unflagging Hunter                Yes
// Call of the Hunt                 TODO
//

} // namespace Khorne
