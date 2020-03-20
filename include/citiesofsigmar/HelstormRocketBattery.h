/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROCKETBATTERY_H
#define ROCKETBATTERY_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class HelstormRocketBattery : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        HelstormRocketBattery();

        ~HelstormRocketBattery() override = default;

        bool configure();

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_rocketSalvo,
                m_crewsTools;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Rocket Salvo                     Yes
// Calculated Trajectory            Yes
//

} // namespace CitiesOfSigmar

#endif //ROCKETBATTERY_H