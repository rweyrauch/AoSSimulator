/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <khorne/BloodBlessingsOfKhorne.h>
#include <Weapon.h>

namespace Khorne {

    class Slaughterpriest : public KhorneBase {
    public:

        enum WeaponOption {
            Bloodbathed_Axe,
            Hackblade_And_Wrath_Hammer
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Slaughterpriest(SlaughterHost host, WeaponOption weapon, BloodBlessingsOfKhorne blessing, CommandTrait trait, Artefact artefact, bool isGeneral);

        Slaughterpriest() = delete;

        ~Slaughterpriest() override = default;

    private:

        Weapon m_bloodbathedAxe{Weapon::Type::Melee, "Bloodbathed Axe", 2, 3, 4, 3, 0, 2},
               m_hackblade{Weapon::Type::Melee, "Hackblade", 1, 3, 3, 4, 0, 1},
               m_wrathHammer{Weapon::Type::Melee, "Wrath-hammer", 3, RAND_D3, 4, 4, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Scorn of Sorcery                 Yes
// Blood Boil                       Yes
// Blood Bind                       TODO
//

} // namespace Khorne
