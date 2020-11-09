/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HELBLASTERVOLLEYGUN_H
#define HELBLASTERVOLLEYGUN_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class HelblasterVolleyGun : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        HelblasterVolleyGun();

        ~HelblasterVolleyGun() override = default;

        bool configure();

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_volley1,
                m_volley2,
                m_volley3,
                m_crewsTools;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Point Blank                      Yes
// Helblaster Volley                TODO
// Working Like Clockwork           TODO
//

} // namespace CitiesOfSigmar

#endif //HELBLASTERVOLLEYGUN_H