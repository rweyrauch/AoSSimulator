/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNTAMEDBEASTS_H
#define UNTAMEDBEASTS_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class UntamedBeasts : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        UntamedBeasts();

        ~UntamedBeasts() override = default;

        bool configure(int numModels);

    protected:

    private:

        Weapon m_harpoonFirstFang,
            m_huntingWeapons,
            m_huntingWeaponsHeartEater;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unleash the Beast                Partial/TODO
//

} // SlavesToDarkness

#endif //UNTAMEDBEASTS_H