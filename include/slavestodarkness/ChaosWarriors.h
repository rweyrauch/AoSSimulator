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

        static int ComputePoints(int numModels);

        static void Init();

        ChaosWarriors();

        ~ChaosWarriors() override = default;

        bool configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblower);

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

        Weapon m_handWeapons,
                m_halberd,
                m_greatBlade,
                m_handWeaponsChampion,
                m_halberdChampion,
                m_greatBladeChampion;

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
