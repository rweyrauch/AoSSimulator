/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEPISTOLIERS_H
#define FREEPISTOLIERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class FreeguildPistoliers : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            RepeaterHandgun,
            BraceOfPistols
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FreeguildPistoliers();

        ~FreeguildPistoliers() override = default;

        bool configure(int numModels, bool trumpeter, WeaponOption outriderWeapon);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        Rerolls runRerolls() const override;

        Rerolls chargeRerolls() const override;

    private:

        bool m_trumpeter = false;

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
// Hail of Bullets                  TODO
// Reckless Riders                  Yes
//

} // namespace CitiesOfSigmar

#endif //FREEPISTOLIERS_H