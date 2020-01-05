/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VARGHEISTS_H
#define VARGHEISTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{
class Vargheists : public Unit
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Vargheists();
    ~Vargheists() override = default;

    bool configure(int numModels);

protected:

private:

    Weapon m_fangsAndTalons,
        m_fangsAndTalonsVargoyle;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Blood-maddened Frenzy            No
//

} //namespace Death

#endif //VARGHEISTS_H