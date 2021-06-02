/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class GraveGuard : public SoulblightBase {
    public:

        enum WeaponOptions {
            Wight_Blade,
            Great_Wight_Blade,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GraveGuard(CursedBloodline bloodline, int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers, int points);

        ~GraveGuard() override;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int rollChargeDistance() override;

    private:

        WeaponOptions m_weaponOption = Wight_Blade;

        Weapon  m_wightBlade{Weapon::Type::Melee, "Wight Blade", 1, 2, 3, 3, -1, 1},
                m_wightBladeSeneschal{Weapon::Type::Melee, "Wight Blade", 1, 3, 3, 3, -1, 1},
                m_greatWightBlade{Weapon::Type::Melee, "Great Wight Blade", 1, 2, 3, 4, -1, 2},
                m_greatWightBladeSeneschal{Weapon::Type::Melee, "Great Wight Blade", 1, 3, 3, 4, -1, 2};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Cursed Weapons                   Yes
// Crypt Shields                    Yes
//

} //namespace Soulblight

