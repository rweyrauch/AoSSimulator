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

    class FreeguildPistoliers : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Repeater_Handgun,
            Brace_Of_Pistols
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FreeguildPistoliers(int points);

        ~FreeguildPistoliers() override = default;

        bool configure(int numModels, bool trumpeter, WeaponOption outriderWeapon);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        Rerolls runRerolls() const override;

        Rerolls chargeRerolls() const override;

        void onCharged() override;

    private:

        Weapon m_pistols,
                m_handgun,
                m_sabreAndPistolButt,
                m_hooves,
                m_sabreAndPistolButtOutrider;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Trumpeter                        Yes
// Hail of Bullets                  Yes
// Reckless Riders                  Yes
//

} // namespace CitiesOfSigmar
