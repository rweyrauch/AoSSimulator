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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Bloodcrushers() = delete;

        ~Bloodcrushers() override;

    protected:

        Bloodcrushers(SlaughterHost host, int numModels, bool iconBearer, bool hornblowers, int points);

        void onCharged() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        void restoreModels(int numModels) override;

        Rerolls hornblowerBattleshockReroll(const Unit *unit);

    private:

        lsignal::slot m_hornblowerSlot;

        Weapon  m_hellblade{Weapon::Type::Melee, "Hellblade", 1, 1, 4, 3, -1, 1},
                m_hellbladeHunter{Weapon::Type::Melee, "Hellblade", 1, 2, 4, 3, -1, 1},
                m_brazenHooves{Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1};

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
