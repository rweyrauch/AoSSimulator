/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Greenskinz {

    class Orruks : public Unit {
    public:

        enum WeaponOption {
            Choppa_And_Shield,
            Spear_And_Shield,
            Paired_Choppas,
            Orruk_Bow_And_Cutta
        };

        enum StandardOption {
            None,
            Orruk_Banner,
            Skull_Icon
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        Orruks();

        ~Orruks() override = default;

        bool configure(int numModels, WeaponOption weapons, bool drummer, StandardOption standardBearer);

    protected:

        int chargeModifier() const override;

        int braveryModifier() const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        WeaponOption m_weaponOption = Choppa_And_Shield;

        Weapon m_orrukBows,
                m_choppa,
                m_pigstikkaSpear,
                m_cutta,
                m_orrukBowBoss,
                m_choppaBoss,
                m_pigstikkaSpearBoss,
                m_cuttaBoss;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Orruk Banner                     Yes
// Skull Icon                       TODO
// Choppas                          Yes
// Mob Rule                         Yes
// Waaagh! Shield                   Yes
// Ready Boyz! Air! Fire!           Yes
//

} // namespace Greenskinz
