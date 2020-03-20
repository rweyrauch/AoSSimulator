/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNEFATHER_H
#define AURICRUNEFATHER_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunefather : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricRunefather();

        ~AuricRunefather() override = default;

        bool configure();

    protected:

    private:

        Weapon m_throwingAxe,
            m_grandAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stare Down                       TODO
// Weapon-breaker                   TODO
// Lodge Leader                     TODO
//

} // namespace Fyreslayers

#endif //AURICRUNEFATHER_H