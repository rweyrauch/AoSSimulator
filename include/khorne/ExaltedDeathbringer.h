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

    class ExaltedDeathbringer : public KhorneBase {
    public:

        enum WeaponOption {
            Ruinous_Axe_And_Skullgouger,
            Bloodbite_Axe_And_Runemarked_Shield,
            Impaling_Spear
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ExaltedDeathbringer() = delete;

        ~ExaltedDeathbringer() override = default;

    protected:

        ExaltedDeathbringer(SlaughterHost host, WeaponOption weapon, CommandTrait trait, Artefact artefact, bool isGeneral);

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    private:

        WeaponOption m_weaponOption = Ruinous_Axe_And_Skullgouger;

        Weapon  m_ruinousAxe{Weapon::Type::Melee, "Ruinous Axe", 1, 3, 4, 3, -1, 2},
                m_bloodbiteAxe{Weapon::Type::Melee, "Bloodbite Axe", 1, 6, 3, 4, 0, 1},
                m_impalingSpear{Weapon::Type::Melee, "Impaling Spear", 2, 5, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blooded Lieutenant               Yes
// Runemarked Shield                Yes
// Skullgouger                      Yes
// Brutal Impalement                Yes
// Brutal Command                   Yes
//

} // namespace Khorne
