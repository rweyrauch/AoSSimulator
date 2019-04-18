/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DOOMSEEKER_H
#define DOOMSEEKER_H

#include <Unit.h>
#include <Weapon.h>

namespace Fyreslayers
{

class Doomseeker : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Doomseeker();
    ~Doomseeker() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_throwingAxe,
        m_warIron,
        m_doomseekerAxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Oathbound                        No
// Runic Power                      No
//

} // namespace Fyreslayers

#endif //DOOMSEEKER_H