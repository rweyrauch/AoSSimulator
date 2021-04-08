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

    class Bloodcrushers : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Bloodcrushers() = delete;

        ~Bloodcrushers() override;

    protected:

        Bloodcrushers(SlaughterHost host, int numModels, bool iconBearer, bool hornblowers);

        void onCharged() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        void restoreModels(int numModels) override;

        Rerolls hornblowerBattleshockReroll(const Unit *unit);

    private:

        lsignal::slot m_hornblowerSlot;

        Weapon m_hellblade,
                m_hellbladeHunter,
                m_brazenHooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Icon Bearer                      Yes
// Decapitating Blow                Yes
// Murderous Charge                 Yes
//

} // namespace Khorne
