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

namespace CitiesOfSigmar
{

class HelstormRocketBattery : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 130;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    HelstormRocketBattery();
    ~HelstormRocketBattery() override = default;

    bool configure();

protected:


private:

    Weapon m_rocketSalvo,
        m_crewsTools;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Rocket Salvo                     TODO
// Calculated Trajectory            TODO
//

} // namespace CitiesOfSigmar

#endif //ROCKETBATTERY_H