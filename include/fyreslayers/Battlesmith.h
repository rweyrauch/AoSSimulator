/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BATTLESMITH_H
#define BATTLESMITH_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers
{

class Battlesmith : public Fyreslayer
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Battlesmith();
    ~Battlesmith() override = default;

    bool configure();

protected:

private:

    Weapon m_throwingAxe,
        m_battleAxe;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Icon of Grimnir                  TODO
// None Shall Defile the Icon       TODO
//

} // namespace Fyreslayers

#endif //BATTLESMITH_H