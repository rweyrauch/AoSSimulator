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

    class Varanguard : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Ensorcelled_Weapon,
            Fellspear,
            Daemonforged_Blade
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Varanguard(DamnedLegion legion, MarkOfChaos mark, int numModels, WeaponOption option);

        Varanguard() = delete;

        ~Varanguard() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onRestore() override;

        void onEndCombat(PlayerId player) override;

    private:

        bool m_usedRelentlessKillers = false;

        Weapon  m_ensorcelledWeapon{Weapon::Type::Melee, "Ensorcelled Weapon", 1, 6, 3, 3, -1, 1},
                m_fellspear{Weapon::Type::Melee, "Fellspear", 2, 3, 3, 4, -1, 2},
                m_blade{Weapon::Type::Melee, "Daemonforged Blade", 1, 3, 3, 3, -1, RAND_D3},
                m_fangs{Weapon::Type::Melee, "Tearing Fangs", 1, 3, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Daemonbound                      Yes
// Favoured of the Everchosen       Yes
// Impaling Charge                  Yes
// Relentless Killers               Yes
// Warpsteel Shields                Yes
//

} // namespace SlavesToDarkness
