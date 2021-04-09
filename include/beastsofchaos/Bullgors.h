/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Bullgors : public BeastsOfChaosBase {
    public:

        enum WeaponOptions {
            Bullgor_Axe,
            Paired_Bullgor_Axes,
            Bullgor_Great_Axe
        };

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Bullgors() = delete;

        Bullgors(Greatfray fray, int numModels, WeaponOptions weapon, bool drummer, bool bannerBearer);

        ~Bullgors() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int chargeModifier() const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        bool m_pairedAxes = false;

        Weapon m_bullgorHorns{Weapon::Type::Melee, "Bullgor Horns", 1, 2, 4, 4, 0, 1};
        Weapon m_bullgorAxe{Weapon::Type::Melee, "Bullgor Axe", 1, 3, 4, 3, -1, 2};
        Weapon m_bullgorAxeBloodkine{Weapon::Type::Melee, "Bullgor Axe", 1, 4, 4, 3, -1, 2};
        Weapon m_bullgorGreatAxe{Weapon::Type::Melee, "Bullgor Great Axe", 1, 2, 4, 3, -2, 3};
        Weapon m_bullgorGreatAxeBloodkine{Weapon::Type::Melee, "Bullgor Great Axe", 1, 3, 4, 3, -2, 3};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodgreed                       Yes
// Dual Axes                        Yes
// Bullshields                      Yes
//

} // namespace BeastsOfChaos
