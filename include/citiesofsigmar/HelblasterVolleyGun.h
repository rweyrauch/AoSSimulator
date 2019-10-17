/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VOLLEYGUN_H
#define VOLLEYGUN_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class HelblasterVolleyGun : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    HelblasterVolleyGun();
    ~HelblasterVolleyGun() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_volley1,
        m_volley2,
        m_volley3,
        m_crewsTools;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Point Blank                      No
// Helblaster Volley                No
// Working Like Clockwork           No
//

} // namespace CitiesOfSigmar

#endif //VOLLEYGUN_H