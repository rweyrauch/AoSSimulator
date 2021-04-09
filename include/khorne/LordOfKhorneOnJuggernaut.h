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

    class LordOfKhorneOnJuggernaut : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        LordOfKhorneOnJuggernaut(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        LordOfKhorneOnJuggernaut() = delete;

        ~LordOfKhorneOnJuggernaut() override = default;

    protected:

        void onCharged() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

    private:

        Weapon m_wrathforgedAxe{Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3},
               m_brazenHooves{Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brass-clad Shield                Yes
// Slaugtherous Charge              Yes
// Daemonic Axe                     Yes
// Blood Stampede                   Yes
//

} // namespace Khorne
