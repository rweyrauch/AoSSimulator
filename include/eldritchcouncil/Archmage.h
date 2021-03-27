/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace EldritchCouncil {

    class Archmage : public Unit {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Archmage();

        ~Archmage() override = default;

    protected:

        void configure(bool steed);

    private:

        bool m_steed = false;

        Weapon m_seerstaff,
                m_steedHooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Talisman of Arcane Power         TODO
// Elemental Shield                 TODO
//

} // namespace EldritchCouncil
