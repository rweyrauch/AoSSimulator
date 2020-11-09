/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KARANAK_H
#define KARANAK_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Karanak : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Karanak();

        ~Karanak() override = default;

        bool configure();

    protected:

        void onBeginTurn(int battleRound) override;

        // Unflagging Hunter
        Rerolls chargeRerolls() const override { return Reroll_Failed; }

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        void onUnboundSpell(Unit *caster, int castRoll) override;

    private:

        Unit *m_pQuarry = nullptr;

        Weapon m_goreSlickClaws,
                m_savageMaws;

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

#endif //KARANAK_H