/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class IcebrowHunter : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        IcebrowHunter() = delete;

        ~IcebrowHunter() override;

    protected:

        IcebrowHunter(Mawtribe tribe, CommandTrait trait, Artefact artefact, bool isGeneral);

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        void onRan() override;

        void onStartMovement(PlayerId player) override;

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_spear{Weapon::Type::Missile, "Great Throwing Spear", 9, 1, 4, 3, -1, RAND_D3},
                m_crossbow{Weapon::Type::Missile, "Hunter's Crossbow", 12, 1, 4, 3, 0, RAND_D3},
                m_club{Weapon::Type::Melee, "Hunter's Culling Club", 1, 4, 3, 3, 0, 2},
                m_bite{Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icy Breath                       Yes
// Masters of Ambush                TODO
// Mighty Throw                     Yes
// Lead the Skal                    Yes
//

} // namespace OgorMawtribes
