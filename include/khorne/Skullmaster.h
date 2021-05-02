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

    class Skullmaster : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Skullmaster() = delete;

        ~Skullmaster() override = default;

    protected:

        Skullmaster(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onCharged() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon  m_bladeOfBlood{Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1},
                m_brazenHooves{Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Slaughter and Ruin               Yes
// Decapitating Blow                Yes
// Slaughterous Charge              Yes
//

} // namespace Khorne
