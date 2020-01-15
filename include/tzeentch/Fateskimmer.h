/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FATESKIMMER_H
#define FATESKIMMER_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class Fateskimmer : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Fateskimmer();
    ~Fateskimmer() override = default;

    bool configure();

protected:

private:

    Weapon m_magicalFlames,
        m_staff,
        m_dagger,
        m_bite;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Arcane Tome                      No
// Sky-shark                        No
// Wake of Fire                     No
// Tzeentch's Firestorm             No
//

} // namespace Tzeentch

#endif //FATESKIMMER_H
