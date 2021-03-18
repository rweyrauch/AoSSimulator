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

    class ChaosMarauders : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Flail = 0,
            Axe_And_Shield,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosMarauders();

        ~ChaosMarauders() override;

        bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool drummer);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int iconBearer(const Unit *unit);

        int rollChargeDistance() override;

    protected:

        bool m_weaponOption = WeaponOption::Axe_And_Shield;

    private:

        Weapon m_axe,
                m_flail,
                m_axeChieftain,
                m_flailChieftain;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// Drummer                          Yes
// Darkwood Shields                 Yes
// Mark of Chaos                    Yes
// Barbarian Hordes                 Yes
// Boundless Ferocity               Yes
//

} // SlavesToDarkness
