/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GRIMWRATHBERZERKER_H
#define GRIMWRATHBERZERKER_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers
{

class GrimwrathBerzerker : public Fyreslayer
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    GrimwrathBerzerker();
    ~GrimwrathBerzerker() override = default;

    bool configure();

protected:

private:

    Weapon m_throwingAxe,
        m_greatAxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Unstoppable Berzerker            No
// Battle-fury                      No
// Dead, But Not Defeated           No
//

} // namespace Fyreslayers

#endif //GRIMWRATHBERZERKER_H