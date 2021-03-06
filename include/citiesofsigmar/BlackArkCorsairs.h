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

    class BlackArkCorsairs : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Repeater_Handbow,
            Wicked_Cutlass
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BlackArkCorsairs(City city, int numModels, bool standardBearer, bool hornblower, WeaponOption weapons, int points);

        ~BlackArkCorsairs() override = default;

    protected:

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override {
            // Sea Dragon Cloak
            auto mod = CitizenOfSigmar::toSaveModifier(weapon, attacker);
            if (weapon->isMissile()) mod++;
            return mod;
        }

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_handbow,
                m_cutlass,
                m_blade,
                m_handbowReaver,
                m_cutlassReaver,
                m_bladeReaver;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Flashing Steel                   Yes
// Sea Dragon Cloak                 Yes
//

} // namespace CitiesOfSigmar
