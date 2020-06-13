/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNESMITER_H
#define AURICRUNESMITER_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunesmiter : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricRunesmiter();

        ~AuricRunesmiter() override = default;

        bool configure(Prayer prayer);

    protected:

    private:

        Weapon m_throwingAxe,
                m_runicIron,
                m_latchAxe;
        Prayer m_prayer = Prayer::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Magmic Tunnelling                TODO
// Runic Empowerment                TODO
//

} // namespace Fyreslayers

#endif //AURICRUNESMITER_H