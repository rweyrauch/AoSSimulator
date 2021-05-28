/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class Nagash : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Nagash(CursedBloodline bloodline, bool isGeneral);

        ~Nagash() override = default;

        int castingModifier() const override;

    protected:

        void onWounded() override;

        void onRestore() override;

        int unbindingModifier() const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        size_t getDamageTableIndex() const;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon  m_gaze{Weapon::Type::Missile, "Gaze of Nagash", 12, 1, 3, 2, -1, RAND_D6},
                m_alakanash{Weapon::Type::Melee, "Alakanash", 3, 1, 3, 2, -3, RAND_D6},
                m_zefetNebtar{Weapon::Type::Melee, "Zefet-nebtar", 2, 6, 3, 3, -2, 3},
                m_clawsAndDaggers{Weapon::Type::Melee, "Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Alakanash, the Staff of Power    Yes
// Invocation of Nagash             TODO
// Frightful Touch                  Yes
// Morikhane                        Yes
// The Nine Books of Nagash         Yes
// Hand of Dust                     Yes
// Soul Stealer                     Yes
// Supreme Lord of the Undead       TODO
//

} // namespace Soulblight
