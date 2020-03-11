/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MINDSTEALERSPHIRANX_H
#define MINDSTEALERSPHIRANX_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class MindstealerSphiranx : public SlavesToDarknessBase
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    MindstealerSphiranx();
    ~MindstealerSphiranx() override;

    bool configure();

protected:

    int telepathicDread(const Unit* unit);

private:

    Weapon m_claws,
        m_tail;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Telepathic Dread                 Yes
// Dominate Mind                    No
//

} // SlavesToDarkness

#endif //MINDSTEALERSPHIRANX_H
