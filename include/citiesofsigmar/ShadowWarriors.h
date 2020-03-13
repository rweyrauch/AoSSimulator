/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SHADOWWARRIORS_H
#define SHADOWWARRIORS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class ShadowWarriors : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 330;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    ShadowWarriors();
    ~ShadowWarriors() override = default;

    bool configure(int numModels);

protected:


private:

    Weapon m_bow,
        m_blade,
        m_bowWalker;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// One with the Shadows             TODO
// Strike Unseen                    TODO
//

} // namespace CitiesOfSigmar

#endif //SHADOWWARRIORS_H