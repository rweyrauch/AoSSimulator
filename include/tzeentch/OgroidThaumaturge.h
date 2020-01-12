/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef OGROID_H
#define OGROID_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class OgroidThaumaturge : public Unit
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 170;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    OgroidThaumaturge();
    ~OgroidThaumaturge() override = default;

    bool configure();

protected:

private:

    Weapon m_staff,
        m_horns,
        m_hooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brutal Rage                      No
// Mighty Bulk                      No
// Choking Tendrils                 No
//

} // namespace Tzeentch

#endif //OGROID_H
