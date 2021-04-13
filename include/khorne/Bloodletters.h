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

    class Bloodletters : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Bloodletters() = delete;

        ~Bloodletters() override;

    protected:

        Bloodletters(SlaughterHost host, int numModels, bool iconBearer, bool standardBearer, bool hornblowers);

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        void restoreModels(int numModels) override;

        Rerolls chargeRerolls() const override;

        Rerolls hornblowerBattleshockReroll(const Unit *unit);

    private:

        Weapon  m_hellblade{Weapon::Type::Melee, "Hellblade", 1, 1, 4, 3, -1, 1},
                m_hellbladeReaper{Weapon::Type::Melee, "Hellblade", 1, 2, 4, 3, -1, 1};

        lsignal::slot m_hornblowerSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Icon Bearer                      Yes
// Standard Bearer                  Yes
// Decapitating Blow                Yes
// Locus of Fury                    Yes
// Murderous Tide                   Yes
//

} // namespace Khorne
