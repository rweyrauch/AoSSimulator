/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SALAMANDERS_H
#define SALAMANDERS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Salamanders : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 50;
    static const int POINTS_MAX_UNIT_SIZE = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Salamanders();
    ~Salamanders() override = default;

    bool configure(int numModels);

protected:

private:

    Weapon m_streamOfFire,
        m_bite;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Goaded to Fury                   No
// It Burns!
//

} // namespace Seraphon

#endif //SALAMANDERS_H