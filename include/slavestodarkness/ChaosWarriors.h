/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosWarriors : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Hand_Weapon_And_Shield = 0,
            Halberd_And_Shield,
            Great_Blade,
            Paired_Hand_Weapons
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ChaosWarriors(DamnedLegion legion, MarkOfChaos mark, int numModels, WeaponOption weapons, bool standardBearer, bool hornblower, int points);

        ChaosWarriors() = delete;
        
        ~ChaosWarriors() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

    protected:

        bool m_pairedWeapons = false;
        bool m_hasShields = false;

    private:

        Weapon  m_handWeapons{Weapon::Type::Melee, "Chaos Hand Weapons", 1, 2, 3, 3, 0, 1},
                m_halberd{Weapon::Type::Melee, "Chaos Halberd", 2, 2, 3, 4, 0, 1},
                m_greatBlade{Weapon::Type::Melee, "Chaos Greatblade", 1, 2, 4, 3, -1, 1},
                m_handWeaponsChampion{Weapon::Type::Melee, "Chaos Hand Weapons", 1, 3, 3, 3, 0, 1},
                m_halberdChampion{Weapon::Type::Melee, "Chaos Halberd", 2, 3, 3, 4, 0, 1},
                m_greatBladeChampion{Weapon::Type::Melee, "Chaos Greatblade", 1, 3, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Chaos Runeshields                Yes
// Mark of Chaos                    Yes
// Pair of Chaos Hand Weapons       Yes
// Legions of Chaos                 Yes
//

} // SlavesToDarkness
