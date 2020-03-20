/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GYROBOMBERS_H
#define GYROBOMBERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class Gyrobombers : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Gyrobombers();

        ~Gyrobombers() override = default;

        bool configure(int numModels);

    protected:


    private:

        Weapon m_clattergun,
            m_rotorBlades;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Grudgebuster Bombs               TODO
//

} // namespace CitiesOfSigmar

#endif //GYROBOMBERS_H