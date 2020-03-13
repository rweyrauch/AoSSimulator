/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NOMADPRINCE_H
#define NOMADPRINCE_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class NomadPrince : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    NomadPrince();
    ~NomadPrince() override = default;

    bool configure();

protected:


private:

    Weapon m_spear;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Harrying Bird of Prey            TODO
// Lord of the Deepwood Host        TODO
//

} // namespace CitiesOfSigmar

#endif //NOMADPRINCE_H