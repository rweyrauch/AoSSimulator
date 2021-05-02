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

    class AspiringDeathbringer : public KhorneBase {
    public:

        enum WeaponOption {
            Bloodaxe_And_Wrathhammer,
            Goreaxe_And_Skullhammer,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AspiringDeathbringer() = delete;

        ~AspiringDeathbringer() override;

    protected:

        AspiringDeathbringer(SlaughterHost host, WeaponOption weapon, CommandTrait trait, Artefact artefact, bool isGeneral);

        int baneOfCowards(const Unit *unit, int roll);

    private:

        lsignal::slot m_baneOfCowards;

        WeaponOption m_weaponOption = Bloodaxe_And_Wrathhammer;

        Weapon m_bloodAxe{Weapon::Type::Melee, "Bloodaxe", 1, 3, 3, 4, 0, 1},
                m_wrathHammer{Weapon::Type::Melee, "Wrath-hammer", 3, RAND_D3, 3, 4, 0, 1},
                m_goreaxe{Weapon::Type::Melee, "Goreaxe", 1, 3, 3, 4, 0, 1},
                m_skullhammer{Weapon::Type::Melee, "Skullhammer", 3, 3, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bane of Cowards                  Yes
// Slaughter Incarnate              Yes
//

} // namespace Khorne
