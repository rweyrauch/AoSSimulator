/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef JABBERSLYTHE_H
#define JABBERSLYTHE_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos {

    class Jabberslythe : public BeastsOfChaosBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Jabberslythe();

        ~Jabberslythe() override = default;

        bool configure();

    protected:


    private:

        Weapon m_slytheyTongue,
                m_vorpalClaws,
                m_spikedTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Madness                  TODO
// Spurting Bile Blood              TODO
//

} // namespace BeastsOfChaos

#endif //JABBERSLYTHE_H