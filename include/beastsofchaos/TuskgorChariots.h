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

    class TuskgorChariots : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        TuskgorChariots() = delete;

        ~TuskgorChariots() override = default;

        TuskgorChariots(Greatfray fray, int numModels);

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls chargeRerolls() const override;

    private:

        Weapon m_despoilerAxe,
                m_gnarledSpear,
                m_tusksAndHooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Tuskgor Charge                   Yes
// Despoilers                       Yes
//

} // namespace BeastsOfChaos
