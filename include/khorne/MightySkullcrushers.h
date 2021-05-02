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

    class MightySkullcrushers : public KhorneBase {
    public:

        enum WeaponOption {
            Ensorcelled_Axe,
            Bloodglaive,
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        MightySkullcrushers() = delete;

        ~MightySkullcrushers() override = default;

    protected:

        MightySkullcrushers(SlaughterHost host, int numModels, WeaponOption weapons, bool standardBearer, bool hornblowers, int points);

        int chargeModifier() const override;

        int runModifier() const override;

        int braveryModifier() const override;

        void onCharged() override;

    private:

        WeaponOption m_weaponOption = Ensorcelled_Axe;

        Weapon  m_ensorcelledAxe{Weapon::Type::Melee, "Ensorcelled Axe", 1, 3, 3, 3, 0, 1},
                m_bloodglaive{Weapon::Type::Melee, "Bloodglaive", 1, 3, 4, 3, -1, 1},
                m_ensorcelledAxeHunter{Weapon::Type::Melee, "Ensorcelled Axe", 1, 4, 3, 3, 0, 1},
                m_bloodglaiveHunter{Weapon::Type::Melee, "Bloodglaive", 1, 4, 4, 3, -1, 1},
                m_brazenHooves{Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Standard Bearer                  Yes
// Murderous Charge                 Yes
//

} // namespace Khorne
