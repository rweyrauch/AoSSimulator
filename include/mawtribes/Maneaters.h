/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class Maneaters : public MawtribesBase {
    public:

        enum Ability {
            Brawlers,
            Crack_Shots,
            Striders,
            Stubborn
        };

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Maneaters() = delete;

        ~Maneaters() override = default;

    protected:

        Maneaters(Mawtribe tribe, int numModels, Ability ability);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        bool battleshockRequired() const override;

    private:

        Ability m_ability = Brawlers;

        Weapon m_pistolsOrStars{Weapon::Type::Missile, "Pistols or Throwing Stars", 12, 1, 3, 3, -1, RAND_D3},
                m_bashers{Weapon::Type::Melee, "Slicers and Bashers", 1, 4, 3, 3, -1, 2},
                m_bite{Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Been There, Done That            Yes
//   Brawlers                       Yes
//   Crack Shots                    Yes
//   Striders                       Yes
//   Stubborn                       Yes

} // namespace OgorMawtribes

