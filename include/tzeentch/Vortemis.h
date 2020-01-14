/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VORTEMIS_H
#define VORTEMIS_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class VortemisTheAllSeeing : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    VortemisTheAllSeeing();
    ~VortemisTheAllSeeing() override = default;

    bool configure();

protected:

private:

    Weapon m_staffMissile,
        m_staff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Magic-touched                    No
// Sorcerous Insight                No
//

} // namespace Tzeentch

#endif //VORTEMIS_H
