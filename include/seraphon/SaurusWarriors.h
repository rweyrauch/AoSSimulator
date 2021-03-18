/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SaurusWarriors : public SeraphonBase {
    public:

        enum WeaponOption {
            Celestite_Club,
            Celestite_Spear
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusWarriors();

        ~SaurusWarriors() override;

        bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool wardrum);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls chargeRerolls() const override;

        int stardrakeIcon(const Unit *target);

    private:

        WeaponOption m_weaponOption = Celestite_Club;

        Weapon m_celestiteClub,
                m_celestiteClubAlpha,
                m_celestiteSpear,
                m_celestiteSpearAlpha,
                m_jaws;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stardrake Icon                   Yes
// Wardrum                          Yes
// Ordered Cohort                   Yes
//

} // namespace Seraphon
