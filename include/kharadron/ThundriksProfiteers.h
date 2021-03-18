/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class ThundriksProfiteers : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ThundriksProfiteers();

        ~ThundriksProfiteers() override = default;

        bool configure();

    protected:

    private:

        Weapon m_volleyGun,
                m_rifle,
                m_privateerPistol,
                m_vulcaniserPistol,
                m_cutter,
                m_gunButt,
                m_skypike;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Thundrik's Profiteers            TODO
//

} // namespace KharadronOverlords

