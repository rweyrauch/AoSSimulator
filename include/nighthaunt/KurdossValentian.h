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

    class KurdossValentian : public Nighthaunt {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        explicit KurdossValentian(bool isGeneral);

        KurdossValentian() = delete;

        ~KurdossValentian() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon  m_sceptre{Weapon::Type::Melee, "Sepulchral Sceptre", 1, 5, 3, 3, -2, RAND_D3},
                m_claws{Weapon::Type::Melee, "Wraith Herald's Spectral Claws", 1, 6, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  Yes
// If I Cannot Rule, ...            Yes
// Soul-crushing Smite              Yes
// Suffer No Rival                  Yes
//

} // namespace Nighthaunt

