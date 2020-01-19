/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef OGROID_H
#define OGROID_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class OgroidThaumaturge : public TzeentchBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
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
