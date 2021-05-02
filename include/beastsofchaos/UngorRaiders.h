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

    class UngorRaiders : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        UngorRaiders() = delete;

        ~UngorRaiders() override = default;

        UngorRaiders(Greatfray fray, int numModels, bool brayhorn, bool bannerBearer, int points);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_raiderBow,
                m_raiderBowHalfhorn,
                m_jaggedShank;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Vile Invaders                    TODO
// Braying Anger                    Yes
//

} // namespace BeastsOfChaos
