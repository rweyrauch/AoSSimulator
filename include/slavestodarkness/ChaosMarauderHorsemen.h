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

    class ChaosMarauderHorsemen : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Axe_And_Shield = 0,
            Javelin_And_Shield,
            Flail,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosMarauderHorsemen(DamnedLegion legion, MarkOfChaos mark,  int numModels, WeaponOption weapons, bool iconBearer, bool hornblower);

        ChaosMarauderHorsemen() = delete;
        
        ~ChaosMarauderHorsemen() override;

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int iconBearer(const Unit *unit);

    protected:

        bool m_weaponOption = WeaponOption::Axe_And_Shield;

    private:

        Weapon  m_javelinMissile{Weapon::Type::Missile, "Marauder Javelin", 12, 1, 4, 3, -1, 1},
                m_axe{Weapon::Type::Melee, "Barbarian Axe", 1, 2, 4, 4, 0, 1},
                m_flail{Weapon::Type::Melee, "Barbarian Flail", 2, 1, 4, 3, -1, 1},
                m_javelin{Weapon::Type::Melee, "Marauder Javelin", 2, 1, 4, 3, 0, 1},
                m_axeMaster{Weapon::Type::Melee, "Barbarian Axe", 1, 3, 4, 4, 0, 1},
                m_flailMaster{Weapon::Type::Melee, "Barbarian Flail", 2, 2, 4, 3, -1, 1},
                m_javelinMaster{Weapon::Type::Melee, "Marauder Javelin", 2, 2, 4, 3, 0, 1},
                m_hooves{Weapon::Type::Melee, "Trampling Hooves", 1, 2, 4, 4, 0, 1};

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
