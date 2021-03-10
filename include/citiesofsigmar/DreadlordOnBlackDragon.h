/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class DreadlordOnBlackDragon : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Exile_Blade_And_Shield,
            Exile_Blade_And_Crossbow,
            Lance_And_Shield,
            Lance_And_Crossbow,
            Pair_Exile_Blades
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        DreadlordOnBlackDragon();

        ~DreadlordOnBlackDragon() override = default;

        bool configure(WeaponOption weapon);

    protected:

        void onWounded() override;

        void onRestore() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override {
            if ((m_weaponOption == Pair_Exile_Blades) && (weapon->name() == m_blade.name())) {
                return Reroll_Failed;
            }
            return Unit::toHitRerolls(weapon, target);
        }

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override {
            auto mod = Unit::toSaveModifier(weapon, attacker);

            // Tyrant Shield
            if (m_weaponOption == Lance_And_Shield || m_weaponOption == Exile_Blade_And_Shield) mod++;

            return mod;
        }

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        int getDamageTableIndex() const;

        WeaponOption m_weaponOption = Lance_And_Shield;

        Weapon m_crossbow,
                m_noxiousBreath,
                m_blade,
                m_lance,
                m_jaws,
                m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lance of Spite                   Yes
// Noxious Breath                   Yes
// Paired Exile Blades              Yes
// Tyrant Shield                    Yes
// Do Not Disappoint Me             Yes
//

} // namespace CitiesOfSigmar
