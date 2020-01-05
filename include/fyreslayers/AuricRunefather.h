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

namespace Fyreslayers
{

class AuricRunefather : public Fyreslayer
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Stare Down                       No
// Weapon-breaker                   No
// Lodge Leader                     No
//

} // namespace Fyreslayers

#endif //AURICRUNEFATHER_H