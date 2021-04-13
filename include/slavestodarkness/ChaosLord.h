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

    class ChaosLord : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Reaperblade,
            Daemonbound_Steel,
            Daemonbound_War_Flail
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosLord(DamnedLegion legion, MarkOfChaos mark, WeaponOption weapon, CommandTrait trait, Artefact artefact, bool isGeneral);

        ChaosLord() = delete;

        ~ChaosLord() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_blade{Weapon::Type::Melee, "Reaperblade", 2, 3, 3, 3, -2, 2},
                m_steel{Weapon::Type::Melee, "Daemonbound Steel", 1, 3, 3, 3, -1, 1},
                m_flail{Weapon::Type::Melee, "Daemonbound War-flail", 2, 6, 4, 4, -2, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Daemonbound                      Yes
// Spurred by the Gods              TODO
// Mark of Chaos                    Yes
//

} // SlavesToDarkness
