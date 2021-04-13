/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class Doomseeker : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Doomseeker();

        ~Doomseeker() override = default;

    protected:

        void configure();

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_throwingAxe,
                m_warIron,
                m_doomseekerAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oathbound                        TODO
// Runic Power                      Yes
//

} // namespace Fyreslayers
