/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAMELEONSKINKS_H
#define CHAMELEONSKINKS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class ChameleonSkinks : public SeraphonBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    ChameleonSkinks();
    ~ChameleonSkinks() override = default;

    bool configure(int numModels);

protected:

private:

    Weapon m_dartpipe,
        m_dagger;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Chameleon Ambush                 No
// Perfect Mimicry                  No
// Star-venom                       No
//

} // namespace Seraphon

#endif //CAMELEONSKINKS_H