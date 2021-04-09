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

    class Ungors : public BeastsOfChaosBase {
    public:

        enum WeaponOptions {
            Ungor_Blade,
            Gnarled_Shortspear
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Ungors() = delete;

        ~Ungors() override = default;

        Ungors(Greatfray fray, int numModels, WeaponOptions weapons, bool brayhorn, bool bannerBearer);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_ungorBlade,
                m_ungorBladeHalfhorn,
                m_gnarledShortspear,
                m_gnarledShortspearHalfhorn;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Braying Hatred                   Yes
// Half-shields                     Yes
//

} // namespace BeastsOfChaos
