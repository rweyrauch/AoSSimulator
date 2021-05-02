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

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Doomseeker(Lodge lodge, Artefact artefact);

        Doomseeker() = delete;

        ~Doomseeker() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon  m_throwingAxe{Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 4, 4, -1, 1},
                m_warIron{Weapon::Type::Melee, "Runic War-iron", 1, 3, 3, 3, 0, 1},
                m_doomseekerAxe{Weapon::Type::Melee, "Doomseeker Axe", 1, 3, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oathbound                        TODO
// Runic Power                      Yes
//

} // namespace Fyreslayers
