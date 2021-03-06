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

    class GorebeastChariots : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Great_Blade_And_Whip,
            War_Flail_And_Whip
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GorebeastChariots(DamnedLegion legion, MarkOfChaos mark, int numModels, WeaponOption weapons, bool idolatorLord,
                          CommandTrait trait, Prayer prayer, bool isGeneral, int points);

        GorebeastChariots() = delete;

        ~GorebeastChariots() override = default;

    protected:

        void onCharged() override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon  m_greatBlade{Weapon::Type::Melee, "Chaos Greatblade", 2, 2, 3, 3, -1, 2},
                m_flail{Weapon::Type::Melee, "Chaos War-flail", 2, RAND_D6, 4, 3, 0, 1},
                m_whip{Weapon::Type::Melee, "Lashing Whip", 2, 2, 4, 4, 0, 1},
                m_greatBladeExalted{Weapon::Type::Melee, "Chaos Greatblade", 2, 2, 2, 3, -1, 2},
                m_flailExalted{Weapon::Type::Melee, "Chaos War-flail", 2, RAND_D6, 3, 3, 0, 1},
                m_whipExalted{Weapon::Type::Melee, "Lashing Whip", 2, 2, 3, 4, 0, 1},
                m_fists{Weapon::Type::Melee, "Crushing Fists", 1, 3, 3, 3, 0, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Crashing Charge                  Yes
// Explosive Brutality              Yes
//

} // SlavesToDarkness
