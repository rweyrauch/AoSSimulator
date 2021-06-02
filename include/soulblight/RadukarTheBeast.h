/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class RadukarTheBeast : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit RadukarTheBeast(bool isGeneral);

        ~RadukarTheBeast() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        void onEndMovement(PlayerId player) override;

        void onRestore() override;

    private:

        bool m_usedMusteringHowl = false;

        Weapon  m_claws{Weapon::Type::Melee, "Blood-slick Claws", 2, 6, 3, 3, -1, 2},
                m_blade{Weapon::Type::Melee, "Piercing Blade", 1, 6, 3, 3, -1, RAND_D3};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bounding Charge                  Yes
// The Hunger                       Yes
// Supernatural Reflexes            Yes
// Unleashed Ferocity               Yes
// Call to the Hunt                 TODO
// Mustering Howl                   Yes
//

} // namespace Soulblight
