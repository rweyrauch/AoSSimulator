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

namespace OgorMawtribes {

    class HrothgornsMantrappers : public MawtribesBase {
    public:

        static Unit *Create(const ParameterList &parameters);

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
// Abilities                    Implemented
// -------------------------------------------
// Shivering Gnoblars               TODO
// Hidden Trap                      TODO
// Here You Go Boss!                TODO
//

} // namespace OgorMawtribes

#endif //MANTRAPPERS_H
