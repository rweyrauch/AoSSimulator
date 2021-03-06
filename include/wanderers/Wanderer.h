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

namespace Wanderers {

    class Wanderer : public Unit {
    public:

        Wanderer() = default;

        ~Wanderer() override = default;

    protected:
        Wanderer(const std::string &name, int move, int wounds, int bravery, int save, bool fly, int points) :
                Unit(name, move, wounds, bravery, save, fly, points) {
            // Melt Away
            m_retreatAndShoot = true;
        }

        // Defiant Hunters
        Rerolls battleshockRerolls() const override { return Rerolls::Failed; }

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Defiant Hunters                  Yes
// Realm Wanderers                  TODO
// Melt Away                        Yes
//

    void Init();

} // namespace Wanderers
