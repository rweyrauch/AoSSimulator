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

        static int ComputePoints(int numModels);

        static void Init();

        Slaughterpriest();

        ~Slaughterpriest() override = default;

    protected:

        void configure(WeaponOption weapon, BloodBlessingsOfKhorne blessing);

    private:

        Weapon m_bloodbathedAxe,
                m_hackblade,
                m_wrathHammer;

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
