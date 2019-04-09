/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WANDERER_H
#define WANDERER_H

#include <Unit.h>
#include <Weapon.h>

namespace Wanderers
{

class Wanderer : public Unit
{
public:

    Wanderer() = default;
    ~Wanderer() override = default;

protected:
    Wanderer(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly)
    {
        // Melt Away
        m_retreatAndShoot = true;
    }

    // Defiant Hunters
    Rerolls battleshockRerolls() const override { return RerollFailed; }

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Defiant Hunters                  Yes
// Realm Wanderers                  No
// Melt Away                        Yes
//

void Init();

} // namespace Wanderers

#endif //WANDERER_H