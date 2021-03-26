/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class Gatebreaker : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Gatebreaker();

        ~Gatebreaker() override = default;

    protected:

        void configure();

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int terror(const Unit *unit);

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:
        Weapon m_boulder{Weapon::Type::Missile, "Hurled Boulder", 18, 1, 3, 2, -3, 4},
            m_stomp{Weapon::Type::Melee, "Almighty Stomp", 2, 2, 3, 3, -2, RAND_D3},
            m_grip{Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6},
            m_flail{Weapon::Type::Melee, "Fortcrusha Flail", 3, 10, 4, 3, -3, 3};

        lsignal::slot m_connection;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Almighty Stomp                   Yes
// Crushing Charge                  Yes
// Death Grip                       Yes
// Longshanks                       TODO
// Smash Down                       TODO
// Son of Behemat                   TODO
// Terror                           Yes
// Timberrrrr!                      TODO
// Artefacts
//   Enchanted Portcullis           Yes
//   The Great Wrecka               Yes
//   Kingslaughter Cowl             Yes
//

} // namespace SonsOfBehemat
