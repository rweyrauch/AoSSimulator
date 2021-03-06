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

    class FreeguildOutriders : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Repeater_Handgun,
            Blunderbuss,
            Brace_Of_Pistols
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FreeguildOutriders(City city, int numModels, bool trumpeter, WeaponOption sharpshooterWeapon, int points);

        ~FreeguildOutriders() override = default;

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_blunderbuss,
                m_pistols,
                m_handgun,
                m_sabre,
                m_hooves,
                m_sabreSharpshooter;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Trumpeter                        Yes
// Expert Gunners                   Yes
// Skilled Riders                   Yes
//

} // namespace CitiesOfSigmar
