/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MANTRAPPERS_H
#define MANTRAPPERS_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class HrothgornsMantrappers : public MawtribesBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int POINTS_PER_UNIT = 40;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    HrothgornsMantrappers();
    ~HrothgornsMantrappers() override = default;

    bool configure();

protected:

private:

    Weapon m_sharpStuff,
        m_motleyWeapons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Shivering Gnoblars               No
// Hidden Trap                      No
// Here You Go Boss!                No
//

} // namespace OgorMawtribes

#endif //MANTRAPPERS_H
