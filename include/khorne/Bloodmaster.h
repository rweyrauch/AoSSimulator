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

    class Bloodmaster : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Bloodmaster() = delete;

        ~Bloodmaster() override = default;

    protected:

        Bloodmaster(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

    private:

        bool m_usedBloodMustFlow = false;

        Weapon  m_bladeOfBlood{Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Decapitating Strike              Yes
// The Blood Must Flow              Yes
//

} // namespace Khorne
