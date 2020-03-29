/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SOULGRINDER_H
#define SOULGRINDER_H


#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class SoulGrinder : public SlavesToDarknessBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SoulGrinder();

        ~SoulGrinder() override = default;

        bool configure();

    protected:


    private:

        Weapon m_cannon,
                m_phlegm,
                m_legs,
                m_claw,
                m_blade,
                m_talon;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hellforged Claw                  TODO
// Implacable Advance               TODO
//

} // SlavesToDarkness

#endif //SOULGRINDER_H
