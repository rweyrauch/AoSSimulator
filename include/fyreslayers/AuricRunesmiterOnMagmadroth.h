/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Magmadroth.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunesmiterOnMagmadroth : public Magmadroth {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricRunesmiterOnMagmadroth();

        ~AuricRunesmiterOnMagmadroth() override = default;

        bool configure(Blessing blessing, MountTrait trait);

    protected:

    private:

        Weapon m_throwingAxe,
                m_latchAxe,
                m_runicIron;
        Blessing m_prayer = Blessing::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
// Grand Ritual of Awakening        TODO
// Runic Empowerment                TODO
//

} // namespace Fyreslayers
