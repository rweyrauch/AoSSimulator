/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSMARAUDERHORSEMEN_H
#define CHAOSMARAUDERHORSEMEN_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosMarauderHorsemen : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            AxeAndShield = 0,
            JavelinAndShield,
            Flail,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosMarauderHorsemen();

        ~ChaosMarauderHorsemen() override;

        bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool drummer);

    protected:

        void onWounded() override;

        int runModifier() const override;

        int chargeModifier() const override;

        void onRestore() override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int iconBearer(const Unit *unit);

    protected:

        bool m_iconBearer = false;
        bool m_hornblower = false;
        bool m_weaponOption = WeaponOption::AxeAndShield;

    private:

        Weapon m_javelinMissile,
                m_axe,
                m_flail,
                m_javelin,
                m_axeMaster,
                m_flailMaster,
                m_javelinMaster,
                m_hooves;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// Hornblower                       Yes
// Darkwood Shields                 Yes
// Mark of Chaos                    Yes
// Barbarian Hordes                 Yes
// Feigned Flight                   Yes
//

} // SlavesToDarkness

#endif //CHAOSMARAUDERHORSEMEN_H