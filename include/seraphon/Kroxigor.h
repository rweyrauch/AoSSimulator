/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KROXIGOR_H
#define KROXIGOR_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Kroxigor : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Kroxigor();
    ~Kroxigor() override = default;

    bool configure(int numModels, int numMoonhammers);

protected:

private:

    Weapon m_maul,
        m_hammer,
        m_jaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Battle Synergy                   No
// Sweeping Blows                   No
// Jaws like a Steel Trap           No
//

} // namespace Seraphon

#endif //KROXIGOR_H