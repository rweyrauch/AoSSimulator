/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLACKARKCORSAIRS_H
#define BLACKARKCORSAIRS_H

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

        static int ComputePoints(int numModels);

        static void Init();

        BlackArkCorsairs();

        ~BlackArkCorsairs() override = default;

        bool configure(int numModels, bool standardBearer, bool hornblower, WeaponOption weapons);

    protected:

        int toSaveModifier(const Weapon *weapon) const override {
            // Sea Dragon Cloak
            auto mod = CitizenOfSigmar::toSaveModifier(weapon);
            if (weapon->isMissile()) mod++;
            return mod;
        }

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        bool m_standardBearer = false;
        bool m_hornblower = false;

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

#endif //BLACKARKCORSAIRS_H