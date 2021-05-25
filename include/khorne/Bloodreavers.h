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

    class Bloodreavers : public KhorneBase {
    public:

        enum WeaponOption {
            Reaver_Blades,
            Meatripper_Axe
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Bloodreavers() = delete;

        Bloodreavers(SlaughterHost host, int numModels, WeaponOption weapons, bool iconBearer, bool hornblowers, int points);

        ~Bloodreavers() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int braveryModifier() const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int runModifier() const override;

        int chargeModifier() const override;

    private:

        WeaponOption m_weaponOption = Reaver_Blades;

        Weapon  m_reaverBlades{Weapon::Type::Melee, "Reaver Blades", 1, 1, 4, 4, 0, 1},
                m_reaverBladesChieftain{Weapon::Type::Melee, "Reaver Blades", 1, 2, 4, 4, 0, 1},
                m_meatripperAxe{Weapon::Type::Melee, "Meatripper Axe", 1, 1, 4, 4, -1, 1},
                m_meatripperAxeChieftain{Weapon::Type::Melee, "Meatripper Axe", 1, 2, 4, 4, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Icon Bearer                      Yes
// Frenzied Devotion                Yes
// Reaver Blades                    Yes
//

} // namespace Khorne
